#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>

#include "offsets/offsets.hpp"
#include "settings.hpp"
#include "csgosdk/structures.hpp"
#include <conio.h>

using std::cout;
using std::endl;
using std::hex;
using std::dec;

template <class dataType>
bool wpm(dataType value, DWORD addy) {
	return WriteProcessMemory(process_handle, (PVOID)addy, &value, sizeof(dataType), 0);
}

template <class dataType>
dataType rpm(DWORD addy) {
	dataType data;
	ReadProcessMemory(process_handle, (PVOID)addy, &data, sizeof(dataType), 0);
	return data;
}


bool attach()
{
	const wchar_t* target_process = L"csgo.exe";

	PROCESSENTRY32 procEntry32;
	procEntry32.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (hProcSnap == INVALID_HANDLE_VALUE)
		return false;

	if (Process32First(hProcSnap, &procEntry32))
	{
		if (!wcscmp(target_process, procEntry32.szExeFile))
		{
			process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			if (!process_handle) {
				CloseHandle(hProcSnap);
				return false;
			}

			process_id = procEntry32.th32ProcessID;

			CloseHandle(hProcSnap);
			return true;
		}
	}

	while (Process32Next(hProcSnap, &procEntry32))
	{
		if (!wcscmp(target_process, procEntry32.szExeFile))
		{
			process_handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procEntry32.th32ProcessID);
			if (!process_handle) {
				CloseHandle(hProcSnap);
				return false;
			}

			process_id = procEntry32.th32ProcessID;

			CloseHandle(hProcSnap);
			return true;
		}
	}

	CloseHandle(hProcSnap);
	return false;
}


DWORD get_module_handle(const wchar_t* moduleName)
{
	HANDLE hSnapShot;
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(MODULEENTRY32);

	if (hSnapShot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	if (Module32First(hSnapShot, &mEntry))
	{
		if (!wcscmp(moduleName, mEntry.szModule))
		{
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}

	while (Module32Next(hSnapShot, &mEntry))
	{
		if (!wcscmp(moduleName, mEntry.szModule))
		{
			CloseHandle(hSnapShot);
			return (DWORD)mEntry.modBaseAddr;
		}
	}
	CloseHandle(hSnapShot);
	return false;
}

bool initialize() {
	printf("\n");
	//sub main startup function
	if (!attach()) {
		printf(" [!]: csgo.exe is not runned\n");
		return false;
	}

	client = get_module_handle(L"client.dll");
	engine = get_module_handle(L"engine.dll");

	if (!client || !engine) {
		printf(" [-]: failed getting module handle\n");
		return false;
	}

	return true;
}
