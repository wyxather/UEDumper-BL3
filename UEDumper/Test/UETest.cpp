#include <Windows.h>
#include <Psapi.h>
#include <cassert>

#include "PatternFinder.h"

static const char data[] = "\x44\33\x22\x11\x00\55";

auto main() -> int
{
	MODULEINFO info;
	if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info)))
		return 1;

	const auto pattern = PatternFinder{
		{ reinterpret_cast<const std::byte*>(info.lpBaseOfDll), info.SizeOfImage }
	}(PatternWrapper{
		"\x44\33\x22\x11\x00\55"
		}());

	assert((void*)pattern == (void*)data);
	printf("%p | %p", pattern, data);

	return 0;
}