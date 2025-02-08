#include <iostream>
#include <string>
#include <cstring>
#include <windows.h>
#include <TlHelp32.h>

DWORD GetProcId(const wchar_t* procName)
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

int main()
{
	HANDLE hProcess;
	DWORD dwProcess;
	const char* DLL_NAME;
	char DLL_PATH[MAX_PATH];

	dwProcess = GetProcId(L"AMPLibraryAgent.exe");
	DLL_NAME = "PineappleMusic.dll";


	GetFullPathNameA(DLL_NAME, MAX_PATH, DLL_PATH, NULL);

	while (!dwProcess)
	{
		dwProcess = GetProcId(L"AMPLibraryAgent.exe");
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