#pragma once

#include <cstdint>

class UEPointer
{
public:
	constexpr UEPointer() noexcept : data{ 0 } {}
	constexpr UEPointer(const std::uintptr_t address) noexcept : data{ address } {}
	constexpr ~UEPointer() noexcept = default;

	[[nodiscard]] constexpr operator bool() const noexcept
	{
		return data != 0;
	}

	template <typename T>
	[[nodiscard]] constexpr operator T() const noexcept
	{
		return static_cast<T>(data);
	}

	template <typename T>
	[[nodiscard]] constexpr operator T*() const noexcept
	{
		return reinterpret_cast<T*>(data);
	}

	template <typename T>
	[[nodiscard]] constexpr auto getAddressOf() noexcept
	{
		return (T*)(&data);
	}

	template <typename T>
	constexpr auto&& operator=(T* address) noexcept
	{
		data = reinterpret_cast<decltype(data)>(address);
		return *this;
	}

	template <typename T>
	constexpr auto&& operator=(T address) noexcept
	{
		data = static_cast<decltype(data)>(address);
		return *this;
	}

	constexpr auto&& operator+=(const UEPointer& ptr) noexcept
	{
		data += ptr.data;
		return *this;
	}

	[[nodiscard]] friend constexpr auto operator+(const UEPointer& a, const UEPointer& b) noexcept
	{
		return UEPointer{ a.data + b.data };
	}

	constexpr auto&& operator-=(const UEPointer& ptr) noexcept
	{
		data -= ptr.data;
		return *this;
	}

	[[nodiscard]] friend constexpr auto operator-(const UEPointer& a, const UEPointer& b) noexcept
	{
		return UEPointer{ a.data - b.data };
	}


private:
	std::uintptr_t data;
};