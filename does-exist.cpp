#define WIN32_LEAN_AND_MEAN 1

#include <windows.h>
#include <stdio.h>

int main()
{
  HANDLE file = CreateFile(
      "bench.pid",           // lpFileName
      GENERIC_WRITE,         // dwDesiredAccess
      0,                     // dwShareMode
      NULL,                  // lpSecurityAttributes
      CREATE_ALWAYS,         // dwCreationDisposition
      FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes
      NULL                   // hTemplateFile
  );

  DWORD pid = GetCurrentProcessId();
  char pidBuf[16];
  sprintf(pidBuf, "%d", pid);

  WriteFile(
      file,           // hFile
      &pidBuf,        // lpBuffer
      strlen(pidBuf), // nNumberOfBytesToWrite
      0,              // lpNumberOfBytesWritten
      NULL            // lpOverlapped
  );

  CloseHandle(file);

  while (TRUE)
  {
    // Wait to be terminated
  }
}
