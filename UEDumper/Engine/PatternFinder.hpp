#pragma once

#include <array>
#include <span>

#include <Windows.h>

template<auto Value>
class PatternCompile
{
public:
    using Type = decltype(Value);
    static constexpr Type value = Value;
};

template <std::size_t Size>
class PatternWrapper
{
public:
    std::array<std::uint8_t, Size> bytes = {};

    constexpr PatternWrapper(const char* pattern) noexcept
    {
        for (std::size_t index = 0; index != Size; ++index)
            bytes[index] = pattern[index];
    }

    constexpr PatternWrapper(const wchar_t* pattern) noexcept
    {
        for (std::size_t index = 0; index != Size; ++index)
            bytes[index] = static_cast<decltype(bytes)::element_type>(pattern[index]);
    }

    PatternWrapper(PatternWrapper&&) = delete;
    PatternWrapper(const PatternWrapper&) = delete;

    PatternWrapper& operator=(PatternWrapper&&) = delete;
    PatternWrapper& operator=(const PatternWrapper&) = delete;

    [[nodiscard]] constexpr auto operator()() const noexcept
    {
        return bytes;
    }
};

template <std::size_t Size>
PatternWrapper(const char(&)[Size]) -> PatternWrapper<Size - 1>;

template <std::size_t Size>
PatternWrapper(const wchar_t(&)[Size]) -> PatternWrapper<Size - 1>;

template <PatternWrapper Pattern>
class PatternView
{
private:
    [[nodiscard]] static constexpr auto Get() noexcept
    {
        return Pattern();
    }

public:
    PatternView() = delete;
    ~PatternView() = delete;

    PatternView(PatternView&&) = delete;
    PatternView(const PatternView&) = delete;

    PatternView& operator=(PatternView&&) = delete;
    PatternView& operator=(const PatternView&) = delete;

    static constexpr auto value = PatternCompile<Get()>::value;
};

class PatternFinder
{
public:
    constexpr explicit PatternFinder(std::span<const std::byte> bytes) noexcept :
        bytes{ bytes }
    {
    }

    PatternFinder(PatternFinder&&) = delete;
    PatternFinder(const PatternFinder&) = delete;

    PatternFinder& operator=(PatternFinder&&) = delete;
    PatternFinder& operator=(const PatternFinder&) = delete;

    [[nodiscard]] auto operator()(std::span<const std::uint8_t> pattern) const noexcept -> const std::byte*;

private:
    std::span<const std::byte> bytes;
};

template <typename ReturnType, typename OffsetType = std::int32_t>
[[nodiscard]] constexpr auto RelativeToAbsolute(std::uintptr_t address) noexcept
{
    const auto addressOfNextInstruction = address + sizeof(OffsetType);
    const auto returnAddress = reinterpret_cast<void*>(addressOfNextInstruction + *reinterpret_cast<const OffsetType*>(address));
    return reinterpret_cast<ReturnType*>(returnAddress);
}