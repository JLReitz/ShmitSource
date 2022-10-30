#pragma once

#include <ShmitCore/Logging/Diagnostics/Logging.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace detail
{

class Buffer : public Named
{
public:
    DIAGNOSTIC_CONTEXT(shmit::Buffer)
    DIAGNOSTIC_POSIT(constructing, shmit::log::Level::eDebug)
    DIAGNOSTIC_POSIT(destructing, shmit::log::Level::eDebug)

    DIAGNOSTIC_DATA_POSIT(at_capacity, shmit::log::Level::eDebug, shmit::log::diagnostics::Times())

    DIAGNOSTIC_DATA_POSIT(insert_front, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(insert_back, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(insert_random, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())

    DIAGNOSTIC_DATA_POSIT(emplace_back, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(emplace_front, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(emplace_random, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())

    DIAGNOSTIC_DATA_POSIT(pop_front, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(pop_back, shmit::log::Level::eTrace, shmit::log::diagnostics::Times())

    DIAGNOSTIC_DATA_POSIT(front_overflow, shmit::log::Level::eWarning, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(back_overflow, shmit::log::Level::eWarning, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(overfill, shmit::log::Level::eWarning, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(truncation, shmit::log::Level::eWarning, shmit::log::diagnostics::Times())

    using size_type       = size_t;
    using difference_type = size_t;

    Buffer() = default;
    constexpr Buffer(shmit::MemoryAddress start_address, size_t element_size, size_t max_element_count) noexcept;

    bool empty() const noexcept;
    bool full() const noexcept;

    size_t size() const noexcept;
    size_t max_size() const noexcept;

    void clear() noexcept;

    friend class BufferIterator;

protected:
    inline size_t RollIndexForward(size_t index, size_t steps) const noexcept
    {
        return (index + steps) % m_max_element_count;
    }

    inline size_t RollIndexBackward(size_t index, size_t steps) const noexcept
    {
        return (index < steps) ? m_max_element_count - (steps - index) : index - steps;
    }

    size_t IncrementIteratorBounded(BufferIterator& iterator, size_t steps) const;
    size_t IncrementIteratorBoundless(BufferIterator& iterator, size_t steps) const;
    size_t DecrementIteratorBounded(BufferIterator& iterator, size_t steps) const;
    size_t DecrementIteratorBoundless(BufferIterator& iterator, size_t steps) const;

    size_t WrapIndex(size_t index) const;
    size_t UnwrapPosition(size_t index) const;

    size_t PrepareForRandomPlacement(size_t start_index, size_t n) noexcept;

private:
    shmit::MemoryAddress m_start_address {nullptr};

    size_t m_front_index {0};
    size_t m_back_index {0};
    bool   m_is_full {false};

    const size_t m_element_size {0};
    size_t       m_max_element_count {0};
};

class BufferIterator
{
public:
    BufferIterator(Buffer& buffer) noexcept;
    BufferIterator(Buffer& buffer, size_t offset) noexcept;

    friend Buffer;

private:
    size_t  m_offset {0};
    Buffer& m_buffer;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer methods in alphabetical order        ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

constexpr Buffer::Buffer(shmit::MemoryAddress start_address, size_t element_size, size_t max_element_count)
    : m_start_address {start_address}, m_element_size {element_size}, m_max_element_count {max_element_count}
{
}

} // namespace detail
} // namespace shmit