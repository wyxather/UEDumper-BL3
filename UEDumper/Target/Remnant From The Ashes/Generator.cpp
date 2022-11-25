#include "NamesStore.hpp"

class Generator : public IGenerator
{
public:
	bool Initialize(void* module) override
	{
		alignasClasses = {
			{ "ScriptStruct CoreUObject.Plane", 16 },
			{ "ScriptStruct CoreUObject.Quat", 16 },
			{ "ScriptStruct CoreUObject.Transform", 16 },
			{ "ScriptStruct CoreUObject.Vector4", 16 },
			{ "ScriptStruct Engine.RootMotionSourceGroup", 8 }
		};

		virtualFunctionPattern["Class CoreUObject.Object"] = {
			{ "\x40\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\xF0", "xxxxxxxxxxxxxxxx", 0x400, R"(	constexpr auto ProcessEvent(class UFunction* function, void* parms) noexcept
	{
		return GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, %d)(this, function, parms);
	})" }
		};
		virtualFunctionPattern["Class CoreUObject.Class"] = {
			{ "\x4C\x8B\xDC\x57\x48\x81\xEC\xE0\x01", "xxxxxxxxx", 0x200, R"(	constexpr auto CreateDefaultObject() noexcept
	{
		return GetVFunction<UObject*(*)(UClass*)>(this, %d)(this);
	})" }
		};

		predefinedMembers["Class CoreUObject.Object"] = {
			{ "void*", "VTable" },
			{ "int32_t", "ObjectFlags" },
			{ "int32_t", "InternalIndex" },
			{ "class UClass*", "ClassPrivate" },
			{ "class FName", "NamePrivate" },
			{ "class UObject*", "OuterPrivate" }
		};
		predefinedStaticMembers["Class CoreUObject.Object"] = {
			{ "FUObjectArray*", "GObjects" }
		};
		predefinedMembers["Class CoreUObject.Field"] = {
			{ "class UField*", "Next" }
		};
		predefinedMembers["Class CoreUObject.Struct"] = {
			{ "char", "pad_0030[16]"},
			{ "class UStruct*", "SuperStruct" },
			{ "class UField*", "Children" },
			{ "int32_t", "PropertySize" },
			{ "int32_t", "MinAlignment" },
			{ "char", "pad_0058[64]"}
		};
		predefinedMembers["Class CoreUObject.Function"] = {
			{ "int32_t", "FunctionFlags" },
			{ "int8_t", "NumParms" },
			{ "char", "pad_009D[1]" },
			{ "int16_t", "ParmsSize" },
			{ "uint16_t", "ReturnValueOffset" },
			{ "int16_t", "RPCId" },
			{ "int16_t", "RPCResponseId" },
			{ "char", "pad_00A6[2]" },
			{ "int64_t", "FirstPropertyToInit" },
			{ "int64_t", "EventGraphFunction" },
			{ "int64_t", "EventGraphCallOffset" },
			{ "char", "pad_00BC[4]" },
			{ "void*", "Func" }
		};

		predefinedMethods["ScriptStruct CoreUObject.Vector"] = {
			PredefinedMethod::Inline(R"(	constexpr FVector(float x = 0.f, float y = 0.f, float z = 0.f) noexcept : X{ x }, Y{ y }, Z{ z } {})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator-(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X - b.X, a.Y - b.Y, a.Z - b.Z };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator+(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X + b.X, a.Y + b.Y, a.Z + b.Z };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator*(const FVector& a, const FVector& b) noexcept -> FVector
	{
		return { a.X * b.X, a.Y * b.Y, a.Z * b.Z };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator*(const FVector& v,  float f) noexcept -> FVector
	{
		return { v.X * f, v.Y * f, v.Z * f };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator-(float f, const FVector& v) noexcept -> FVector
	{
		return{ f - v.X, f - v.Y, f - v.Z };
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& operator+=(const FVector& v) noexcept
	{
		X += v.X;
		Y += v.Y;
		Z += v.Z;
		return *this;
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& operator-=(const FVector& v) noexcept
	{
		X -= v.X;
		Y -= v.Y;
		Z -= v.Z;
		return *this;
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& operator*=(const FVector& v) noexcept
	{
		X *= v.X;
		Y *= v.Y;
		Z *= v.Z;
		return *this;
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& operator/=(const FVector& v) noexcept
	{
		X /= v.X;
		Y /= v.Y;
		Z /= v.Z;
		return *this;
	})"),
			PredefinedMethod::Default("[[nodiscard]] auto toAngle() const noexcept -> FRotator;",
			R"(auto FVector::toAngle() const noexcept -> FRotator
	{
		return {
			math::rad2deg(std::atan2(Z, std::hypot(X, Y))),
			math::rad2deg(std::atan2(Y, X)),
			0.f
		};
	})"),
		};
		predefinedMethods["ScriptStruct CoreUObject.Vector2D"] = {
			PredefinedMethod::Inline(R"(	constexpr FVector2D(float x = 0.f, float y = 0.f) noexcept : X{ x }, Y{ y } {})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator-(const FVector2D& a, const FVector2D& b) noexcept -> FVector2D
	{
		return { a.X - b.X, a.Y - b.Y };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator+(const FVector2D& a, const FVector2D& b) noexcept -> FVector2D
	{
		return { a.X + b.X, a.Y + b.Y };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator*(const FVector2D& vec, float f) noexcept -> FVector2D
	{
		return { vec.X * f, vec.Y * f };
	})")
		};
		predefinedMethods["ScriptStruct CoreUObject.LinearColor"] = {
			PredefinedMethod::Inline(R"(	constexpr FLinearColor(float r = 0.f, float g = 0.f, float b = 0.f, float a = 0.f) noexcept : R{ r }, G{ g }, B{ b }, A{ a } {})")
		};
		predefinedMethods["ScriptStruct CoreUObject.Rotator"] = {
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator-(const FRotator& a, const FRotator& b) noexcept -> FRotator
	{
		return { a.Pitch - b.Pitch, a.Yaw - b.Yaw, a.Roll - b.Roll };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] friend constexpr auto operator+(const FRotator& a, const FRotator& b) noexcept -> FRotator
	{
		return { a.Pitch + b.Pitch, a.Yaw + b.Yaw, a.Roll + b.Roll };
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] constexpr auto fromAngle() const noexcept -> FVector
	{
		return {
			std::cos(math::deg2rad(Pitch)) * std::cos(math::deg2rad(Yaw)),
			std::cos(math::deg2rad(Pitch)) * std::sin(math::deg2rad(Yaw)),
			std::sin(math::deg2rad(Pitch))
		};
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& operator+=(const FRotator& o) noexcept
	{
		Pitch += o.Pitch;
		Yaw += o.Yaw;
		Roll += o.Roll;
		return *this;
	})"),
			PredefinedMethod::Inline(R"(	constexpr auto& normalize() noexcept
	{
		Pitch = std::isfinite(Pitch) ? std::remainder(Pitch, 360.f) : 0.f;
		Yaw = std::isfinite(Yaw) ? std::remainder(Yaw, 360.f) : 0.f;
		Roll = 0.f;
		return *this;
	})")
		};

		predefinedMethods["Class CoreUObject.Object"] = {
			PredefinedMethod::Inline(R"(	[[nodiscard]] static constexpr auto& GetGlobalObjects() noexcept
	{
		return GObjects->ObjObjects;
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] static constexpr auto FindObject(std::string_view name) noexcept -> T*
	{
		for (decltype(GetGlobalObjects().Num()) i = 0, max = GetGlobalObjects().Num(); i < max; ++i) {

			auto object = GetGlobalObjects().GetByIndex(i).Object;
			if (object == nullptr)
				continue;

			if (object->GetFullName() == name)
				return static_cast<T*>(object);
		}
		return nullptr;
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] static constexpr auto FindObjects(std::string_view name) noexcept -> std::vector<T*>
	{
		std::vector<T*> objects;
		for (decltype(GetGlobalObjects().Num()) i = 0, max = GetGlobalObjects().Num(); i < max; ++i) {

			auto object = GetGlobalObjects().GetByIndex(i).Object;
			if (object == nullptr)
				continue;

			if (object->GetFullName() == name)
				objects.emplace_back(static_cast<T*>(object));
		}
		return objects;
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] static constexpr auto FindObject() noexcept -> T*
	{
		auto StaticClass = T::StaticClass();
		for (decltype(GetGlobalObjects().Num()) i = 0, max = GetGlobalObjects().Num(); i < max; ++i) {

			auto object = GetGlobalObjects().GetByIndex(i).Object;
			if (object == nullptr)
				continue;

			if (object->IsA(StaticClass))
				return static_cast<T*>(object);
		}
		return nullptr;
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] static constexpr auto FindObjects() noexcept -> std::vector<T*>
	{
		std::vector<T*> objects;
		auto StaticClass = T::StaticClass();
		for (decltype(GetGlobalObjects().Num()) i = 0, max = GetGlobalObjects().Num(); i < max; ++i) {

			auto object = GetGlobalObjects().GetByIndex(i).Object;
			if (object == nullptr)
				continue;

			if (object->IsA(StaticClass))
				objects.emplace_back(static_cast<T*>(object));
		}
		return objects;
	})"),
			PredefinedMethod::Inline(R"(	[[nodiscard]] static auto FindClass(std::string_view name) noexcept
	{
		return FindObject<UClass>(name);
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] static auto GetObjectCasted(std::size_t index) noexcept
	{
		return static_cast<T*>(GetGlobalObjects().GetByIndex(index).Object);
	})"),
			PredefinedMethod::Default("[[nodiscard]] auto GetName() const noexcept -> std::string",
			R"(auto UObject::GetName() const noexcept -> std::string
	{
		std::string name{ Name.GetName() };
		if (Name.Number > 0)
			name += '_' + std::to_string(Name.Number);

		const auto pos = name.rfind('/');
		if (pos == std::string::npos)
			return name;

		return name.substr(pos + 1);
	})"),
			PredefinedMethod::Default("[[nodiscard]] auto GetFullName() const noexcept -> std::string",
			R"(auto UObject::GetFullName() const noexcept -> std::string
	{
		std::string name;
		if (Class == nullptr)
			return name;

		std::string temp;
		for (auto p = Outer; p; p = p->Outer)
			temp = p->GetName() + "." + temp;

		name = Class->GetName();
		name += " ";
		name += temp;
		name += GetName();

		return name;
	})"),
			PredefinedMethod::Default("auto IsA(UClass* cmp) const noexcept -> bool",
			R"(auto UObject::IsA(UClass* cmp) const noexcept -> bool
	{
		for (auto super = Class; super; super = static_cast<UClass*>(super->SuperField))
			if (super == cmp)
				return true;
		return false;
	})"),
			PredefinedMethod::Inline(R"(	template <typename T>
	[[nodiscard]] constexpr auto IsA() const noexcept
	{
		return IsA(T::StaticClass());
	})")
		};
		predefinedMethods["Class CoreUObject.Class"] = {
			PredefinedMethod::Inline(R"(	template <typename T>
	constexpr auto CreateDefaultObject() noexcept -> T*
	{
		return static_cast<T*>(CreateDefaultObject());
	})")
		};

		return true;
	}

	std::string GetGameName() const override
	{
		return "Remnant: From the Ashes";
	}

	std::string GetGameNameShort() const override
	{
		return "RFTA";
	}

	std::string GetGameVersion() const override
	{
		return "Remnant_Win64_Shipping_275957";
	}

	std::string GetNamespaceName() const override
	{
		return "SDK";
	}

	std::vector<std::string> GetIncludes() const override
	{
		return { };
	}

	std::string GetOutputDirectory() const
	{
		return "C:/SDK_GEN"; //TODO: add a feature so users can customize this
	}

	std::string GetBasicDeclarations() const override
	{
		return R"(template <typename T, typename K>
constexpr auto Cast(K* object) noexcept
{
	return static_cast<T*>(object);
}

template <typename T>
constexpr auto GetVFunction(const void *instance, std::size_t index) noexcept -> T
{
	const auto vtable = *reinterpret_cast<const void***>(const_cast<void*>(instance));
	return reinterpret_cast<T>(vtable[index]);
}

class FNameEntry
{
public:
	[[nodiscard]] constexpr auto GetAnsiName() const noexcept
	{
		return AnsiName;
	}

	[[nodiscard]] constexpr auto GetWideName() const noexcept
	{
		return WideName;
	}

private:
	class FNameEntry* HashNext; //0x0000
	int32_t Index; //0x0008

	union //0x000C
	{
		char AnsiName[2048];
		wchar_t WideName[1024];
	};
};

class TNameEntryArray
{
	enum
	{
		ElementsPerChunk = 16 * 1024,
		ChunkTableSize = (4 * 1024 * 1024 + ElementsPerChunk - 1) / ElementsPerChunk
	};

	[[nodiscard]] constexpr auto GetItemPtr(std::size_t Index) const noexcept
	{
		const auto ChunkIndex = Index / ElementsPerChunk;
		const auto WithinChunkIndex = Index % ElementsPerChunk;
		return Chunks[ChunkIndex] + WithinChunkIndex;
	}

public:
	[[nodiscard]] constexpr auto Num() const noexcept
	{
		return NumElements;
	}

	[[nodiscard]] constexpr auto IsValidIndex(std::size_t Index) const noexcept
	{
		return Index >= 0 && Index < NumElements && GetById(Index);
	}

	[[nodiscard]] constexpr auto& operator[](std::size_t Index) const noexcept
	{
		return *GetItemPtr(Index);
	}

private:
	FNameEntry* const* Chunks[ChunkTableSize]; //0x0000
	int32_t NumElements; //0x0800
	int32_t NumChunks; //0x0804
}; //Size: 0x0808
static_assert(sizeof(TNameEntryArray) == 0x808);

struct FName
{
	static TNameEntryArray* GNames;

	[[nodiscard]] static constexpr auto& GetGlobalNames() noexcept
	{
		return *GNames;
	}

	[[nodiscard]] constexpr auto GetName() const noexcept
	{
		return GetGlobalNames()[ComparisonIndex]->GetAnsiName();
	}

	[[nodiscard]] constexpr auto operator==(const FName& other) const noexcept
	{
		return ComparisonIndex == other.ComparisonIndex;
	}

	std::int32_t ComparisonIndex;
	std::uint32_t Number;

	constexpr FName() noexcept :
		ComparisonIndex(0),
		Number(0)
	{
	}

	constexpr FName(std::int32_t i) noexcept :
		ComparisonIndex(i),
		Number(0)
	{
	}

	FName(const char* nameToFind) noexcept :
		ComparisonIndex(0),
		Number(0)
	{
		static std::unordered_set<std::int32_t> cache;

		for (auto i : cache) {
			if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind)) {
				ComparisonIndex = i;
				return;
			}
		}

		for (decltype(GetGlobalNames().Num()) i = 0, max = GetGlobalNames().Num(); i < max; ++i) {
			if (GetGlobalNames()[i] != nullptr) {
				if (!std::strcmp(GetGlobalNames()[i]->GetAnsiName(), nameToFind)) {
					cache.insert(i);
					ComparisonIndex = i;
					return;
				}
			}
		}
	}
}; //Size: 0x0008
static_assert(sizeof(FName) == 0x8);

class FUObjectItem
{
	enum class EInternalObjectFlags : std::int32_t
	{
		None = 0,
		ReachableInCluster = 1 << 23,
		ClusterRoot = 1 << 24,
		Native = 1 << 25,
		Async = 1 << 26,
		AsyncLoading = 1 << 27,
		Unreachable = 1 << 28,
		PendingKill = 1 << 29,
		RootSet = 1 << 30,
		GarbageCollectionKeepFlags = Native | Async | AsyncLoading,
		AllFlags = ReachableInCluster | ClusterRoot | Native | Async | AsyncLoading | Unreachable | PendingKill | RootSet,
	};

public:
	class UObject* Object; //0x0000
	int32_t Flags; //0x0008
	int32_t ClusterRootIndex; //0x000C
	int32_t SerialNumber; //0x0010
	char pad_0014[4]; //0x0014

	[[nodiscard]] constexpr auto IsUnreachable() const noexcept
	{
		return !!(Flags & static_cast<std::underlying_type_t<EInternalObjectFlags>>(EInternalObjectFlags::Unreachable));
	}

	[[nodiscard]] constexpr auto IsPendingKill() const noexcept
	{
		return !!(Flags & static_cast<std::underlying_type_t<EInternalObjectFlags>>(EInternalObjectFlags::PendingKill));
	}
}; //Size: 0x0018
static_assert(sizeof(FUObjectItem) == 0x18);

class FChunkedFixedUObjectArray
{
public:
	enum
	{
		NumElementsPerChunk = 64 * 1024
	};

	[[nodiscard]] constexpr auto Num() const noexcept
	{
		return NumElements;
	}

	[[nodiscard]] constexpr auto GetObjectPtr(std::size_t Index) const noexcept
	{
		const auto ChunkIndex = Index / NumElementsPerChunk;
		const auto WithinChunkIndex = Index % NumElementsPerChunk;
		return Objects[ChunkIndex] + WithinChunkIndex;
	}

	[[nodiscard]] constexpr auto& GetByIndex(std::size_t Index) const noexcept
	{
		return *GetObjectPtr(Index);
	}
private:
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

template<class T>
class TArray
{
	friend class FString;

	T* Data; //0x0000
	int32_t Count; //0x0008
	int32_t Max; //0x000C

public:
	constexpr TArray() noexcept : Data{ nullptr }, Count{ 0 }, Max{ 0 }
	{
	};

	[[nodiscard]] constexpr auto Num() const noexcept
	{
		return Count;
	};

	[[nodiscard]] constexpr auto& operator[](std::size_t i) noexcept
	{
		return Data[i];
	};

	[[nodiscard]] constexpr const auto& operator[](std::size_t i) const noexcept
	{
		return Data[i];
	};

	[[nodiscard]] constexpr auto IsValidIndex(std::size_t i) const noexcept
	{
		return i < Num();
	}
}; //Size: 0x0010
static_assert(sizeof(TArray<void>) == 0x10);

class FString : public TArray<wchar_t>
{
public:
	constexpr FString() noexcept 
	{
	};

	constexpr explicit FString(const wchar_t* other) noexcept
	{
		Max = Count = *other ? static_cast<std::int32_t>(std::wcslen(other)) + 1 : 0;
		if (Count)
			Data = const_cast<wchar_t*>(other);
	};

	[[nodiscard]] constexpr auto IsValid() const noexcept
	{
		return Data != nullptr;
	}

	[[nodiscard]] constexpr auto c_str() const noexcept
	{
		return Data;
	}

	[[nodiscard]] auto ToString() const noexcept
	{
		const auto length = std::wcslen(Data);
		std::string str(length, '\0');
		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);
		return str;
	}
}; //Size: 0x0010
static_assert(sizeof(FString) == 0x10);

template<class TEnum>
class TEnumAsByte
{
public:
	constexpr TEnumAsByte() noexcept
	{
	}

	constexpr TEnumAsByte(TEnum _value) noexcept :
		value(static_cast<std::uint8_t>(_value))
	{
	}

	constexpr explicit TEnumAsByte(std::int32_t _value) noexcept :
		value(static_cast<std::uint8_t>(_value))
	{
	}

	constexpr explicit TEnumAsByte(std::uint8_t _value) noexcept :
		value(_value)
	{
	}

	[[nodiscard]] constexpr operator TEnum() const noexcept
	{
		return (TEnum)value;
	}

	[[nodiscard]] constexpr TEnum GetValue() const noexcept
	{
		return (TEnum)value;
	}

private:
	std::uint8_t value;
};

#pragma push_macro("GetObject")
#undef GetObject
class FScriptInterface
{
private:
	class UObject *ObjectPointer; //0x0000
	void* InterfacePointer; //0x0008

public:
	[[nodiscard]] constexpr auto GetObject() const noexcept
	{
		return ObjectPointer;
	}

	[[nodiscard]] constexpr auto& GetObjectRef() noexcept
	{
		return ObjectPointer;
	}

	[[nodiscard]] constexpr auto GetInterface() const noexcept
	{
		return ObjectPointer != nullptr ? InterfacePointer : nullptr;
	}
}; //Size: 0x0010
static_assert(sizeof(FScriptInterface) == 0x10);
#pragma pop_macro("GetObject")

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	[[nodiscard]] constexpr auto operator->() const noexcept -> InterfaceType*
	{
		return reinterpret_cast<InterfaceType*>(GetInterface());
	}

	[[nodiscard]] constexpr InterfaceType& operator*() const noexcept
	{
		return *reinterpret_cast<InterfaceType*>(GetInterface());
	}

	[[nodiscard]] constexpr explicit operator bool() const noexcept
	{
		return GetInterface() != nullptr;
	}
};

class FTextData
{
public:
	char UnknownData[0x28];
	wchar_t* Name;
	__int32 Length;
};

struct FText
{
	FTextData* Data;
	char UnknownData[0x10];

	[[nodiscard]] constexpr auto Get() const noexcept -> wchar_t*
	{
		if (Data)
			return Data->Name;
		return nullptr;
	}
};

struct FScriptDelegate
{
	char UnknownData[0x14];
};

struct FScriptMulticastDelegate
{
	char UnknownData[0x10];
};

template<typename Key, typename Value>
class TMap
{
	char UnknownData[0x50];
};

struct FWeakObjectPtr
{
public:
	[[nodiscard]] auto IsValid() const noexcept -> bool;
	[[nodiscard]] auto Get() const noexcept -> class UObject*;

	int32_t ObjectIndex; //0x0000
	int32_t ObjectSerialNumber; //0x0004
}; //Size: 0x0008
static_assert(sizeof(FWeakObjectPtr) == 0x8);

template<class T, class TWeakObjectPtrBase = FWeakObjectPtr>
struct TWeakObjectPtr : private TWeakObjectPtrBase
{
public:
	constexpr auto Get() const noexcept -> T*
	{
		return reinterpret_cast<T*>(TWeakObjectPtrBase::Get());
	}

	[[nodiscard]] constexpr auto& operator*() const noexcept
	{
		return *Get();
	}

	[[nodiscard]] constexpr auto operator->() const noexcept
	{
		return Get();
	}

	[[nodiscard]] constexpr auto IsValid() const noexcept
	{
		return TWeakObjectPtrBase::IsValid();
	}
};

template<class T, class TBASE>
class TAutoPointer : public TBASE
{
public:
	[[nodiscard]] constexpr operator T*() const noexcept
	{
		return TBASE::Get();
	}

	[[nodiscard]] constexpr operator const T*() const noexcept
	{
		return (const T*)TBASE::Get();
	}

	[[nodiscard]] constexpr explicit operator bool() const noexcept
	{
		return TBASE::Get() != nullptr;
	}
};

template<class T>
class TAutoWeakObjectPtr : public TAutoPointer<T, TWeakObjectPtr<T>>
{
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr WeakPtr;
	std::int32_t TagAtLastTest;
	TObjectID ObjectID;
};

struct FStringAssetReference_
{
	char UnknownData[0x10];
};

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference_>
{
};

template<typename ObjectType>
class TAssetPtr : FAssetPtr
{
};

struct FSoftObjectPath
{
	FName AssetPathName;
	FString SubPathString;
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
};

template<typename ObjectType>
class TSoftObjectPtr : FSoftObjectPtr
{
};

struct FUniqueObjectGuid_
{
	char UnknownData[0x10];
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid_>
{
};

template<typename ObjectType>
class TLazyObjectPtr : FLazyObjectPtr
{
};

template<class TClass>
class TSubclassOf
{
public:
	constexpr TSubclassOf(class UClass* Class) noexcept
	{
		this->Class = Class;
	}

	[[nodiscard]] constexpr auto GetClass() noexcept
	{
		return Class;
	}

private:
	class UClass* Class;
};)";
	}

	std::string GetBasicDefinitions() const override
	{
		return R"(	TNameEntryArray* FName::GNames = nullptr;
	FUObjectArray* UObject::GObjects = nullptr;

	auto FWeakObjectPtr::IsValid() const noexcept -> bool
	{
		if (ObjectSerialNumber == 0)
			return false;

		if (ObjectIndex < 0 || ObjectIndex >= UObject::GetGlobalObjects().Num())
			return false;

		const auto& item = UObject::GetGlobalObjects().GetByIndex(ObjectIndex);
		if (item.SerialNumber != ObjectSerialNumber)
			return false;

		return !(item.IsUnreachable() || item.IsPendingKill());
	}

	auto FWeakObjectPtr::Get() const noexcept -> class UObject*
	{
		if (!IsValid())
			return nullptr;

		auto& item = UObject::GetGlobalObjects().GetByIndex(ObjectIndex);
		return item.Object;
	})";
	}
};

Generator _generator;
IGenerator* generator = &_generator;