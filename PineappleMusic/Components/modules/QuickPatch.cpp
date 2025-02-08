// ==========================================================
// Project: PineappleMusic
// 
// Component: pineapplemusic.dll
//
// Purpose: Apply quick patches to Apple Music
//          
// Initial author: meik97
//
// Started: 19.01.2025
// ==========================================================

#include "..\include\Include.hpp"
#include "QuickPatch.hpp"
#include "..\utils\hooking.hpp"

namespace Modules
{
	void QuickPatch::branding()
	{
		//add branding
	}

	DWORD64 print_f_back = AMPLibraryAgentBase + 0x0200A51;
	DWORD64 locret_140200D20 = AMPLibraryAgentBase + 0x0200D20;
	void __attribute__((naked)) QuickPatch::print_f_stub(int a1, int a2, __int128* fmt, ...)
	{
		__asm
		{
			test    ecx, ecx
			js      locret_140200D20
			mov	[rsp + 24], r8
			mov	rax, print_f_back
			push	rax
			ret
		}
	}

	void QuickPatch::print_f(int a1, int a2, __int128 fmt, ...)
	{
		va_list args;
		va_start(args, (const char*)fmt);

		//remove annoying idle trigger from console
		if (strstr((const char*)fmt, "idle> trigger (%s)") == nullptr)
		{
			vprintf((const char*)fmt, args);
			printf("\n");
		}

		va_end(args);
	}

	typedef float(*gain_t)();
	extern gain_t getGain;
	gain_t getGain = (gain_t)(AMPLibraryAgentBase + 0x0A1A42C);

	float QuickPatch::calculateVolume(double slider)
	{
		if (slider <= 0.0)
			return 0.0;
		if (slider >= 1.0)
			return 1; 
		return pow(10, getGain() * (1 - slider) * 0.05);
	}

	unsigned int* _r8;
	void QuickPatch::changeStepFrequency()
	{
		uint8_t stepFrequency_value[] = { 0x7B, 0x14, 0xAE, 0x47, 0xE1, 0x7A, 0x84, 0x3F, 0x49, 0x01 };

		//check for step frequency
		if ((*_r8 == 2684354560))
		{			
			memcpy(_r8, stepFrequency_value, sizeof(stepFrequency_value));		
		}
	}
	 
	DWORD64 fixVolumeStepFrequency_back = MicrosoftUIXamlBase + 0xFF10;
	DWORD64 loc_1800102AE = MicrosoftUIXamlBase + 0xFF06;
	void __attribute__((naked)) QuickPatch::fixVolumeStepFrequency(__int64 a1, int a2, int a3)
	{
		__asm
		{
			mov	_r8, r8
			call	changeStepFrequency

			mov     eax, [r8 + 4]
			mov	[rsi + 4], eax
			mov     eax, [rsi + 8]
			and	eax, 4294967241
			or	eax, 49h
			
			jmp     loc_1800102AE
			mov     eax, [r8]
			xor     edi, edi
			mov	rax, fixVolumeStepFrequency_back
			
			push	rax
			ret

		}
	}

	QuickPatch::QuickPatch(std::wstring module)
	{
		if (module.compare(L"AMPLibraryAgent.exe") == 0)
		{
			//AMP library agent debug log
			Utils::Hook::InstallHook((void*)(AMPLibraryAgentBase + 0x0200A44), print_f);

			//we dont need this
			Utils::Hook::InstallHook((void*)(AMPLibraryAgentBase + 0x0A1A614), calculateVolume); 
		}

		if (module.compare(L"AppleMusic.exe") == 0)
		{
			//Volume step frequency
			Utils::Hook::InstallHook((void*)(MicrosoftUIXamlBase + 0xFEF6), fixVolumeStepFrequency);

			//AMP Services debug log
			Utils::Hook::InstallHook((void*)(AMPServicesBase + 0x011D18C), print_f);
		}
	}
}
