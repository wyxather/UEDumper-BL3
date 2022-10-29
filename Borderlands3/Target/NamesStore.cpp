#include "NamesStore.hpp"
#include "PatternFinder.hpp"

namespace
{
	class FNameEntry
	{
	public:
		std::int32_t Index; //0x0000
		char pad_0004[4]; //0x0004
		class FNameEntry* HashNext; //0x0008
		char AnsiName[1024]; //0x0010
	}; //Size: 0x0410
	static_assert(sizeof(FNameEntry) == 0x410);

	class TNameEntryArray
	{
	public:
		enum
		{
			ElementsPerChunk = 16 * 1024,
			ChunkTableSize = (2 * 1024 * 1024 + ElementsPerChunk - 1) / ElementsPerChunk
		};

		[[nodiscard]] constexpr auto GetItemPtr(size_t Index) const noexcept
		{
			const auto ChunkIndex = Index / ElementsPerChunk;
			const auto WithinChunkIndex = Index % ElementsPerChunk;
			const auto Chunk = Chunks[ChunkIndex];
			return Chunk + WithinChunkIndex;
		}

		[[nodiscard]] constexpr auto& GetById(size_t Index) const noexcept
		{
			return *GetItemPtr(Index);
		}

		[[nodiscard]] constexpr auto IsValidIndex(size_t Index) const noexcept
		{
			return Index >= 0 && Index < NumElements && GetById(Index);
		}

		FNameEntry* const* Chunks[ChunkTableSize];
		std::int32_t NumElements;
		std::int32_t NumChunks;
	};

	TNameEntryArray* GlobalNames;
}

auto NamesStore::Initialize() noexcept -> bool
{
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x48\x83\xEC\x28\x48\x8B\x05\x00\x00\x00\x00\x48\x85\xC0\x75\x00\xB9\x00\x00\x00\x00\x48\x89\x5C\x24\x20\xE8", "xxxxxxx????xxxx?x????xxxxxx");
	printf("NamesStore:Address = %p\n", (void*)Address);
	GlobalNames = *(TNameEntryArray**)(Address + *(DWORD*)(Address + 0x7) + 0xB);
	printf("NamesStore:GlobalNames = %p\n", (void*)GlobalNames);

	return true;
}

auto NamesStore::GetAddress() noexcept -> void*
{
	return GlobalNames;
}

auto NamesStore::GetNamesNum() const noexcept -> std::size_t
{
	return static_cast<std::size_t>(GlobalNames->NumElements);
}

auto NamesStore::IsValid(std::size_t id) const noexcept -> bool
{
	return GlobalNames->IsValidIndex(id);
}
auto NamesStore::GetById(std::size_t id) const noexcept -> std::string
{
	return GlobalNames->GetById(id)->AnsiName;
}