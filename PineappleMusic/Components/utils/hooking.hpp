#pragma once

namespace Utils
{
	class Hook
	{
	public:
		static void InstallHook(void* hooked_function, void* target_function);
	};
}
