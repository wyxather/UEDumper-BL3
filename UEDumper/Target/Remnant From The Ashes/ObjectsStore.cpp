#include "ObjectsStore.hpp"
#include "PatternFinder.hpp"

namespace
{
	class FUObjectItem
	{
	public:
		class UObject* Object; //0x0000
		int32_t Flags; //0x0008
		int32_t ClusterRootIndex; //0x000C
		int32_t SerialNumber; //0x0010
		char pad_0014[4]; //0x0014
	}; //Size: 0x0018
	static_assert(sizeof(FUObjectItem) == 0x18);

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

		class FUObjectItem** Objects; //0x0000
		class FUObjectItem* PreAllocatedObjects; //0x0008
		int32_t MaxElements; //0x0010
		int32_t NumElements; //0x0014
		int32_t MaxChunks; //0x0018
		int32_t NumChunks; //0x001C
	}; //Size: 0x0020
	static_assert(sizeof(FChunkedFixedUObjectArray) == 0x20);

	class FUObjectArray
	{
	public:
		int32_t ObjFirstGCIndex; //0x0000
		int32_t ObjLastNonGCIndex; //0x0004
		int32_t MaxObjectsNotConsideredByGC; //0x0008
		bool OpenForDisregardForGC; //0x000C
		char pad_000D[3]; //0x000D
		class FChunkedFixedUObjectArray ObjObjects; //0x0010
	}; //Size: 0x0030
	static_assert(sizeof(FUObjectArray) == 0x30);

	FUObjectArray* GlobalObjects;
}

bool ObjectsStore::Initialize()
{
	auto Address = FindPattern(GetModuleHandleW(0), (unsigned char*)"\x44\x8B\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x00\x00\x00\x00\x00\x48\x89\x71\x10", "xx???xxx?????????xxxx");
	printf("ObjecStore::Address = %p\n", (void*)Address);
	GlobalObjects = (FUObjectArray*)(Address + *(DWORD*)(Address + 0x8) + 7);
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