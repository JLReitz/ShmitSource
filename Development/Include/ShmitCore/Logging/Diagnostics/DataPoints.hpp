#pragma once

#include "Detail/DataPoint.hpp"

namespace shmit
{
namespace log
{
namespace diagnostics
{

//  Data point listings ================================================================================================

// The following functions are supplied to a diagnostic posit upon declaration

/// @brief Counts the number of times the posit has been logged since startup
/// @return The initialization struct for a Count data point
constexpr detail::DataPointInit Count();

/// @brief No additional data is logged with this data posit
/// @return The initialization struct for a nulled-out data point
constexpr detail::DataPointInit NoData();

//  End of data point listings  ========================================================================================
//  Data point Implementation details    ===============================================================================

//  Count   ------------------------------------------------------------------------------------------------------------

namespace detail
{

/// @brief Variable data struct for Count DataPoints
struct CountVariableData
{
    uint64_t count; //!< Rolling counter of posit occurences
};

/// @brief Count processing function
/// @param[in] const_data Start of const database for the DataPoint
/// @param[in] variable_data Start of variable database for the DataPoint
/// @param[out] out_str Results string
/// @param[in] str_length Maximum length of the output string
/// @return Number of characters written to the output string
size_t ProcessCount(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

} // namespace detail

constexpr detail::DataPointInit Count()
{
    return detail::DataPointInit {&detail::ProcessCount, sizeof(detail::CountVariableData), {}};
}

//  NoData  ------------------------------------------------------------------------------------------------------------

namespace detail
{

/// @brief NoData processing function
/// @param[in] const_data Start of const database for the DataPoint
/// @param[in] variable_data Start of variable database for the DataPoint
/// @param[out] out_str Results string
/// @param[in] str_length Maximum length of the output string
/// @return Number of characters written to the output string
size_t ProcessNoData(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

} // namespace detail

constexpr detail::DataPointInit NoData()
{
    return detail::DataPointInit {&detail::ProcessNoData, 0, {}};
}

} // namespace diagnostics
} // namespace log
} // namespace shmit