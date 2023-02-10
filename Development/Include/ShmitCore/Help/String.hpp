#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

#include <algorithm>
#include <string_view>

#include <cctype>
#include <cstdarg>
#include <cstdio>

namespace shmit
{

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

} // namespace shmit