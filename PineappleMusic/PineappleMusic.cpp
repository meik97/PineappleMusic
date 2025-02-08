// ==========================================================
// Project: PineappleMusic
// 
// Component: pineapplemusic.dll
//          
// Initial author: meik97
//
// Started: 18.01.2025
// ==========================================================

#include "Components/include/Include.hpp"
#include "Components/modules/QuickPatch.hpp"
#include "Components/utils/Utils.hpp"

HINSTANCE MODULE = NULL;

DWORD WINAPI DLLFunction(LPVOID lpParam)
{
	FILE* x;
	AllocConsole();
	freopen_s(&x, "CONOUT$", "w", stdout);
	TCHAR szFileName[MAX_PATH];

	GetModuleFileName(NULL, szFileName, MAX_PATH);

	//Apply patches
	Modules::QuickPatch(std::wstring(szFileName).substr(std::wstring(szFileName).find_last_of(L"\\/") + 1));
	std::cout << "Pinapple Music v0.0.1" << std::endl;
	
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
	if (dwAttached == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, 0, &DLLFunction, NULL, 0, NULL);
		MODULE = hModule;
	}
	return 1;
}
