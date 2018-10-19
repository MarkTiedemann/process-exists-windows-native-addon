#include <node_api.h>
#include <windows.h>
#include <tlhelp32.h>

BOOL is_process_running_by_id(DWORD id)
{
  if (id == 0)
  {
    return TRUE;
  }

  HANDLE proc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, id);
  if (proc == NULL)
  {
    DWORD lastError = GetLastError();
    if (lastError == ERROR_ACCESS_DENIED)
    {
      return TRUE;
    }
    else if (lastError == ERROR_INVALID_PARAMETER)
    {
      return FALSE;
    }
    else
    {
      // TODO: add proper error handling
      return FALSE;
    }
  }

  DWORD exitCode;
  BOOL success = GetExitCodeProcess(proc, &exitCode);
  if (!success)
  {
    // TODO: add proper error handling
    CloseHandle(proc);
    return FALSE;
  }

  CloseHandle(proc);
  return exitCode == STILL_ACTIVE;
}

BOOL is_process_running_by_name(char name[])
{
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snap == INVALID_HANDLE_VALUE)
  {
    // TODO: add proper error handling
    return FALSE;
  }

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(snap, &entry))
  {
    // TODO: add proper error handling
    CloseHandle(snap);
    return FALSE;
  }

  do
  {
    if (strcmp(name, entry.szExeFile) == 0)
    {
      CloseHandle(snap);
      return TRUE;
    }
  } while (Process32Next(snap, &entry));

  CloseHandle(snap);
  return FALSE;
}

napi_value wrap_by_id(napi_env env, napi_callback_info info)
{
  napi_status status;
  napi_value result;

  size_t argc = 1;
  napi_value argv[1];
  status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
  if (status != napi_ok)
    return nullptr;

  int pid;
  status = napi_get_value_int32(env, argv[0], &pid);
  if (status != napi_ok)
    return nullptr;

  BOOL is_running = is_process_running_by_id(pid);

  status = napi_get_boolean(env, is_running, &result);
  if (status != napi_ok)
    return nullptr;

  return result;
}

napi_value wrap_by_name(napi_env env, napi_callback_info info)
{
  napi_status status;
  napi_value result;

  size_t argc = 1;
  napi_value argv[1];
  status = napi_get_cb_info(env, info, &argc, argv, nullptr, nullptr);
  if (status != napi_ok)
    return nullptr;

  // https://docs.microsoft.com/en-us/windows/desktop/FileIO/naming-a-file#maximum-path-length-limitation
  char name[260];
  size_t str_len;
  status = napi_get_value_string_utf8(env, argv[0], (char *)&name, 260, &str_len);
  if (status != napi_ok)
    return nullptr;

  BOOL is_running = is_process_running_by_name(name);

  status = napi_get_boolean(env, is_running, &result);
  if (status != napi_ok)
    return nullptr;

  return result;
}

napi_value init(napi_env env, napi_value exports)
{
  napi_status status;

  napi_value by_id;
  status = napi_create_function(env, nullptr, 0, wrap_by_id, nullptr, &by_id);
  if (status != napi_ok)
    return nullptr;
  status = napi_set_named_property(env, exports, "byId", by_id);
  if (status != napi_ok)
    return nullptr;

  napi_value by_name;
  status = napi_create_function(env, nullptr, 0, wrap_by_name, nullptr, &by_name);
  if (status != napi_ok)
    return nullptr;
  status = napi_set_named_property(env, exports, "byName", by_name);
  if (status != napi_ok)
    return nullptr;

  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
