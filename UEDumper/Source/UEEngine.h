#pragma once

#include <Windows.h>
#include <cstdint>

class UEEngine
{
public:
	explicit UEEngine(const DWORD version[4]) noexcept;
	virtual ~UEEngine() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }

	struct {
		std::int16_t Index = 0;
		std::int16_t Class = 0;
		std::int16_t Name = 0;
		std::int16_t Outer = 0;
	} UObject;

private:
	bool error;
};