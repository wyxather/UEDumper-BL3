#pragma once

#include "UEEngine.h"
#include <filesystem>

class UEGame
{
public:
	explicit UEGame() noexcept;
	virtual ~UEGame() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }
	[[nodiscard]] constexpr auto&& getVersion() const noexcept { return version; }

private:
	bool error;
	DWORD id;
	HWND window;
	HANDLE handle;
	DWORD version[4];
	std::filesystem::path path;
};