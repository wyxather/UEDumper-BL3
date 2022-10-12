#include "UEDumper.h"

auto main(const int argc, const char* argv[]) noexcept -> int
{
	if (UEDumper dumper{ argc, argv }; dumper) {
		dumper.dump();
	}
	return 0;
}