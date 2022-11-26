#include <Windows.h>
#include <Psapi.h>
#include <cassert>

#include "../Engine/PatternFinder.hpp"

static const char data[] = "\x44\x33\x22\x11\x00\x55";

auto main() -> int
{
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info)))
		return 1;

	const auto pattern = PatternFinder{
		{ reinterpret_cast<const std::byte*>(info.lpBaseOfDll), info.SizeOfImage }
	}(PatternView<"44 33 22 11 00 55">::value);

	assert((void*)pattern == (void*)data);
	printf("%p | %p", pattern, data);

	return 0;
}