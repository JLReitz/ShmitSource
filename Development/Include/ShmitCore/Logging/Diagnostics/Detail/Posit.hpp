#pragma once

#include "DataPoint.hpp"

#include <ShmitCore/Logging/Defines.hpp>

#include <array>
#include <initializer_list>

namespace shmit
{
namespace log
{
namespace diagnostics
{

/// @brief Interface for a diagnostic posit
class Posit
{
public:
    /// @brief Fetches a string representation of the tag
    /// @return Const c-style string
    virtual const char* GetTag() const = 0;

    /// @brief Fetches the log level for this Posit
    /// @return Log level
    virtual Level GetLevel() const = 0;

    /// @brief Fetches the DataPoints held by this Posit
    /// @return Pointer to the start of a contiguous sequence of DataPoints in memory
    virtual DataPoint const* DataPoints() const = 0;

    /// @brief The number of data points held by this Posit
    /// @return Number of DataPoints
    virtual size_t DataPointCount() const = 0;
};

namespace detail
{

/// @brief Posit implementation generated by the diagnostic logging macros to be constructed at build time
/// @tparam NumDataPoints Number of DataPoints held by the Posit
/// @tparam ConstDataSize Size of const database
template<size_t NumDataPoints, size_t ConstDataSize>
class Posit : public shmit::log::diagnostics::Posit
{
public:
    static constexpr size_t kTagMaxLength = 16; //!< Max allowable length of the tag

    /// @brief Constexpr constructor
    /// @param tag String representation of the tag
    /// @param level Log level of the Posit
    /// @param data_points Initializer list of DataPoint initializer structs
    constexpr Posit(char const* tag, shmit::log::Level level, std::initializer_list<DataPointInit> data_points)
        : m_tag {0}, m_level(level), m_const_data(), m_data_points()
    {
        // Cache tag
        for (size_t i = 0; i < kTagMaxLength; i++)
        {
            m_tag[i] = tag[i];
            if (m_tag[i] == '\0')
                break;
        }

        // Iterate through and initialize data points
        size_t const_data_index    = 0;
        size_t variable_data_index = 0;
        size_t data_point_index    = 0;
        for (DataPointInit data_point_init : data_points)
        {
            shmit::log::diagnostics::DataPoint& data_point = m_data_points[data_point_index];
            data_point.function                            = data_point_init.function;

            size_t const_data_size = data_point_init.const_data.size();
            if (const_data_size > 0)
            {
                uint8_t* const_data_ptr = &m_const_data[const_data_index];
                data_point.const_data   = const_data_ptr;
                const_data_index += const_data_size;

                // Copy const data from initializer list to location in const database
                for (uint8_t data : data_point_init.const_data)
                    *const_data_ptr++ = data;
            }
            else
                data_point.const_data = nullptr;

            data_point.variable_data_size = data_point_init.variable_data_size;
            data_point_index += data_point_init.variable_data_size;
        }
    }

    const char* GetTag() const override
    {
        return m_tag;
    }

    Level GetLevel() const
    {
        return m_level;
    }

    shmit::log::diagnostics::DataPoint const* DataPoints() const override
    {
        return m_data_points.data();
    }

    size_t DataPointCount() const override
    {
        return NumDataPoints;
    }

private:
    char m_tag[kTagMaxLength + 1]; //!< String representation of the tag, terminated at the end

    shmit::log::Level m_level; //!< Posit log level

    std::array<uint8_t, ConstDataSize>                            m_const_data;  //!< Const database
    std::array<shmit::log::diagnostics::DataPoint, NumDataPoints> m_data_points; //!< Data points held by the posit
};

/// @brief Creates a Posit
/// @tparam NumDataPoints Number of DataPoints held by the Posit
/// @tparam ConstDataSize Size of const database
/// @param tag String representation of the tag
/// @param level Log level of the Posit
/// @param data_points Initializer list of DataPoint initializer structs
/// @return Constructed Posit
template<size_t NumDataPoints, size_t ConstDataSize>
constexpr Posit<NumDataPoints, ConstDataSize> MakePosit(char const* tag, Level level,
                                                        std::initializer_list<DataPointInit> data_points)
{
    return Posit<NumDataPoints, ConstDataSize>(tag, level, data_points);
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit