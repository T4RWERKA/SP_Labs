#include "Injection.h"

#include <stdio.h>

const char *DLL_PATH = "D:\Study\SP\SP_Labs\Laba3\Dll1\x64\DLL1.dll";
typedef BOOL(*TVirtualReplace)(const char *, const char *);

DWORD CALLBACK CallDLLThread(LPVOID lpParam)
{
    //HINSTANCE hModule = GetModuleHandleA("DLL1.dll");
    //if (hModule)
    //{
    //    TVirtualReplace fn = (TVirtualReplace)GetProcAddress(hModule, "VirtualReplace");
    //    printf("%d %d", (int)hModule, (int)fn);

    //    //fn(((PMYDATA)lpParam)->data, ((PMYDATA)lpParam)->replacement);
    //}

    return 1;
}

void InjectDLL(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
    HANDLE hThread;
    MYDATA param{"laba", "replacement"};

    if (hProcess && hProcess != INVALID_HANDLE_VALUE)
    {
        void *ptr = VirtualAllocEx(hProcess, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

        if (ptr)
        {
            WriteProcessMemory(hProcess, ptr, DLL_PATH, strlen(DLL_PATH) + 1, 0);
            hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, ptr, 0, NULL);
            if (hThread)
            {
                CloseHandle(hThread);
                VirtualFreeEx(hProcess, ptr, 0, MEM_RELEASE);

                ptr = VirtualAllocEx(hProcess, 0, sizeof(MYDATA), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
                if (ptr)
                {
                    WriteProcessMemory(hProcess, ptr, &param, sizeof(MYDATA), 0);
                    hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CallDLLThread, ptr, 0, NULL);
                    VirtualFreeEx(hProcess, ptr, 0, MEM_RELEASE);
                }
            }
        }
        CloseHandle(hProcess);
    }
    
}