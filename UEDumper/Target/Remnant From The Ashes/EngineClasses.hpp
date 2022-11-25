#include <Windows.h>

enum class EPropertyFlags : int64_t
{
	None = 0,
	Edit = 0x0000000000000001,
	ConstParm = 0x0000000000000002,
	BlueprintVisible = 0x0000000000000004,
	ExportObject = 0x0000000000000008,
	BlueprintReadOnly = 0x0000000000000010,
	Net = 0x0000000000000020,
	EditFixedSize = 0x0000000000000040,
	Parm = 0x0000000000000080,
	OutParm = 0x0000000000000100,
	ZeroConstructor = 0x0000000000000200,
	ReturnParm = 0x0000000000000400,
	DisableEditOnTemplate = 0x0000000000000800,
	Transient = 0x0000000000002000,
	Config = 0x0000000000004000,
	DisableEditOnInstance = 0x0000000000010000,
	EditConst = 0x0000000000020000,
	GlobalConfig = 0x0000000000040000,
	InstancedReference = 0x0000000000080000,
	DuplicateTransient = 0x0000000000200000,
	SubobjectReference = 0x0000000000400000,
	SaveGame = 0x0000000001000000,
	NoClear = 0x0000000002000000,
	ReferenceParm = 0x0000000008000000,
	BlueprintAssignable = 0x0000000010000000,
	Deprecated = 0x0000000020000000,
	IsPlainOldData = 0x0000000040000000,
	RepSkip = 0x0000000080000000,
	RepNotify = 0x0000000100000000,
	Interp = 0x0000000200000000,
	NonTransactional = 0x0000000400000000,
	EditorOnly = 0x0000000800000000,
	NoDestructor = 0x0000001000000000,
	AutoWeak = 0x0000004000000000,
	ContainsInstancedReference = 0x0000008000000000,
	AssetRegistrySearchable = 0x0000010000000000,
	SimpleDisplay = 0x0000020000000000,
	AdvancedDisplay = 0x0000040000000000,
	Protected = 0x0000080000000000,
	BlueprintCallable = 0x0000100000000000,
	BlueprintAuthorityOnly = 0x0000200000000000,
	TextExportTransient = 0x0000400000000000,
	NonPIEDuplicateTransient = 0x0000800000000000,
	ExposeOnSpawn = 0x0001000000000000,
	PersistentInstance = 0x0002000000000000,
	UObjectWrapper = 0x0004000000000000,
	HasGetValueTypeHash = 0x0008000000000000,
	NativeAccessSpecifierPublic = 0x0010000000000000,
	NativeAccessSpecifierProtected = 0x0020000000000000,
	NativeAccessSpecifierPrivate = 0x0040000000000000,
	SkipSerialization = 0x0080000000000000
};

template<typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType   Key;
	ValueType Value;
};

class FName
{
public:
	int32_t ComparisonIndex; //0x0000
	uint32_t Number; //0x0004
}; //Size: 0x0008
static_assert(sizeof(FName) == 0x8);

class UObject
{
public:
	void* VTable; //0x0000
	int32_t ObjectFlags; //0x0008
	int32_t InternalIndex; //0x000C
	class UClass *ClassPrivate; //0x0010
	class FName NamePrivate; //0x0018
	class UObject *OuterPrivate; //0x0020
}; //Size: 0x0028
static_assert(sizeof(UObject) == 0x28);

class UField : public UObject
{
public:
	class UField *Next; //0x0028
}; //Size: 0x0030
static_assert(sizeof(UField) == 0x30);

class UStruct : public UField
{
public:
	char pad_0030[16]; //0x0030
	class UStruct *SuperStruct; //0x0040
	class UField *Children; //0x0048
	int32_t PropertySize; //0x0050
	int32_t MinAlignment; //0x0054
	char pad_0058[64]; //0x0058
}; //Size: 0x0098
static_assert(sizeof(UStruct) == 0x98);

class UClass : public UStruct
{
public:
	char pad_0098[360]; //0x0098
}; //Size: 0x0200
static_assert(sizeof(UClass) == 0x200);

template<class T>
class TArray
{
private:
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

