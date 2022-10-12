#include "UEEngine.h"

UEEngine::UEEngine(const UEPointer version[4]) noexcept : error{ false }
{
	if (4 <= version[0]) {
		UObject.Index = 0xC;
		UObject.Class = 0x10;
		UObject.Name = 0x18;
		UObject.Outer = 0x20;
	}
}

UEEngine::~UEEngine() noexcept
{
}