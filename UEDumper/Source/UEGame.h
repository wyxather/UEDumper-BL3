#pragma once

#include "UEEngine.h"

class UEGame
{
public:
	explicit UEGame() noexcept;
	virtual ~UEGame() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }
	[[nodiscard]] constexpr auto&& getPath() const noexcept { return path; }
	[[nodiscard]] constexpr auto&& getVersion() const noexcept { return version; }

private:
	bool error;

	UEPointer id;
	UEPointer window;
	UEPointer handle;
	UEPointer version[4];

	std::filesystem::path path;
	std::unique_ptr<std::byte[]> image;
};