	[[nodiscard]] constexpr auto& operator[](std::size_t i) const noexcept
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
	[[nodiscard]] auto ToString() const noexcept
	{
		const auto size = WideCharToMultiByte(CP_UTF8, 0, Data, Count, nullptr, 0, nullptr, nullptr);
		std::string str(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, Data, Count, &str[0], size, nullptr, nullptr);
		return str;
	}
}; //Size: 0x0010
static_assert(sizeof(FString) == 0x10);

class UEnum : public UField
{
public:
	class FString CppType; //0x0030
	class TArray<TPair<FName, int64_t>> Names; //0x0040
	int32_t CppForm; //0x0050
	char pad_0054[4]; //0x0054
	void* EnumDisplayNameFn; //0x0058
}; //Size: 0x0060
static_assert(sizeof(UEnum) == 0x60);

class UScriptStruct : public UStruct
{
public:
	int32_t StructFlags; //0x0098
	bool bPrepareCppStructOpsCompleted; //0x009C
	char pad_009D[3]; //0x009D
	int64_t CppStructOps; //0x00A0
}; //Size: 0x00A8
static_assert(sizeof(UScriptStruct) == 0xA8);

class UFunction : public UStruct
{
public:
	int32_t FunctionFlags; //0x0098
	int8_t NumParms; //0x009C
	char pad_009D[1]; //0x009D
	int16_t ParmsSize; //0x009E
	uint16_t ReturnValueOffset; //0x00A0
	int16_t RPCId; //0x00A2
	int16_t RPCResponseId; //0x00A4
	char pad_00A6[2]; //0x00A6
	int64_t FirstPropertyToInit; //0x00A8
	int64_t EventGraphFunction; //0x00B0
	int32_t EventGraphCallOffset; //0x00B8
	char pad_00BC[4]; //0x00BC
	void* Func; //0x00C0
}; //Size: 0x00C8
static_assert(sizeof(UFunction) == 0xC8);

class UProperty : public UField
{
public:
	int32_t ArrayDim; //0x0030
	int32_t ElementSize; //0x0034
	EPropertyFlags PropertyFlags; //0x0038
	int16_t RepIndex; //0x0040
	int8_t BlueprintReplicationCondition; //0x0042
	char pad_0043[1]; //0x0043
	int32_t Offset_Internal; //0x0044
	class FName *RepNotifyFunc; //0x0048
	class UField *PropertyLinkNext; //0x0050
	void* NextRef; //0x0058
	void* DestructorLinkNext; //0x0060
	void* PostConstructLinkNext; //0x0068
}; //Size: 0x0070
static_assert(sizeof(UProperty) == 0x70);

class UNumericProperty : public UProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UNumericProperty) == 0x70);

class UObjectPropertyBase : public UProperty
{
public:
	class UClass *PropertyClass; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UObjectPropertyBase) == 0x78);

class UByteProperty : public UNumericProperty
{
public:
	class UEnum* Enum; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UByteProperty) == 0x78);

class UObjectProperty : public UObjectPropertyBase
{
public:
}; //Size: 0x0078
static_assert(sizeof(UObjectProperty) == 0x78);

class UClassProperty : public UObjectProperty
{
public:
	class UClass *MetaClass; //0x0078
}; //Size: 0x0080
static_assert(sizeof(UClassProperty) == 0x80);

class UDelegateProperty : public UProperty
{
public:
	class UFunction* SignatureFunction; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UDelegateProperty) == 0x78);

class UDoubleProperty : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UDoubleProperty) == 0x70);

class UFloatProperty : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UFloatProperty) == 0x70);

class UIntProperty : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UIntProperty) == 0x70);

class UInt16Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UInt16Property) == 0x70);

class UInt64Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UInt64Property) == 0x70);

class UInt8Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UInt8Property) == 0x70);

class UInterfaceProperty : public UProperty
{
public:
	class UClass *InterfaceClass; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UInterfaceProperty) == 0x78);

class ULazyObjectProperty : public UObjectPropertyBase
{
public:
}; //Size: 0x0078
static_assert(sizeof(ULazyObjectProperty) == 0x78);

class UMapProperty : public UProperty
{
public:
	class UProperty *KeyProp; //0x0070
	class UProperty *ValueProp; //0x0078
	char pad_0080[24]; //0x0080
}; //Size: 0x0098
static_assert(sizeof(UMapProperty) == 0x98);

class UMulticastDelegateProperty : public UProperty
{
public:
	class UFunction *SignatureFunction; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UMulticastDelegateProperty) == 0x78);

class UStrProperty : public UProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UStrProperty) == 0x70);

class UStructProperty : public UProperty
{
public:
	class UScriptStruct *Struct; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UStructProperty) == 0x78);

class UUInt16Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UUInt16Property) == 0x70);

class UUInt32Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UUInt32Property) == 0x70);

class UUInt64Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UUInt64Property) == 0x70);

class UTextProperty : public UProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UTextProperty) == 0x70);

class UEnumProperty : public UProperty
{
public:
	class UNumericProperty *UnderlyingProp; //0x0070
	class UEnum *Enum; //0x0078
}; //Size: 0x0080
static_assert(sizeof(UEnumProperty) == 0x80);

class UArrayProperty : public UProperty
{
public:
	class UProperty *Inner; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UArrayProperty) == 0x78);

class UBoolProperty : public UProperty
{
public:
	int8_t FieldSize; //0x0070
	int8_t ByteOffset; //0x0071
	int8_t ByteMask; //0x0072
	int8_t FieldMask; //0x0073
	char pad_0074[4]; //0x0074
}; //Size: 0x0078
static_assert(sizeof(UBoolProperty) == 0x78);

class FPointer
{
public:
	uint64_t Dummy; //0x0000
}; //Size: 0x0008
static_assert(sizeof(FPointer) == 0x8);

class FText
{
public:
	void* Object; //0x0000
	int64_t SharedReferenceCount; //0x0008
	uint32_t Flags; //0x0010
	char pad_0014[4]; //0x0014
}; //Size: 0x0018
static_assert(sizeof(FText) == 0x18);

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

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	[[nodiscard]] constexpr auto operator->() const noexcept
	{
		return reinterpret_cast<InterfaceType*>(GetInterface());
	}

	[[nodiscard]] constexpr auto& operator*() const noexcept
	{
		return *reinterpret_cast<InterfaceType*>(GetInterface());
	}

	[[nodiscard]] constexpr operator bool() const noexcept
	{
		return GetInterface() != nullptr;
	}
};

class FGuid
{
public:
	uint32_t A; //0x0000
	uint32_t B; //0x0004
	uint32_t C; //0x0008
	uint32_t D; //0x000C
}; //Size: 0x0010
static_assert(sizeof(FGuid) == 0x10);

class UNameProperty : public UProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UNameProperty) == 0x70);

class UWeakObjectProperty : public UObjectPropertyBase
{
public:
}; //Size: 0x0078
static_assert(sizeof(UWeakObjectProperty) == 0x78);

class FUniqueObjectGuid : public FGuid
{
public:
}; //Size: 0x0010
static_assert(sizeof(FUniqueObjectGuid) == 0x10);

class FWeakObjectPtr
{
public:
	int32_t ObjectIndex; //0x0000
	int32_t ObjectSerialNumber; //0x0004
}; //Size: 0x0008
static_assert(sizeof(FWeakObjectPtr) == 0x8);

class FSoftObjectPath
{
public:
	class FName AssetPathName; //0x0000
	class FString SubPathString; //0x0008
}; //Size: 0x0018
static_assert(sizeof(FSoftObjectPath) == 0x18);

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	class FWeakObjectPtr WeakPtr; //0x0000
	int32_t TagAtLastTest; //0x0008
	TObjectID ObjectID; //0x000C
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
public:
}; //Size: 0x0028
static_assert(sizeof(FSoftObjectPtr) == 0x28);

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
{
public:
}; //Size: 0x001C
static_assert(sizeof(FLazyObjectPtr) == 0x1C);

struct FScriptDelegate
{
	char UnknownData[0x14];
};

struct FScriptMulticastDelegate
{
	char UnknownData[0x10];
};

struct FStringAssetReference
{
	FString AssetLongPathname;
};

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference>
{
};

class UAssetObjectProperty : public UObjectPropertyBase
{
};

class UAssetClassProperty : public UAssetObjectProperty
{
public:
	UClass* MetaClass;
};