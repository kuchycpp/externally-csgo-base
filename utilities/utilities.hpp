#include <cstddef>
#include <memory>
#include <type_traits>
#include <iostream>
#include <utility>
#include "../main.hpp"

bool verifying_address(HANDLE hProcess, DWORD dwAddress, PBYTE bMask, char* szMask)
{
	PBYTE* pTemp = { 0 };

	for (int i = 0; *szMask; ++szMask, ++bMask, ++i)
	{
		if (!ReadProcessMemory(hProcess, reinterpret_cast<LPCVOID>(dwAddress + i), &pTemp, 2048, 0))
			return false;

		if (*szMask == 'x' && reinterpret_cast<char*>(pTemp) != reinterpret_cast<char*>(*bMask))
			return false;
	}

	return true;
}

namespace utilities {
	DWORD find_pattern(HANDLE hProcess, PBYTE bMask, char* szMask)
	{
		for (DWORD dwCurrentAddress = 0x401000; dwCurrentAddress < 0x7FFFFFF; dwCurrentAddress++)
			if (verifying_address(hProcess, dwCurrentAddress, bMask, szMask))
				return dwCurrentAddress;

		return -1;
	}

	bool world_to_screen(vector pos, vector& screen, int window_width, int window_height)
	{
		vec4 coords;
		float matrix[16];
		coords.x = pos.x * matrix[0] + pos.y * matrix[1] + pos.z * matrix[2] + matrix[3];
		coords.y = pos.x * matrix[4] + pos.y * matrix[5] + pos.z * matrix[6] + matrix[7];
		coords.z = pos.x * matrix[8] + pos.y * matrix[9] + pos.z * matrix[10] + matrix[11];
		coords.w = pos.x * matrix[12] + pos.y * matrix[13] + pos.z * matrix[14] + matrix[15];

		if (coords.w < 0.1f)
			return false;

		vector NDC;
		NDC.x = coords.x / coords.w;
		NDC.y = coords.y / coords.w;
		NDC.z = coords.z / coords.w;

		screen.x = (window_width / 2 * NDC.x) + (NDC.x + window_width / 2);
		screen.y = -(window_height / 2 * NDC.y) + (NDC.y + window_height / 2);

		return true;
	};
}