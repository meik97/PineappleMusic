#include "..\include\include.hpp"
#include "hooking.hpp"

namespace Utils
{

    void Hook::InstallHook(void* hooked_function, void* target_function)
    {
        DWORD oldProtect;
        const uint64_t jmp_dest = (uint64_t)target_function;
        uint8_t jmp_instructions[] = { 0x49, 0xBA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0xFF, 0xE2 }; //mov r10, xxxxxxxxx ; jmp r10 --> stubs need to be at least 13 bytes

        VirtualProtect(hooked_function, 1024, PAGE_EXECUTE_READWRITE, &oldProtect);
        memcpy(&jmp_instructions[2], &jmp_dest, sizeof(jmp_dest));
        memcpy(hooked_function, jmp_instructions, sizeof(jmp_instructions));
        VirtualProtect(hooked_function, 1024, oldProtect, &oldProtect);
    }

}