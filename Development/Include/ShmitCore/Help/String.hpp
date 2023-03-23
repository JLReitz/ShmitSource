#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

#include <algorithm>
#include <string_view>

#include <cctype>
#include <charconv>
#include <climits>
#include <cstdarg>
#include <cstdio>

namespace shmit
{

template<typename T, typename = void>
struct max_digits_for_type;

template<typename T>
size_t print(char* const str, size_t str_length, T const& value);

inline constexpr size_t strlen(char const* str)
{
    size_t count = 0;
    while (*str++ != '\0')
        count++;

    return count;
}

inline constexpr size_t strnlen(char const* str, size_t n)
{
    size_t count = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (*str++ == '\0')
            break;

        count++;
    }

    return count;
}

/**
 * @brief
 * @note Cannot be constexpr until C++20 support is available across all platforms
 *
 * @param str
 * @return true
 * @return false
 */
inline bool is_alpha(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isalpha(c); };
    return std::all_of(str.begin(), str.end(), check);
}

/**
 * @brief
 * @note Cannot be constexpr until C++20 support is available across all platforms
 *
 * @param str
 * @return true
 * @return false
 */
inline bool is_alphanumeric(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isalnum(c); };
    return std::all_of(str.begin(), str.end(), check);
}

/**
 * @brief
 * @note Cannot be constexpr until C++20 support is available across all platforms
 *
 * @param str
 * @return true
 * @return false
 */
inline bool is_digit(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isdigit(c); };
    return std::all_of(str.begin(), str.end(), check);
}

template<typename... ARGV>
inline size_t protected_snprintf(char* out_str, size_t max_length, char const* format, ARGV... args)
{
    // Perform format
    long snprintf_return = std::snprintf(out_str, max_length, format, args...);

    // snprintf() returns the number of characters in the resulting string
    // If it is negative, an error occurred. Return 0 because nothing was printed.
    size_t bytes_written = 0;
    if (snprintf_return > 0)
        bytes_written = static_cast<size_t>(snprintf_return);

    return bytes_written;
}

inline size_t protected_vsnprintf(char* out_str, size_t max_length, char const* format, std::va_list& args)
{
    // Perform format
    long vsnprintf_return = std::vsnprintf(out_str, max_length, format, args);

    // snprintf() returns the number of characters in the resulting string
    // If it is negative, an error occurred. Return 0 because nothing was printed.
    size_t bytes_written = 0;
    if (vsnprintf_return > 0)
        bytes_written = static_cast<size_t>(vsnprintf_return);

    return bytes_written;
}

namespace detail
{

template<typename T>
struct common_max_digits_for_type
{
    static constexpr size_t binary {CHAR_BIT * sizeof(T)};  // Size of this type in chars times char bitsize
    static constexpr size_t octal {(binary + 2) / 3};       // Binary digits divided by 3, rounded up
    static constexpr size_t hexadecimal {(binary + 3) / 4}; // Binary digits divided by 4, rounded up

    static constexpr size_t kNumSignDigits {(std::is_signed_v<T>) ? 1 : 0}; // '-' or '+'
};

template<typename T, std::enable_if_t<std::is_integral_v<T>>* = nullptr>
inline std::to_chars_result print(char* const str, size_t str_max_length, T value)
{
    std::to_chars_result result {str, std::errc()};
    if constexpr (std::is_same_v<char, std::remove_cv_t<T>> || std::is_same_v<bool, std::remove_cv_t<T>>)
    {
        char value_char = value;
        if constexpr (std::is_same_v<bool, std::remove_cv_t<T>>)
        {
            if (value)
                value_char = 'T';
            else
                value_char = 'F';
        }

        if (str_max_length >= 1)
        {
            *str       = value_char;
            result.ptr = str + 1;
        }
        else
            result.ec = std::errc::value_too_large;
    }
    else
        result = std::to_chars(str, str + str_max_length, value, 10);

    return result;
}

template<typename T, std::enable_if_t<std::is_floating_point_v<T>>* = nullptr>
inline std::to_chars_result print(char* const str, size_t str_max_length, T const& value)
{
    constexpr int kFloatingPrecision = std::numeric_limits<T>::max_digits10;
    int           printf_result      = std::snprintf(str, str_max_length, "%.*f", kFloatingPrecision, value);

    if (printf_result >= 0)
        return std::to_chars_result {str + (printf_result - 1), std::errc()};
    else
        return std::to_chars_result {str, std::errc::value_too_large};
}

template<typename T, std::enable_if_t<std::is_pointer_v<T>>* = nullptr>
inline std::to_chars_result print(char* const str, size_t str_max_length, T const& value)
{
    uintptr_t ptr_value {static_cast<uintptr_t>(value)};
    return std::to_chars(str, str + str_max_length, ptr_value, 10);
}

} // namespace detail

template<typename T>
struct max_digits_for_type<T, std::enable_if_t<std::is_integral_v<T>>> : public detail::common_max_digits_for_type<T>
{
private:
    static constexpr bool   kIsBoolOrChar = (std::is_same_v<T, bool> || std::is_same_v<T, char>);
    static constexpr size_t kNumBoolandCharacterDigits {1}; // Bool can be 'T' or 'F' and a character is singular

public:
    static constexpr size_t decimal {std::numeric_limits<T>::digits10 +
                                     detail::common_max_digits_for_type<T>::kNumSignDigits};

    static constexpr size_t value {kIsBoolOrChar ? kNumBoolandCharacterDigits : decimal};
};

template<typename T>
struct max_digits_for_type<T, std::enable_if_t<std::is_floating_point_v<T>>> : public detail::common_max_digits_for_type<T>
{
private:
    static constexpr size_t kNumFloatFormatDigits {2}; // '.' and 'e'

public:
    static constexpr size_t decimal {std::numeric_limits<T>::max_digits10 + kNumFloatFormatDigits};

    static constexpr size_t value {decimal};
};

template<typename T>
struct max_digits_for_type<T, std::enable_if_t<std::is_pointer_v<T>>> : public max_digits_for_type<uintptr_t>
{
};

template<typename T>
inline size_t print(char* const str, size_t str_length, T const& value)
{
    std::to_chars_result result = detail::print(str, str_length, value);

    std::ptrdiff_t print_length = 0;
    if (result.ec == std::errc())
        print_length = result.ptr - str;

    return static_cast<size_t>(print_length);
}

} // namespace shmit