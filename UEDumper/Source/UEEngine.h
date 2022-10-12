#pragma once

#include "UEPointer.h"
#include <Windows.h>
#include <filesystem>
#include <array>
#include <algorithm>

class UEGame;

class UEEngine
{
public:
	UEEngine(const UEGame& game) noexcept;
	~UEEngine() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }

	struct
	{
		UEPointer Index = 0;
		UEPointer Class = 0;
		UEPointer Name = 0;
		UEPointer Outer = 0;

	} UObject;

private:
	bool error;
};