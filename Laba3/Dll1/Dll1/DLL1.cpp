#include "pch.h"
#include "DLL1.h"
#include <iostream>

std::string MyFunc(std::string str)
{
	std::cout << str;
	return str;
}

BOOL VirtualReplace(const char *data, const char *replacement)
{
	SIZE_T len = strlen(data);
	SIZE_T rep_len = strlen(replacement);
	SIZE_T min_len = len;
	SYSTEM_INFO system_info;
	HANDLE process = GetCurrentProcess();
	MEMORY_BASIC_INFORMATION info;
	char *buff, *curr_pos;
	BOOL is_replaced = FALSE;

	if (len > rep_len)
		min_len = rep_len;

	GetSystemInfo(&system_info);

	curr_pos = (char *)system_info.lpMinimumApplicationAddress;
	while (curr_pos < system_info.lpMaximumApplicationAddress)
	{
		if (VirtualQuery(curr_pos, &info, sizeof(info)) == sizeof(info))
		{
			if (info.State == MEM_COMMIT && info.AllocationProtect != PAGE_NOACCESS)
			{
				curr_pos = (char *)info.BaseAddress;
				buff = (char *)malloc(info.RegionSize * sizeof(char));
				SIZE_T bytes_read;

				DWORD oldProtect = 0;
				DWORD newProtect = PAGE_READWRITE;

				if (ReadProcessMemory(process, curr_pos, buff, info.RegionSize, &bytes_read))
				{
					for (int i = 0; i < (bytes_read - len); ++i)
					{
						if (memcmp(&data[0], &buff[i], len) == 0)
						{
							char *ref = curr_pos + i;

							VirtualProtectEx(process, curr_pos, info.RegionSize, newProtect, &oldProtect);

							memcpy(ref, replacement, min_len);
							ZeroMemory(ref + min_len, len - min_len + 1);

							VirtualProtectEx(process, curr_pos, info.RegionSize, oldProtect, &newProtect);
							is_replaced = TRUE;
						}
					}
				}

				free(buff);
			}

			curr_pos += info.RegionSize;
		}
	}

	return is_replaced;
}