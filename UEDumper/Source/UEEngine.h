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

	UEPointer Stride = 0;

	struct
	{
		UEPointer Index = 0;
		UEPointer Class = 0;
		UEPointer Name = 0;
		UEPointer Outer = 0;

	} UObject;

	struct
	{
		UEPointer Info = 0;
		UEPointer WideBit = 0;
		UEPointer LenBit = 0;
		UEPointer HeaderSize = 0;

	} FNameEntry;

private:
	bool error;
};