#include <emmintrin.h>

#include "PatternFinder.hpp"

#pragma push_macro("max")
#undef max

[[nodiscard]] static constexpr auto operator==(std::span<const std::byte> bytes, std::span<const std::uint8_t> pattern) noexcept
{
    using PatternType = decltype(pattern)::element_type;
    for (std::size_t i = 0; i < bytes.size(); ++i)
        if (std::to_integer<PatternType>(bytes[i]) != pattern[i] && pattern[i] != std::numeric_limits<PatternType>::max())
            return false;
    return true;
}

auto PatternFinder::operator()(std::span<const std::uint8_t> pattern) const noexcept -> const std::byte*
{
    const auto indexOfLastPatternChar = pattern.size() - 1;
    const auto patternWithoutFirstAndLastChar = pattern.subspan(1, pattern.size() >= 2 ? pattern.size() - 2 : 0);
    const auto byteSpanPerIteration = indexOfLastPatternChar + sizeof(__m128i);

    const auto firstCharMask = _mm_set1_epi8(static_cast<char>(pattern[0]));
    const auto lastCharMask = _mm_set1_epi8(static_cast<char>(pattern.back()));

    std::size_t i{};

    for (; i + byteSpanPerIteration < bytes.size(); i += sizeof(__m128i)) {
        const auto possibleFirstChars = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&bytes[i]));
        const auto possibleLastChars = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&bytes[i + indexOfLastPatternChar]));

        const auto firstCharMatchPositions = _mm_cmpeq_epi8(firstCharMask, possibleFirstChars);
        const auto lastCharMatchPositions = _mm_cmpeq_epi8(lastCharMask, possibleLastChars);

        auto mask = static_cast<std::uint16_t>(_mm_movemask_epi8(_mm_and_si128(firstCharMatchPositions, lastCharMatchPositions)));
        while (mask != 0) {
            if (const auto bitPos = std::countr_zero(mask); bytes.subspan(i + bitPos + 1, patternWithoutFirstAndLastChar.size()) == patternWithoutFirstAndLastChar)
                return &bytes[i + bitPos];

            mask = mask & (mask - 1);
        }
    }

    while (i + pattern.size() <= bytes.size()) {
        if (bytes.subspan(i, pattern.size()) == pattern)
            return &bytes[i];

        ++i;
    }

    return nullptr;
}

#pragma pop_macro("max")