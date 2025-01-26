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

DWORD WINAPI DLLFunction(LPVOID lpParam)
{
	FILE* x;
	AllocConsole();
	freopen_s(&x, "CONOUT$", "w", stdout);

	//Apply patches
	Modules::QuickPatch();
	std::cout << "Pinapple Music v0.0.1" << std::endl;

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD dwAttached, LPVOID lpvReserved)
{
	if (dwAttached == DLL_PROCESS_ATTACH)
	{
		CreateThread(NULL, 0, &DLLFunction, NULL, 0, NULL);
	}
	return 1;
}
