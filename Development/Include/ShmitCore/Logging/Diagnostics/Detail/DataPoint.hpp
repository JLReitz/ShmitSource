#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <initializer_list>

namespace shmit
{
namespace log
{
namespace diagnostics
{

/// @brief Processes a DataPoint and writes the output to a string
/// @param[in] const_data Start of const database for the DataPoint
/// @param[in] variable_data Start of variable database for the DataPoint
/// @param[out] out_str Results string
/// @param[in] str_length Maximum length of the output string
/// @return Number of characters written to the output string
typedef size_t (*DataPointProcess)(uint8_t const* const_data, uint8_t* variable_data, char* out_str, size_t str_length);

/// @brief Represents a singular point of measurement to be recorded and logged during a diagnostic Posit
struct DataPoint
{
    DataPointProcess function; //!< Processing function

    uint8_t const* const_data;         //!< Start of const database
    size_t         variable_data_size; //!< Size of variable data in variable database
};

namespace detail
{

/// @brief Used by a Posit to initialize a DataPoint
struct DataPointInit
{
    DataPointProcess function; //!< Processing function

    size_t                         variable_data_size; //!< Size of variable data in variable database
    std::initializer_list<uint8_t> const_data;         //!< Const data used by 'function'
};

/// @brief Counts the DataPoints provided in a set
/// @param data_points DataPoint set
/// @return Number of DataPoints
constexpr size_t CountDataPoints(std::initializer_list<DataPointInit> data_points)
{
    return data_points.size();
}

/// @brief Calculates the required const database size for a given set of DataPoints
/// @param data_points DataPoint set
/// @return Size of const database
constexpr size_t ConstDatabaseSize(std::initializer_list<DataPointInit> data_points)
{
    size_t size = 0;
    for (DataPointInit data_point : data_points)
    {
        size += data_point.const_data.size();
    }

    return size;
}

/// @brief Calculates the required variable database size for a given set of DataPoints
/// @param data_points DataPoint set
/// @return Size of variable database
constexpr size_t VariableDatabaseSize(std::initializer_list<DataPointInit> data_points)
{
    size_t size = 0;
    for (DataPointInit data_point : data_points)
    {
        size += data_point.variable_data_size;
    }

    return size;
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit