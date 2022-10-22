#include <ShmitCore/Logging/Diagnostics/DataPoints.hpp>

#include <cstdio>

namespace shmit
{
namespace log
{
namespace diagnostics
{
namespace detail
{

size_t ProcessCount(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length)
{
    // Lay 'CountVariableData' struct over variable data
    CountVariableData* data = (CountVariableData*)variable_data;
    data->count++;

    size_t print_size = snprintf(out_str, str_length, "Count: %lu", data->count);
    return print_size;
}

size_t ProcessNoData(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length)
{
    (void)const_data;    // Unused
    (void)variable_data; // Unused
    (void)out_str;       // Unused
    (void)str_length;    // Unused

    // Do nothing, return 0
    return 0;
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit