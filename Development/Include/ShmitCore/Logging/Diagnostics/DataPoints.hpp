#pragma once

#include "Detail/DataPoint.hpp"

namespace shmit
{
namespace log
{
namespace diagnostics
{

//  Data point listings ================================================================================================

constexpr detail::DataPointInit Count();
constexpr detail::DataPointInit NoData();

//  Data point Implementation details    ===============================================================================

//  Count() ============================================================================================================

namespace detail
{

struct CountVariableData
{
    uint64_t count;
};

size_t ExecuteCount(uint8_t* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

} // namespace detail

constexpr detail::DataPointInit Count()
{
    return detail::DataPointInit {&detail::ExecuteCount, sizeof(detail::CountVariableData), {}};
}

//  NoData()    ========================================================================================================

namespace detail
{

size_t ExecuteNoData(uint8_t* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

} // namespace detail

constexpr detail::DataPointInit NoData()
{
    return detail::DataPointInit {&detail::ExecuteNoData, 0, {}};
}

//  End of data points  ================================================================================================

} // namespace diagnostics
} // namespace log
} // namespace shmit