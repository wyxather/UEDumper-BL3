#pragma once

#include "UEGame.h"

class UEDumper
{
public:
	enum class Error
	{
		SUCCESS,
		GAME_NOT_FOUND
	};

	explicit UEDumper(const int argc, const char* argv[]) noexcept;
	virtual ~UEDumper() noexcept;

	[[nodiscard]] constexpr operator bool() const noexcept { return error == Error::SUCCESS; }

private:
	Error error;
	std::filesystem::path path;

	std::optional<UEGame> game;
};