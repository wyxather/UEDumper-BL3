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

	UEPointer id;
	UEPointer window;
	UEPointer handle;
	UEPointer version[4];

	std::filesystem::path path;
};