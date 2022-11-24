#include <Windows.h>

struct FPointer
{
	uintptr_t Dummy;
};

struct FQWord
{
	int32_t A;
	int32_t B;
};

class FName
{
public:
	int32_t ComparisonIndex;
	int32_t Number;
};

template<class T>
class TArray
{
	friend class FString;

public:
	TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	size_t Num() const
	{
		return Count;
	};

	T& operator[](size_t i)
	{
		return Data[i];
	};

	const T& operator[](size_t i) const
	{
		return Data[i];
	};

	bool IsValidIndex(size_t i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

template<typename KeyType, typename ValueType>
class TPair
{
public:
	KeyType   Key;
	ValueType Value;
};

class FString : public TArray<wchar_t>
{
public:
	std::string ToString() const
	{
		const int size = WideCharToMultiByte(CP_UTF8, 0, Data, Count, nullptr, 0, nullptr, nullptr);
		std::string str(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, Data, Count, &str[0], size, nullptr, nullptr);
		return str;
	}
};

class FScriptInterface
{
private:
	UObject* ObjectPointer;
	void* InterfacePointer;

public:
	UObject* GetObject() const
	{
		return ObjectPointer;
	}

	UObject*& GetObjectRef()
	{
		return ObjectPointer;
	}

	void* GetInterface() const
	{
		return ObjectPointer != nullptr ? InterfacePointer : nullptr;
	}
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	InterfaceType* operator->() const
	{
		return (InterfaceType*)GetInterface();
	}

	InterfaceType& operator*() const
	{
		return *((InterfaceType*)GetInterface());
	}

	operator bool() const
	{
		return GetInterface() != nullptr;
	}
};

struct FText
{
	char UnknownData[0x28];
};

struct FScriptDelegate
{
	char UnknownData[0x14];
};

struct FScriptMulticastDelegate
{
	char UnknownData[0x10];
};

struct FWeakObjectPtr
{
	int32_t ObjectIndex;
	int32_t ObjectSerialNumber;
};

struct FStringAssetReference
{
	FString AssetLongPathname;
};

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr WeakPtr;
	int32_t TagAtLastTest;
	TObjectID ObjectID;
};

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference>
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

struct FGuid
{
	uint32_t A;
	uint32_t B;
	uint32_t C;
	uint32_t D;
};

struct FUniqueObjectGuid
{
	FGuid Guid;
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
{

};

class UObject
{
public:
	void* Vtable; //0x0000
	int32_t ObjectFlags; //0x0008
	int32_t InternalIndex; //0x000C
	class UClass* Class; //0x0010
	class FName Name; //0x0018
	class UObject* Outer; //0x0020
}; //Size: 0x0028
static_assert(sizeof(UObject) == 0x28);

class UField : public UObject
{
public:
	class UField* Next; //0x0028
}; //Size: 0x0030
static_assert(sizeof(UField) == 0x30);

class UEnum : public UField
{
public:
	class FString CppType; //0x0030
	class TArray<TPair<FName, int64_t>> Names; //0x0040
	int64_t CppForm; //0x0050
	char pad_0058[8]; //0x0058
}; //Size: 0x0060
static_assert(sizeof(UEnum) == 0x60);

class UProperty : public UField
{
public:
	int32_t ArrayDim;
	int32_t ElementSize;
	FQWord PropertyFlags;
	int16_t RepIndex;
	uint8_t BlueprintReplicationCondition;
	char UnknownData[0x1];
	int32_t Offset;
	FName RepNotifyFunc;
	UProperty* PropertyLinkNext;
	UProperty* NextRef;
	UProperty* DestructorLinkNext;
	UProperty* PostConstructLinkNext;
};
static_assert(sizeof(UProperty) == 0x70);

class UStruct : public UField
{
public:
	class UStruct* SuperField; //0x0030
	class UField* Children; //0x0038
	int32_t PropertySize; //0x0040
	int32_t MinAlignment; //0x0044
	char pad_0048[64]; //0x0048
}; //Size: 0x0088
static_assert(sizeof(UStruct) == 0x88);

class UFunction : public UStruct
{
public:
	int32_t FunctionFlags; //0x0088
	int16_t NumParms; //0x008C
	int8_t ParmsSize; //0x008E
	char pad_008F[1]; //0x008F
	uint16_t ReturnValueOffset; //0x0090
	char pad_0092[30]; //0x0092
	void* Func; //0x00B0
}; //Size: 0x00B8
static_assert(sizeof(UFunction) == 0xB8);

class UClass : public UStruct
{
public:
	char pad_0088[376]; //0x0088
}; //Size: 0x0200
static_assert(sizeof(UClass) == 0x200);

class UScriptStruct : public UStruct
{
public:
	char pad_0088[16]; //0x0088
}; //Size: 0x0098
static_assert(sizeof(UScriptStruct) == 0x98);

class UNumericProperty : public UProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UNumericProperty) == 0x70);

class UByteProperty : public UNumericProperty
{
public:
	class UEnum* Enum;; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UByteProperty) == 0x78);

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

class UInt8Property : public UNumericProperty
{
public:

};

class UInt16Property : public UNumericProperty
{
public:

};

class UIntProperty : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UIntProperty) == 0x70);

class UInt64Property : public UNumericProperty
{
public:
}; //Size: 0x0070
static_assert(sizeof(UInt64Property) == 0x70);

class UFloatProperty : public UNumericProperty
{
public:

};

class UDoubleProperty : public UNumericProperty
{
public:

};

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

class UObjectPropertyBase : public UProperty
{
public:
	class UClass* PropertyClass; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UObjectPropertyBase) == 0x78);

class UObjectProperty : public UObjectPropertyBase
{
public:
}; //Size: 0x0078
static_assert(sizeof(UObjectProperty) == 0x78);

class UClassProperty : public UObjectProperty
{
public:
	class UClass* MetaClass; //0x0078
}; //Size: 0x0080
static_assert(sizeof(UClassProperty) == 0x80);

class UInterfaceProperty : public UProperty
{
public:
	class UClass* InterfaceClass; //0x0070
}; //Size: 0x0078
static_assert(sizeof(UInterfaceProperty) == 0x78);

class UWeakObjectProperty : public UObjectPropertyBase
{
public:

};

class ULazyObjectProperty : public UObjectPropertyBase
{
public:

};

class UAssetObjectProperty : public UObjectPropertyBase
{
public:

};

class UAssetClassProperty : public UAssetObjectProperty
{
public:
	UClass* MetaClass;
};

class UNameProperty : public UProperty
{
public:

};

class UStructProperty : public UProperty
{
public:
	UScriptStruct* Struct;
};

class UStrProperty : public UProperty
{
public:

};

class UTextProperty : public UProperty
{
public:

};

class UArrayProperty : public UProperty
{
public:
	UProperty* Inner;
};

class UMapProperty : public UProperty
{
public:
	class UProperty* KeyProp; //0x0070
	class UProperty* ValueProp; //0x0078
	char pad_0080[40]; //0x0080
}; //Size: 0x00A8
static_assert(sizeof(UMapProperty) == 0xA8);

class UDelegateProperty : public UProperty
{
public:
	UFunction* SignatureFunction;
};

class UMulticastDelegateProperty : public UProperty
{
public:
	UFunction* SignatureFunction;
};

class UEnumProperty : public UProperty
{
public:
	UNumericProperty* UnderlyingProp;
	UEnum* Enum;
};