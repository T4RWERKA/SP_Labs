#pragma once
#include <string>

#ifdef DLL1_EXPORTS
#define DLL1_API __declspec(dllexport)
#else
#define DLL1_API __declspec(dllimport)
#endif

extern "C" DLL1_API std::string MyFunc(std::string str);
extern "C" DLL1_API BOOL VirtualReplace(const char *data, const char *replacement);
