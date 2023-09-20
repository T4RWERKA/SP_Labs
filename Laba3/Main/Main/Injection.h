#pragma once

#include <Windows.h>

typedef struct MyData
{
    const char *data;
    const char *replacement;
} MYDATA, *PMYDATA;

void InjectDLL(DWORD dwProcessId);