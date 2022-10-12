#pragma once

#include "UEPointer.h"
#include <Windows.h>
#include <filesystem>

class UEEngine
{
public:
	explicit UEEngine(const UEPointer version[4]) noexcept;
	virtual ~UEEngine() noexcept;

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