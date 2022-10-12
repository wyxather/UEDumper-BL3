#include "UEEngine.h"
#include "UEGame.h"

namespace
{
	template <typename T = std::uintptr_t>
	constexpr auto findPattern(const std::vector<std::byte>& image, std::string_view pattern) noexcept
	{
		constexpr auto generateBadCharTable = [](std::string_view pattern) noexcept {

			std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;

			auto lastWildcard = pattern.rfind('?');
			if (lastWildcard == std::string_view::npos)
				lastWildcard = 0;

			const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);
			table.fill(defaultShift);

			for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
				table[std::clamp<std::uint8_t>(static_cast<std::uint8_t>(pattern[i]), 0, 255)] = pattern.length() - 1 - i;

			return table;
		};

		if (!image.empty()) {

			const auto lastIdx = pattern.length() - 1;
			const auto badCharTable = generateBadCharTable(pattern);

			auto start = reinterpret_cast<const char*>(image.data());
			const auto end = start + image.size() - pattern.length();

			while (start <= end) {

				int i = static_cast<decltype(i)>(lastIdx);
				while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i]))
					--i;

				if (i < 0)
					return reinterpret_cast<T>(start);

				start += badCharTable[std::clamp<std::uint8_t>(static_cast<std::uint8_t>(start[lastIdx]), 0, 255)];
			}
		}

		return T{};
	}

	template <typename T = std::uintptr_t>
	constexpr auto relativeToAbsolute(std::uintptr_t address) noexcept
	{
		return (T)(address + sizeof(std::uint32_t) + *reinterpret_cast<std::uint32_t*>(address));
	}

	class UENamePool
	{
	public:
	};

	class UENamePool_4_27_2_0 : public UENamePool
	{
	public:
		std::byte lock[8]; //0x0000
		std::uint32_t currentBlock; //0x0008
		std::uint32_t currentByteCursor; //0x000C
		struct { std::byte* data; } *blocks[8192]; //0x0010
	}; //Size: 0x10010
	static_assert(sizeof(UENamePool_4_27_2_0) == 0x10010);
}

UEEngine::UEEngine(const UEGame& game) noexcept : error{ false }
{
	std::printf("UEEngine...\n");

	const auto version = game.getVersion();

	if (4 <= version[0]) {
		if (27 <= version[1]) {
			if (2 <= version[2]) {

				// Game List:
				// 1. Prometheus / Omega Strike (Steam)

				UObject.Index = 0xC;
				UObject.Class = 0x10;
				UObject.Name = 0x18;
				UObject.Outer = 0x20;

				const auto namepool = "\x48\x8D\x0D????\xE8????\xC6\x05????\x01\x0F\x10\x03\x4C\x8D\x44\x24\x20\x48\x8B\xC8"; // 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? 01 0F 10 03 4C 8D 44 24 20 48 8B C8
				const auto gobjects = "\x48\x8B\x05????\x48\x8B\x0C\xC8\x48\x8D\x04\xD1\xEB"; // 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB
				//both of this addresses is different from the game addresses since it's an image copy,
				//but it's still points to the same address.
				const auto namepoolFakeAddy = relativeToAbsolute<std::uintptr_t*>(findPattern(game.getImage(), namepool) + 3);
				const auto gobjectsFakeAddy = relativeToAbsolute<std::uintptr_t*>(findPattern(game.getImage(), gobjects) + 3);
			}
		}
	}
}

UEEngine::~UEEngine() noexcept
{
	std::printf("~UEEngine...\n");
}