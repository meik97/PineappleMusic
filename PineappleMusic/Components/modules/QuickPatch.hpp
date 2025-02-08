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

namespace Modules
{
	class QuickPatch
	{
	public:
		//QuickPatch();
		QuickPatch(std::wstring module);
		static void branding();
		static void print_f(int a1, int a2, __int128 fmt, ...);
		static void __attribute__((naked)) print_f_stub(int a1, int a2, __int128* fmt, ...);
		static void fixVolumeStepFrequency(__int64 a1, int a2, int a3);
		static void changeStepFrequency();
		static float calculateVolume(double slider);
	};
}