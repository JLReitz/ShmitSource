#pragma once

#include "Detail/DataPoint.hpp"

namespace shmit
{
namespace log
{
namespace diagnostics
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Data point listings         ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// The following functions are supplied to a diagnostic posit upon declaration

/// @brief Tracks the number of times the posit has been logged since startup
/// @return The initialization struct for a Count data point
constexpr detail::DataPointInit Times();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  End of data point listings         /////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Detail      ////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Count   ------------------------------------------------------------------------------------------------------------

namespace detail
{

/// @brief Variable data struct for Count DataPoints
struct TimesVariableData
{
    uint64_t times; //!< Rolling counter of posit occurences
};

/// @brief Processing function for Times DataPoint
/// @param[in] const_data Start of const database for the DataPoint
/// @param[in] variable_data Start of variable database for the DataPoint
/// @param[out] out_str Results string
/// @param[in] str_length Maximum length of the output string
/// @return Number of characters written to the output string
size_t ProcessTimes(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

} // namespace detail

constexpr detail::DataPointInit Times()
{
    return detail::DataPointInit {&detail::ProcessTimes, sizeof(detail::TimesVariableData), {}};
}

} // namespace diagnostics
} // namespace log
} // namespace shmit