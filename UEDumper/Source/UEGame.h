#pragma once

#include <filesystem>
#include <Windows.h>

class UEGame
{
public:
	explicit UEGame() noexcept;
	virtual ~UEGame() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }

private:
	bool error;
	DWORD id;
	HWND window;
	HANDLE handle;
	DWORD version[4];
	std::filesystem::path path;
};