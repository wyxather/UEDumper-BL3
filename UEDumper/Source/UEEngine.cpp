#include "UEEngine.h"

UEEngine::UEEngine(const UEPointer version[4]) noexcept : error{ false }
{
	std::printf("UEEngine...\n");

	if (4 <= version[0]) {
		if (27 <= version[1]) {
			if (2 <= version[2]) {

				// Game List:
				// 1. Prometheus / Omega Strike (Steam)

				UObject.Index = 0xC;
				UObject.Class = 0x10;
				UObject.Name = 0x18;
				UObject.Outer = 0x20;
			}
		}
	}
}

UEEngine::~UEEngine() noexcept
{
	std::printf("~UEEngine...\n");
}