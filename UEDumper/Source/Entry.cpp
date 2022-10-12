#include "UEDumper.h"

auto main(const int argc, const char* argv[]) noexcept -> int
{
	if (UEDumper dumper{ argc, argv }; dumper) {
		dumper.dump();
	}
	else {
		switch (dumper.getError()) {
		case UEDumper::Error::SUCCESS: break;
		case UEDumper::Error::GAME_NOT_FOUND: std::printf("Game not found.\n"); break;
		case UEDumper::Error::ENGINE_INIT_FAILURE: std::printf("Engine init failure.\n"); break;
		}
	}
	return 0;
}