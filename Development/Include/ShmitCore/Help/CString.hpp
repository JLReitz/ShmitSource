#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

#include <cstdio>

namespace shmit
{

template<typename... ARGV>
size_t protected_snprintf(char* format, size_t max_length, ARGV... args)
{
    // Perform format
    long snprintf_return = std::snprintf(format, max_length, args...);

    // snprintf() returns the number of characters in the resulting string
    // If it is negative, an error occurred. Return 0 because nothing was printed.
    size_t bytes_written = 0;
    if (snprintf_return > 0)
        bytes_written = static_cast<size_t>(snprintf_return);

    return bytes_written;
}

} // namespace shmit