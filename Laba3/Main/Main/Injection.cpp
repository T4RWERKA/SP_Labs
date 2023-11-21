#include "Injection.h"

#include <stdio.h>

const char *DLL_PATH = "D:\Study\SP\SP_Labs\Laba3\Dll1\Debug\DLL1.dll";

void InjectDLL(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
    HANDLE hThread;
    MYDATA param{"laba", "replacement"};
    DWORD hRemoteModule, hRemoteFunc;

    if (hProcess && hProcess != INVALID_HANDLE_VALUE)
    {
        void *ptr = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (ptr)
        {
            WriteProcessMemory(hProcess, ptr, DLL_PATH, strlen(DLL_PATH) + 1, 0);
            hRemoteFunc = (DWORD)GetProcAddress(GetModuleHandle(L"Kernel32.dll"), "LoadLibraryA");
            hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)hRemoteFunc, ptr, 0, NULL);
            if (hThread)
            {
                WaitForSingleObject(hThread, INFINITE);
                
                GetExitCodeThread(hThread, &hRemoteModule);

                CloseHandle(hThread);
                VirtualFreeEx(hProcess, ptr, 0, MEM_RELEASE);

                if (hRemoteModule)
                {
                    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)FreeLibrary, (LPVOID)FreeLibrary, 0, NULL);

                    if (hThread)
                    {
                        WaitForSingleObject(hThread, INFINITE);
                        CloseHandle(hThread);
                    }
                }
            }
        }
        CloseHandle(hProcess);
    }
    
}