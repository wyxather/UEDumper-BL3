#pragma once

#include "UEGame.h"

class UEDumper
{
public:
	enum class Error
	{
		SUCCESS,
		GAME_NOT_FOUND,
		ENGINE_INIT_FAILURE,
	};

	UEDumper(const int argc, const char* argv[]) noexcept;
	~UEDumper() noexcept;

	auto dump() const noexcept -> void;

	[[nodiscard]] constexpr operator bool() const noexcept { return error == Error::SUCCESS; }
	[[nodiscard]] constexpr auto getError() const noexcept { return error; }

private:
	Error error;
	std::filesystem::path path;

	std::optional<const UEGame> game;
	std::optional<const UEEngine> engine;
};