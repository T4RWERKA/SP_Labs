#include <iostream>
#include <Windows.h>

int main()
{
    const char *data = "laba";
    printf("%d\n", GetCurrentProcessId());
    getchar();
    printf("%s", data);
}
