#include "wiringPi.H"

#ifndef __WIRING_PI_wiringPiStrings_H
#define __WIRING_PI_wiringPiStrings_H

namespace WiringPi
{
    [[nodiscard]] inline consteval const std::string_view toStringView(const auto &a)
    {
        return std::string_view(a.begin(), a.end());
    }

    [[nodiscard]] inline consteval size_t constexpr_strlen(const char *s)
    {
        return std::string::traits_type::length(s);
    }

    template <std::size_t... sizes>
    [[nodiscard]] inline consteval auto constexpr_strcat(const std::array<char, sizes> &...arrays)
    {
        std::array<char, ((sizes - 1) + ...) + 1> result;
        std::size_t index{};

        ((std::copy_n(arrays.begin(), sizes - 1, result.begin() + index), index += sizes - 1), ...);

        return result;
    }

    namespace detail
    {
        template <uint8_t... digits>
        struct positive_to_chars
        {
            static const char value[];
        };
        template <uint8_t... digits>
        constexpr const char positive_to_chars<digits...>::value[] = {('0' + digits)..., 0};

        template <uint8_t... digits>
        struct negative_to_chars
        {
            static const char value[];
        };
        template <uint8_t... digits>
        constexpr const char negative_to_chars<digits...>::value[] = {'-', ('0' + digits)..., 0};

        template <bool neg, uint8_t... digits>
        struct to_chars : positive_to_chars<digits...>
        {
        };

        template <uint8_t... digits>
        struct to_chars<true, digits...> : negative_to_chars<digits...>
        {
        };

        template <bool neg, uintmax_t rem, uint8_t... digits>
        struct explode : explode<neg, rem / 10, rem % 10, digits...>
        {
        };

        template <bool neg, uint8_t... digits>
        struct explode<neg, 0, digits...> : to_chars<neg, digits...>
        {
        };

        template <typename T>
        constexpr uintmax_t cabs(T num) { return (num < 0) ? -num : num; }
    }

    template <typename T, T num>
        struct string_from : detail::explode < num<0, detail::cabs(num)>
    {
    };
}

#endif