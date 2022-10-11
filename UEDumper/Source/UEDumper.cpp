#include "UEDumper.h"

UEDumper::UEDumper(const int argc, const char* argv[]) noexcept : error{ Error::SUCCESS }, path{ argv[0] }
{
	std::printf("UEDumper...\n");

	path.remove_filename();

	if (game.emplace(); !game.value()) {
		error = Error::GAME_NOT_FOUND;
		return;
	}

	if (engine.emplace(game->getVersion()); !engine.value()) {
		error = Error::ENGINE_INIT_FAILURE;
		return;
	}
}

UEDumper::~UEDumper() noexcept
{
	engine.reset();
	game.reset();

	std::printf("~UEDumper...\n");
}