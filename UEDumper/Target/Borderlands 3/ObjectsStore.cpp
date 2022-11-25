#include "ObjectsStore.hpp"
#include "PatternFinder.hpp"

namespace
{
	class FUObjectItem
	{
	public:
		class UObject* Object;
		std::int32_t Flags;
		std::int32_t ClusterRootIndex;
		std::int32_t SerialNumber;
		char pad_0014[4];
	};

	class FChunkedFixedUObjectArray
	{
	public:
		enum
		{
			ElementsPerChunk = 64 * 1024
		};

		constexpr auto GetObjectPtr(std::size_t Index) const noexcept
		{
			const auto ChunkIndex = Index / ElementsPerChunk;
			const auto WithinChunkIndex = Index % ElementsPerChunk;
			const auto Chunk = Objects[ChunkIndex];
			return Chunk + WithinChunkIndex;
		}

		constexpr auto& GetByIndex(std::size_t Index) const noexcept
		{
			return *GetObjectPtr(Index);
		}

		class FUObjectItem** Objects;
		class FUObjectItem* PreAllocatedObjects;
		std::int32_t MaxElements;
		std::int32_t NumElements;
		std::int32_t MaxChunks;
		std::int32_t NumChunks;
	};

	class FUObjectArray
	{
	public:
		std::int32_t ObjFirstGCIndex;
		std::int32_t ObjLastNonGCIndex;
		std::int32_t MaxObjectsNotConsideredByGC;
		bool OpenForDisregardForGC;
		FChunkedFixedUObjectArray ObjObjects;
	};

	FUObjectArray* GlobalObjects;
}

bool ObjectsStore::Initialize()
{
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x48\x8B\x0D\x00\x00\x00\x00\x48\x98\x4C\x8B\x04\xD1\x48\x8D\x0C\x40\x49\x8D\x04\xC8\xEB", "xxx????xxxxxxxxxxxxxxx");
	printf("ObjecStore::Address = %p\n", (void*)Address);
	GlobalObjects = (FUObjectArray*)(Address + *(DWORD*)(Address + 0x3) + 7 - 0x10);
	printf("ObjecStore::GlobalObjects = %p\n", (void*)GlobalObjects);

	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.NumElements;
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects.GetByIndex(id).Object;
}