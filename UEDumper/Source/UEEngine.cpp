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

	class UENamePool {};
	class UEObjectArray {};

	class UENamePool_4_27_2_0 : public UENamePool
	{
	public:
		struct Block
		{
			std::byte* data;
		};

		explicit UENamePool_4_27_2_0(const UEGame& game, std::string_view pattern, const std::ptrdiff_t offset) noexcept
		{
			std::memset(this, 0, sizeof(*this));
			const auto names = relativeToAbsolute<decltype(this)>(findPattern(game.getImage(), pattern) + offset);
			currentBlock = names->currentBlock;
			currentByteCursor = names->currentByteCursor;
			std::memcpy(blocks, names->blocks, names->currentBlock * sizeof(std::uintptr_t));
		}

		auto print(const UEGame& game, const UEEngine& engine) const noexcept
		{
			for (std::uint32_t block = 0; block < currentBlock; block++)
				for (std::uint32_t cursor = 0; cursor < currentByteCursor;)
					if (!print(game, engine, block, cursor))
						break;
		}

		[[nodiscard]] auto print(const UEGame& game, const UEEngine& engine, const std::uint32_t block, std::uint32_t& cursor) const noexcept -> bool
		{
			std::size_t info{};
			if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(blocks[block]) + cursor), &info, engine.FNameEntry.HeaderSize))
				return false;

			const auto length = info >> engine.FNameEntry.LenBit;
			const auto wide = (info >> engine.FNameEntry.WideBit) & 1;

			cursor += engine.FNameEntry.HeaderSize;

			std::unique_ptr<std::byte[]> name = std::make_unique<decltype(name)::element_type[]>(length + 1);
			if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(blocks[block]) + cursor), name.get(), length))
				return false;

			cursor += static_cast<std::remove_reference_t<decltype(cursor)>>(length);
			if (length % 2)
				cursor++;

			const auto offset = cursor / engine.Stride;
			const auto index = (block << 16 | offset);

			if (wide)
				std::printf("[%08X] %ls\n", index, reinterpret_cast<const wchar_t*>(name.get()));
			else
				std::printf("[%08X] %s\n", index, reinterpret_cast<const char*>(name.get()));

			return true;
		}

		[[nodiscard]] auto print(const UEGame& game, const UEEngine& engine, const std::uint32_t id) const noexcept
		{
			const auto offset = id & 65535;
			auto cursor = offset * engine.Stride;
			return print(game, engine, id >> 16, cursor);
		}

		[[nodiscard]] auto get(const UEGame& game, const UEEngine& engine, const std::uint32_t block, std::uint32_t& cursor) const noexcept -> std::unique_ptr<std::byte[]>
		{
			std::unique_ptr<std::byte[]> name;

			std::size_t info{};
			if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(blocks[block]) + cursor), &info, engine.FNameEntry.HeaderSize))
				return name;

			const auto length = info >> engine.FNameEntry.LenBit;
			const auto wide = (info >> engine.FNameEntry.WideBit) & 1;

			cursor += engine.FNameEntry.HeaderSize;

			name = std::make_unique<decltype(name)::element_type[]>(length + 1);
			if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(blocks[block]) + cursor), name.get(), length))
				return name;

			cursor += static_cast<std::remove_reference_t<decltype(cursor)>>(length);
			if (length % 2)
				cursor++;

			return name;
		}

		[[nodiscard]] auto get(const UEGame& game, const UEEngine& engine, const std::uint32_t id) const noexcept
		{
			const auto offset = id & 65535;
			auto cursor = offset * engine.Stride;
			return get(game, engine, id >> 16, cursor);
		}

		std::byte lock[8]; //0x0000
		std::uint32_t currentBlock; //0x0008
		std::uint32_t currentByteCursor; //0x000C
		Block *blocks[8192]; //0x0010

	}; //Size: 0x10010
	static_assert(sizeof(UENamePool_4_27_2_0) == 0x10010);

	class UEObjectArray_4_27_2_0 : public UEObjectArray
	{
	public:
		struct UEClass {};

		struct UObject
		{
			void* __vftable; //0x0000
			std::uint32_t objectFlags; //0x0008
			std::uint32_t internalIndex; //0x000C
			UEClass* classPrivate; //0x0010
			std::uint32_t namePrivate; //0x0018
			char pad_001C[4]; //0x001C
			UObject* Outer; //0x0020

		}; //Size: 0x0028
		static_assert(sizeof(UObject) == 0x28);

		struct FUObjectItem
		{
			UObject* object; //0x0000
			std::int32_t flags; //0x0008
			std::int32_t clusterRootIndex; //0x000C
			std::int32_t serialNumber; //0x0010
			char pad_0014[4]; //0x0014

		}; //Size: 0x0018
		static_assert(sizeof(FUObjectItem) == 0x18);

		FUObjectItem(*objects)[65536]; //0x0000
		char(*preAllocatedObjects)[8]; //0x0008
		std::int32_t maxElements; //0x0010
		std::int32_t numElements; //0x0014
		std::int32_t maxChunks; //0x0018
		std::int32_t numChunks; //0x001C

		explicit UEObjectArray_4_27_2_0(const UEGame& game, std::string_view pattern, const std::ptrdiff_t offset) noexcept
		{
			std::memset(this, 0, sizeof(*this));
			const auto objects = *relativeToAbsolute<decltype(this)*>(findPattern(game.getImage(), pattern) + offset);
			static_cast<void>(game.read(objects, this, sizeof(*this)));
		}

		auto print(const UEGame& game, const UEEngine& engine, const UENamePool_4_27_2_0& names) const noexcept
		{
			for (std::size_t i = 0; i < 65536; i++) {

				std::unique_ptr<FUObjectItem> item = std::make_unique<decltype(item)::element_type>();
				if (!game.read(reinterpret_cast<void*>(reinterpret_cast<std::uintptr_t>(objects) + sizeof(decltype(item)::element_type) * i), item.get(), sizeof(decltype(item)::element_type)))
					break;

				if (!item->object)
					break;

				std::unique_ptr<UObject> object = std::make_unique<decltype(object)::element_type>();
				if (!game.read(item->object, object.get(), sizeof(decltype(object)::element_type)))
					continue;

				std::printf("[%08X] %p %s\n", object->internalIndex, item->object, reinterpret_cast<const char*>(names.get(game, engine, object->namePrivate).get()));
			}
		}

	}; //Size: 0x0020
	static_assert(sizeof(UEObjectArray_4_27_2_0) == 0x20);

	class UENamePool_4_20_3_0 : public UENamePool
	{
	public:
		struct Entry
		{
			std::uint64_t index; //0x0000
			Entry* next; //0x0008
			std::uint8_t name[1024]; //0x0010

		}; //Size: 0x0410
		static_assert(sizeof(Entry) == 0x410);

		struct Block
		{
			Entry* entry[16384]; //0x0000

		}; //Size: 0x20000
		static_assert(sizeof(Block) == 0x20000);

		explicit UENamePool_4_20_3_0(const UEGame& game, std::string_view pattern, const std::ptrdiff_t offset) noexcept
		{
			std::memset(this, 0, sizeof(*this));
			static_cast<void>(game.read(*relativeToAbsolute<decltype(this)*>(findPattern(game.getImage(), pattern) + offset), this, sizeof(*this)));
		}

		auto print(const UEGame& game) const noexcept
		{
			for (std::size_t i = 0; i < (sizeof(blocks) / sizeof(blocks[0])); i++) {

				std::unique_ptr<Block> block = std::make_unique<decltype(block)::element_type>();
				if (!game.read(blocks[i], block.get(), sizeof(decltype(block)::element_type)))
					continue;

				for (std::size_t j = 0; j < (sizeof(block->entry) / sizeof(block->entry[0])); j++) {

					std::unique_ptr<Entry> entry = std::make_unique<decltype(entry)::element_type>();
					if (!game.read(block->entry[j], entry.get(), sizeof(decltype(entry)::element_type)))
						continue;

					std::printf("[%p][%08X] %s\n", reinterpret_cast<void*>(block->entry[j]), static_cast<std::uint32_t>(entry->index / 2), entry->name);
				}
			}
		}

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

				Stride = 2;

				UObject.Index = 0xC;
				UObject.Class = 0x10;
				UObject.Name = 0x18;
				UObject.Outer = 0x20;

				FNameEntry.Info = 0;
				FNameEntry.WideBit = 0;
				FNameEntry.LenBit = 6;
				FNameEntry.HeaderSize = 2;

				std::unique_ptr<UENamePool_4_27_2_0> names = std::make_unique<decltype(names)::element_type>(game, "\x48\x8D\x0D????\xE8????\xC6\x05????\x01\x0F\x10\x03\x4C\x8D\x44\x24\x20\x48\x8B\xC8", 3); // 48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? 01 0F 10 03 4C 8D 44 24 20 48 8B C8
				names->print(game, *this);

				std::unique_ptr<UEObjectArray_4_27_2_0> objects = std::make_unique<decltype(objects)::element_type>(game, "\x48\x8B\x05????\x48\x8B\x0C\xC8\x48\x8D\x04\xD1\xEB", 3); // 48 8B 05 ? ? ? ? 48 8B 0C C8 48 8D 04 D1 EB
				objects->print(game, *this, *names.get());
			}
		}
		else if (20 <= version[1]) {
			if (3 <= version[2]) {

				// Game List
				// 1. Borderlands 3 (Epic Games)

				const auto gobjectsPattern = "\x48\x8B\x0D????\x48\x98\x4C\x8B\x04\xD1\x48\x8D\x0C\x40\x49\x8D\x04\xC8\xEB"; // 48 8B 0D ? ? ? ? 48 98 4C 8B 04 D1 48 8D 0C 40 49 8D 04 C8 EB
				const auto gobjectsFakeAddy = relativeToAbsolute<std::uintptr_t*>(findPattern(game.getImage(), gobjectsPattern) + 3);

				std::unique_ptr<UENamePool_4_20_3_0> names = std::make_unique<decltype(names)::element_type>(game, "\x48\x83\xEC\x28\x48\x8B\x05????\x48\x85\xC0\x75?\xB9????\x48\x89\x5C\x24\x20\xE8", 7); // 48 83 EC 28 48 8B 05 ? ? ? ? 48 85 C0 75 ? B9 ? ? ? ? 48 89 5C 24 20 E8
				names->print(game);
			}
		}
	}
}

UEEngine::~UEEngine() noexcept
{
	std::printf("~UEEngine...\n");
}