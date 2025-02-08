#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>
#include <TlHelp32.h>

DWORD getProcId(const wchar_t* procName)
{
	PROCESSENTRY32 procEntry;
	procEntry.dwSize = sizeof(procEntry);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (hSnap == INVALID_HANDLE_VALUE)
		return 0;

	if (Process32First(hSnap, &procEntry))
	{
		do
		{
			if (std::wcscmp(procEntry.szExeFile, procName) == 0)
			{
				CloseHandle(hSnap);
				return procEntry.th32ProcessID;
			}
		} while (Process32Next(hSnap, &procEntry));
	}

	CloseHandle(hSnap);
	return 0;
}

bool inject(const wchar_t* procName, const char* dll)
{
	HANDLE hProcess;
	DWORD dwProcess = 0;
	char DLL_PATH[MAX_PATH];

	GetFullPathNameA(dll, MAX_PATH, DLL_PATH, NULL);

	while (!dwProcess)
	{
		dwProcess = getProcId(procName);
		Sleep(10);
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcess);

	if (hProcess && hProcess != INVALID_HANDLE_VALUE)
	{
		HANDLE hThread;
		LPVOID allocMem;

		allocMem = VirtualAllocEx(hProcess, NULL, MAX_PATH, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		WriteProcessMemory(hProcess, allocMem, DLL_PATH, strlen(DLL_PATH) + 1, NULL);

		hThread = CreateRemoteThread(hProcess, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, allocMem, 0, 0);

		if (hThread)
			CloseHandle(hThread);

		CloseHandle(hProcess);
	}

	return true;
}

int main()
{
	inject(L"AMPLibraryAgent.exe", "PineappleMusic.dll");
	//add injection to applemusic.exe --> requires .dll to be able to distinguish between targets

	return true;
}