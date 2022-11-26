#pragma once

#include <array>
#include <span>

#include <Windows.h>

#pragma push_macro("max")
#undef max

using PatternType = std::uint8_t;

template <auto Value>
class PatternValue
{
public:
    using Type = decltype(Value);
    static constexpr Type value = Value;
};

template <std::size_t Size>
class PatternWrapper
{
public:
    std::array<PatternType, Size> bytes = {};

    consteval PatternWrapper(const char* pattern) noexcept
    {
        for (std::size_t index = 0; index != Size; ++index)
            bytes[index] = pattern[index];
    }
    constexpr ~PatternWrapper() noexcept = default;

    PatternWrapper(PatternWrapper&&) = delete;
    PatternWrapper(const PatternWrapper&) = delete;

    PatternWrapper& operator=(PatternWrapper&&) = delete;
    PatternWrapper& operator=(const PatternWrapper&) = delete;

    [[nodiscard]] constexpr auto operator()() const noexcept
    {
        return bytes;
    }

    [[nodiscard]] consteval auto find_first_of_start(const std::size_t start, const PatternType delimiter) const noexcept -> std::size_t
    {
        for (auto index = start; index < Size; ++index)
            if (bytes[index] == delimiter)
                return index;

        return std::numeric_limits<std::size_t>::max();
    }

    [[nodiscard]] constexpr auto find_first_not_of_start(const std::size_t start, const PatternType delimiter) const noexcept -> std::size_t
    {
        for (auto index = start; index < Size; ++index)
            if (bytes[index] != delimiter)
                return index;

        return std::numeric_limits<std::size_t>::max();
    }

    [[nodiscard]] consteval auto find_last_not_of(const PatternType delimiter) const noexcept -> std::size_t
    {
        for (auto index = Size - 1; index >= 0; --index)
            if (bytes[index] != delimiter)
                return index;

        return std::numeric_limits<std::size_t>::max();
    }
};

template <std::size_t Size>
PatternWrapper(const char(&)[Size]) -> PatternWrapper<Size - 1>;

template <PatternWrapper Pattern, PatternType Wildcard = '?', PatternType Masked = std::numeric_limits<PatternType>::max(), PatternType Delimiter = ' '>
class PatternView
{
private:
    class PatternParser
    {
    public:
        PatternParser() = delete;
        ~PatternParser() = delete;

        PatternParser(PatternParser&&) = delete;
        PatternParser(const PatternParser&) = delete;

        PatternParser& operator=(PatternParser&&) = delete;
        PatternParser& operator=(const PatternParser&) = delete;

        class PatternResult
        {
        public:
            consteval PatternResult(const std::size_t count, const std::size_t start, const std::size_t next, const std::size_t end) noexcept :
                count{ count },
                start{ start },
                next{ next },
                end{ end }
            {
            }
            constexpr ~PatternResult() noexcept = default;

            PatternResult(PatternResult&&) = delete;
            PatternResult(const PatternResult&) = delete;

            PatternResult& operator=(PatternResult&&) = delete;
            PatternResult& operator=(const PatternResult&) = delete;

            std::size_t count;
            std::size_t start;
            std::size_t next;
            std::size_t end;
        };

        [[nodiscard]] static consteval auto Get() noexcept
        {
            constexpr auto start = Pattern.find_first_not_of_start(0, Delimiter);
            constexpr auto next = Pattern.find_first_of_start(start, Delimiter);
            constexpr auto end = Pattern.find_last_not_of(Delimiter);

            std::size_t count = 1;
            auto previousDelimiter = false;

            for (auto index = next; index < end; ++index) {
                if (Pattern()[index] == Delimiter) {
                    if (!previousDelimiter)
                        ++count;
                    previousDelimiter = true;
                }
                else previousDelimiter = false;
            }

            return PatternResult{ count, start, next, end };
        }

        [[nodiscard]] static constexpr auto char_to_hex(const PatternType ch) noexcept -> PatternType
        {
            if (ch >= '0' && ch <= '9')
                return ch - '0';

            if (ch >= 'A' && ch <= 'F')
                return ch - 'A' + 10;

            return ch - 'a' + 10;
        }

        [[nodiscard]] static constexpr auto concat_hex(const PatternType lhs, const PatternType rhs) noexcept -> PatternType
        {
            return 16 * lhs + rhs;
        }
    };

    [[nodiscard]] static constexpr auto Get() noexcept
    {
        constexpr auto data = PatternParser::Get();

        auto previousSkip = false;
        std::size_t skipped = {};
        std::array<std::size_t, data.count> skips = {};

        for (auto traversed = data.start; traversed < data.end; ++traversed) {
            if (Pattern()[traversed] == Delimiter) {
                if (!previousSkip)
                    skips[skipped++] = traversed;
                previousSkip = true;
            }
            else previousSkip = false;
        }

        std::size_t conversions = 1;
        std::array<PatternType, data.count> result = {};

        result[0] = Pattern()[data.start] == Wildcard ? Masked : PatternParser::concat_hex(PatternParser::char_to_hex(Pattern()[data.start]), PatternParser::char_to_hex(Pattern()[data.start + 1]));
        for (auto traversed = data.next; traversed < data.end; ++traversed) {
            for (auto entry : skips) {
                if (entry == traversed && entry < data.end) {
                    const auto index = Pattern.find_first_not_of_start(traversed + 1, Delimiter);
                    const auto oneChar = Pattern()[index + 1] == Delimiter;
                    result[conversions++] = Pattern()[index] == Wildcard ? Masked : (oneChar ? PatternParser::char_to_hex(Pattern()[index]) : PatternParser::concat_hex(PatternParser::char_to_hex(Pattern()[index]), PatternParser::char_to_hex(Pattern()[index + 1])));
                }
            }
        }

        return result;
    }

public:
    PatternView() = delete;
    ~PatternView() = delete;

    PatternView(PatternView&&) = delete;
    PatternView(const PatternView&) = delete;

    PatternView& operator=(PatternView&&) = delete;
    PatternView& operator=(const PatternView&) = delete;

    static constexpr auto value = PatternValue<Get()>::value;
};

class PatternFinder
{
public:
    constexpr explicit PatternFinder(std::span<const std::byte> bytes) noexcept :
        bytes{ bytes }
    {
    }
    constexpr ~PatternFinder() noexcept = default;

    PatternFinder(PatternFinder&&) = delete;
    PatternFinder(const PatternFinder&) = delete;

    PatternFinder& operator=(PatternFinder&&) = delete;
    PatternFinder& operator=(const PatternFinder&) = delete;

    [[nodiscard]] auto operator()(std::span<const PatternType> pattern) const noexcept -> const std::byte*;

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

#pragma pop_macro("max")