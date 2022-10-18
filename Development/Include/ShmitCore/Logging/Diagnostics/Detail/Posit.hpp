#pragma once

#include "DataPoint.hpp"

#include <ShmitCore/Logging/Defines.hpp>

#include <initializer_list>

namespace shmit
{
namespace log
{
namespace diagnostics
{

class Posit
{
public:
    virtual const char* GetTag() const   = 0;
    virtual Level       GetLevel() const = 0;

    virtual detail::DataPoint const* DataPoints() const     = 0;
    virtual size_t                   DataPointCount() const = 0;
};

namespace detail
{

template<size_t NumDataPoints, size_t ConstDataSize>
class Posit : public shmit::log::diagnostics::Posit
{
public:
    constexpr Posit(char const* tag, shmit::log::Level level, std::initializer_list<DataPointInit> data_points)
        : m_tag(tag), m_level(level), m_const_data(), m_data_points()
    {
        size_t const_data_index    = 0;
        size_t variable_data_index = 0;

        // Iterate through and initialize data points
        size_t data_point_index = 0;
        for (DataPointInit data_point_init : data_points)
        {
            DataPoint* data_point = &m_data_points[data_point_index];
            data_point->function  = data_point_init.function;

            size_t const_data_size = data_point_init.const_data.size();
            if (const_data_size > 0)
            {
                uint8_t* const_data_ptr = &m_const_data[const_data_index];
                data_point->const_data  = const_data_ptr;
                const_data_index += const_data_size;

                // Copy const data from initializer list to location in const database
                for (uint8_t data : data_point_init.const_data)
                    *const_data_ptr++ = data;
            }
            else
                data_point->const_data = nullptr;

            data_point->variable_data_size = data_point_init.variable_data_size;
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

    DataPoint const* DataPoints() const override
    {
        return m_data_points;
    }

    size_t DataPointCount() const override
    {
        return NumDataPoints;
    }

protected:
    const char* m_tag;

    shmit::log::Level m_level;

    uint8_t   m_const_data[ConstDataSize];
    DataPoint m_data_points[NumDataPoints];
};

template<size_t NumDataPoints, size_t ConstDataSize>
constexpr Posit<NumDataPoints, ConstDataSize> MakePosit(char const* name, Level level,
                                                        std::initializer_list<DataPointInit> data_points)
{
    return Posit<NumDataPoints, ConstDataSize>(name, level, data_points);
}

} // namespace detail
} // namespace diagnostics
} // namespace log
} // namespace shmit