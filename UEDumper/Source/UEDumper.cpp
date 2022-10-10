#include "UEDumper.h"

UEDumper::UEDumper(const int argc, const char* argv[]) noexcept : error{ Error::SUCCESS }, path{ argv[0] }
{
	std::printf("UEDumper...\n");

	path.remove_filename();

	if (game.emplace(); !game.value()) {
		error = Error::GAME_NOT_FOUND;
		return;
	}
}

UEDumper::~UEDumper() noexcept
{
	game.reset();

	std::printf("~UEDumper...\n");
}