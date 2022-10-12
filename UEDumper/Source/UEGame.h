#pragma once

#include "UEEngine.h"

class UEGame
{
public:
	UEGame() noexcept;
	~UEGame() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return !error; }
	[[nodiscard]] constexpr auto&& getPath() const noexcept { return path; }
	[[nodiscard]] constexpr auto&& getImage() const noexcept { return image; }
	[[nodiscard]] constexpr auto&& getVersion() const noexcept { return version; }

	[[nodiscard]] auto read(void* const address, void* const buffer, const SIZE_T size) const noexcept -> bool;

	template <typename T>
	[[nodiscard]] constexpr auto read(void* const address) const noexcept
	{
		T buffer{};
		static_cast<void>(read(address, &buffer, sizeof(T)));
		return buffer;
	}

private:
	bool error;

	UEPointer id;
	UEPointer window;
	UEPointer handle;
	UEPointer version[4];

	std::filesystem::path path;
	std::vector<std::byte> image;
};