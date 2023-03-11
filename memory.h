#pragma once

#include <vector>
#include <windows.h>

DWORD GetProcId(const wchar_t* procName);

uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

uintptr_t FindDMAAddress(HANDLE hProc, uintptr_t ptr, std::vector<unsigned int> offsets);

namespace Memory {
	template <typename T>
	constexpr T Read(HANDLE hProc, const uintptr_t& address)
	{
		T value = {};
		::ReadProcessMemory(hProc, reinterpret_cast<const void*>(address), &value, sizeof(T), NULL);
		return value;
	}

	template <typename T>
	constexpr void Write(HANDLE hProc, const uintptr_t& address, const T& value)
	{
		::WriteProcessMemory(hProc, reinterpret_cast<void*>(address), &value, sizeof(T), NULL);
	}
}