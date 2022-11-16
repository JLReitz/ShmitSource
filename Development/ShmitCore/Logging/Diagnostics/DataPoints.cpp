#include <ShmitCore/Logging/Diagnostics/DataPoints.hpp>

#include <ShmitCore/Help/CString.hpp>

namespace shmit
{
namespace log
{
namespace diagnostics
{
namespace detail
{

size_t ProcessTimes(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length)
{
    // Lay 'TimesVariableData' struct over variable data
    TimesVariableData* data = (TimesVariableData*)variable_data;
    data->times++;

    size_t print_size = protected_snprintf(out_str, str_length, "Times:%lu", data->times);
    return print_size;
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit