#include <iostream>
#include <windows.h> 

#include "DLL1.h"
#include "Injection.h"

typedef std::string (*TMyFunc)(std::string);

int main()
{
    const char *replacement = "replacement";
    const char *data = "laba";
    BOOL is_replaced;
    DWORD pid;

    std::cin >> pid;
    InjectDLL(pid);

    is_replaced = VirtualReplace(data, replacement);

    printf("%s\n", data);

    HINSTANCE hinstLib;
    TMyFunc ProcAdd;
    BOOL fRunTimeLinkSuccess = FALSE;

    hinstLib = LoadLibrary(TEXT("DLL1.dll"));

    if (hinstLib != NULL)
    {
        ProcAdd = (TMyFunc)GetProcAddress(hinstLib, "MyFunc");

        if (NULL != ProcAdd)
        {
            fRunTimeLinkSuccess = TRUE;
            ProcAdd("Run-time import\n");
        }

        FreeLibrary(hinstLib);
    }

    if (!fRunTimeLinkSuccess)
        printf("No run-time import\n");

    return 0;
}
