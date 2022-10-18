#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <initializer_list>

namespace shmit
{
namespace log
{
namespace diagnostics
{
namespace detail
{

typedef size_t (*Function)(uint8_t*, uint8_t*, char*, size_t);

struct DataPoint
{
    Function function; // Function which calculates the data point

    uint8_t* const_data; // Address of data in const database
    size_t   variable_data_size;
};

struct DataPointInit
{
    Function function; // Function which calculates the data point

    size_t                         variable_data_size; // Amount of variable data used by the function
    std::initializer_list<uint8_t> const_data;         // Const data used by the function
};

constexpr size_t CountDataPoints(std::initializer_list<DataPointInit> data_points)
{
    return data_points.size();
}

constexpr size_t ConstDatabaseSize(std::initializer_list<DataPointInit> data_points)
{
    size_t size = 0;
    for (DataPointInit data_point : data_points)
    {
        size += data_point.const_data.size();
    }

    // If size == 0, return 1
    return (size > 0) ? size : 1;
}

constexpr size_t VariableDatabaseSize(std::initializer_list<DataPointInit> data_points)
{
    size_t size = 0;
    for (DataPointInit data_point : data_points)
    {
        size += data_point.variable_data_size;
    }

    // If size == 0, return 1
    return (size > 0) ? size : 1;
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit