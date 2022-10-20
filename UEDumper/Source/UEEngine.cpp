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

	class UENamePool_4_20_3_0 : public UENamePool
	{
	public:
		struct Entry {
			std::uint64_t index; //0x0000
			Entry* next; //0x0008
			std::uint8_t name[1024]; //0x0010
		}; //Size: 0x0410
		static_assert(sizeof(Entry) == 0x410);

		struct Block {
			Entry* entry[16384]; //0x0000
		}; //Size: 0x20000
		static_assert(sizeof(Block) == 0x20000);

		Block* blocks[35]; //0x0000
	}; //Size: 0x0118
	static_assert(sizeof(UENamePool_4_20_3_0) == 0x118);
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

				FNameEntry.Info = 0;
				FNameEntry.WideBit = 0;
				FNameEntry.LenBit = 6;
				FNameEntry.HeaderSize = 2;

				const auto namepoolPattern = "\x48\x8D\x0D????\xE8????\xC6\x05????\x01\x0F\x10\x03\x4C\x8D\x44\x24\x20\x48\x8B\xC8"; // 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? 01 0F 10 03 4C 8D 44 24 20 48 8B C8
				const auto gobjectsPattern = "\x48\x8B\x05????\x48\x8B\x0C\xC8\x48\x8D\x04\xD1\xEB"; // 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB
	
				const auto namepoolPtr = relativeToAbsolute<UENamePool_4_27_2_0*>(findPattern(game.getImage(), namepoolPattern) + 3);
				const auto gobjectsFakeAddy = relativeToAbsolute<std::uintptr_t*>(findPattern(game.getImage(), gobjectsPattern) + 3);

				for (std::uint32_t i = 0; i < namepoolPtr->currentBlock; i++) {

					for (std::uint32_t j = 0; j < namepoolPtr->currentByteCursor;) {

						std::size_t info{};
						if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(namepoolPtr->blocks[i]) + j), &info, FNameEntry.HeaderSize))
							continue;

						auto length = info >> FNameEntry.LenBit;
						const auto wide = (info >> FNameEntry.WideBit) & 1;

						j += FNameEntry.HeaderSize;

						std::unique_ptr<std::byte[]> name = std::make_unique<decltype(name)::element_type[]>(length + 1);
						if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(namepoolPtr->blocks[i]) + j), name.get(), length))
							continue;

						j += static_cast<decltype(j)>(length);
						if (length % 2)
							j++;

						std::printf("%s[%zd]\n", reinterpret_cast<const char*>(name.get()), length);
					}
				}
			}
		}
		else if (20 <= version[1]) {
			if (3 <= version[2]) {

				// Game List
				// 1. Borderlands 3 (Epic Games)

				const auto namepoolPattern = "\x48\x83\xEC\x28\x48\x8B\x05????\x48\x85\xC0\x75?\xB9????\x48\x89\x5C\x24\x20\xE8"; // 48 83 EC 28 48 8B 05 ? ? ? ? 48 85 C0 75 ? B9 ? ? ? ? 48 89 5C 24 20 E8
				const auto gobjectsPattern = "\x48\x8B\x0D????\x48\x98\x4C\x8B\x04\xD1\x48\x8D\x0C\x40\x49\x8D\x04\xC8\xEB"; // 48 8B 0D ? ? ? ? 48 98 4C 8B 04 D1 48 8D 0C 40 49 8D 04 C8 EB
			
				std::unique_ptr<UENamePool_4_20_3_0> namePool = std::make_unique<decltype(namePool)::element_type>();
				if (!game.read(*relativeToAbsolute<UENamePool_4_20_3_0**>(findPattern(game.getImage(), namepoolPattern) + 7), namePool.get(), sizeof(decltype(namePool)::element_type))) {
					error = true;
					return;
				}

				const auto gobjectsFakeAddy = relativeToAbsolute<std::uintptr_t*>(findPattern(game.getImage(), gobjectsPattern) + 3);

				for (std::size_t i = 0; i < (sizeof(namePool->blocks) / sizeof(namePool->blocks[0])); i++) {

					std::unique_ptr<UENamePool_4_20_3_0::Block> block = std::make_unique<decltype(block)::element_type>();
					if (!game.read(namePool->blocks[i], block.get(), sizeof(decltype(block)::element_type)))
						continue;

					for (std::size_t j = 0; j < (sizeof(block->entry) / sizeof(block->entry[0])); j++) {

						std::unique_ptr<UENamePool_4_20_3_0::Entry> entry = std::make_unique<decltype(entry)::element_type>();
						if (!game.read(block->entry[j], entry.get(), sizeof(decltype(entry)::element_type)))
							continue;

						std::printf("[%p][%08X] %s\n", reinterpret_cast<void*>(block->entry[j]), static_cast<std::uint32_t>(entry->index / 2), entry->name);
					}
				}
			}
		}
	}
}

UEEngine::~UEEngine() noexcept
{
	std::printf("~UEEngine...\n");
}