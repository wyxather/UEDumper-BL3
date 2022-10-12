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

auto UEDumper::dump() noexcept -> void
{
	std::printf("Game Informations\n");
	std::printf("Name: %ls\n", game->getPath().filename().wstring().c_str());
	std::printf("Path: %ls\n", game->getPath().wstring().c_str());
	std::printf("Version: %d.%d.%d.%d\n", static_cast<int>(game->getVersion()[0]), static_cast<int>(game->getVersion()[1]), static_cast<int>(game->getVersion()[2]), static_cast<int>(game->getVersion()[3]));
}