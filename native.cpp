#define WIN32_LEAN_AND_MEAN 1

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tlhelp32.h>

DWORD IsProcessRunningByName(char name[])
{
  HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  if (snap == INVALID_HANDLE_VALUE)
  {
    fprintf(stderr, "Unexpected error occured: CreateToolhelp32Snapshot");
    return FALSE;
  }

  PROCESSENTRY32 entry;
  entry.dwSize = sizeof(PROCESSENTRY32);

  if (!Process32First(snap, &entry))
  {
    fprintf(stderr, "Unexpected error occured: Process32First");
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

BOOL IsProcessRunningById(DWORD id)
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
      fprintf(stderr, "Unexpected error occured: OpenProcess: %d\n", lastError);
      return FALSE;
    }
  }

  DWORD exitCode;
  BOOL success = GetExitCodeProcess(proc, &exitCode);
  if (!success)
  {
    fprintf(stderr, "Unexpected error occured: GetExitCodeProcess: %d\n", GetLastError());
    CloseHandle(proc);
    return FALSE;
  }

  CloseHandle(proc);
  return exitCode == STILL_ACTIVE;
}

void PrintResults(
    char type[],
    BOOL result,
    LARGE_INTEGER frequency,
    LARGE_INTEGER startingTime,
    LARGE_INTEGER endingTime)
{
  LARGE_INTEGER millis;
  millis.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
  millis.QuadPart *= 1000;
  millis.QuadPart /= frequency.QuadPart;

  LARGE_INTEGER micros;
  micros.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
  micros.QuadPart *= 1000000;
  micros.QuadPart /= frequency.QuadPart;

  LARGE_INTEGER nanos;
  nanos.QuadPart = endingTime.QuadPart - startingTime.QuadPart;
  nanos.QuadPart *= 1000000000;
  nanos.QuadPart /= frequency.QuadPart;

  printf(
      "native,%s,%s,%I64d,%I64d,%I64d\n",
      type, result ? "true" : "false", millis.QuadPart, micros.QuadPart, nanos.QuadPart);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fputs("Usage: native.exe <pid|name>\n", stderr);
    return 1;
  }

  LARGE_INTEGER frequency;
  QueryPerformanceFrequency(&frequency);

  for (char *c = argv[1]; *c; ++c)
  {
    if (!isdigit(*c))
    {
      char *name = argv[1];
      LARGE_INTEGER startingTime, endingTime;
      QueryPerformanceCounter(&startingTime);
      BOOL result = IsProcessRunningByName(name);
      QueryPerformanceCounter(&endingTime);
      PrintResults("byName", result, frequency, startingTime, endingTime);
      return 0;
    }
  }

  DWORD pid = atoi(argv[1]);
  LARGE_INTEGER startingTime, endingTime;
  QueryPerformanceCounter(&startingTime);
  BOOL result = IsProcessRunningById(pid);
  QueryPerformanceCounter(&endingTime);
  PrintResults("byId", result, frequency, startingTime, endingTime);
  return 0;
}