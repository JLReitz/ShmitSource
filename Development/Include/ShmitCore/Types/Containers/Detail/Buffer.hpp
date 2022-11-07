#pragma once

#include <ShmitCore/Logging/Diagnostics/Logging.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer iterator base class          ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer base class       ////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Buffer : public Named
{
public:
    using difference_type = size_t;
    using size_type       = size_t;

protected:
    class BufferIterator
    {
    public:
        DIAGNOSTIC_CONTEXT(BufferIterator)

        DIAGNOSTIC_POSIT(increment, shmit::log::Level::eTrace)
        DIAGNOSTIC_POSIT(decrement, shmit::log::Level::eTrace)

        using iterator_category = std::random_access_iterator_tag;

        BufferIterator() = delete;

        BufferIterator(BufferIterator const& rhs) noexcept;

        BufferIterator operator++() noexcept;
        BufferIterator operator++(int) noexcept;
        BufferIterator operator--() noexcept;
        BufferIterator operator--(int) noexcept;

        BufferIterator operator+=(difference_type steps) noexcept;
        BufferIterator operator-=(difference_type steps) noexcept;

        BufferIterator operator=(BufferIterator const& rhs) noexcept;

        bool operator>(BufferIterator const& rhs) const noexcept;
        bool operator>=(BufferIterator const& rhs) const noexcept;
        bool operator<(BufferIterator const& rhs) const noexcept;
        bool operator<=(BufferIterator const& rhs) const noexcept;

        bool operator==(BufferIterator const& rhs) const noexcept;
        bool operator!=(BufferIterator const& rhs) const noexcept;

        friend Buffer;
        friend class ConstBufferIterator; // Forward declaration

        template<typename T>
        friend class BufferIteratorT; // Forward declaration

        friend BufferIterator  operator+(BufferIterator const& lhs, difference_type rhs) noexcept;
        friend BufferIterator  operator+(const difference_type lhs, BufferIterator const& rhs) noexcept;
        friend difference_type operator-(BufferIterator const& lhs, BufferIterator const& rhs) noexcept;
        friend BufferIterator  operator-(BufferIterator const& lhs, const difference_type rhs) noexcept;

    protected:
        BufferIterator(Buffer& buffer) noexcept;
        BufferIterator(Buffer& buffer, size_type offset) noexcept;

        size_type m_offset {0};
        Buffer*   m_buffer; // No default initialization, pointer must always be valid
    };

    class ConstBufferIterator
    {
    public:
        ConstBufferIterator() = delete;

        ConstBufferIterator(BufferIterator const& rhs) noexcept;
        ConstBufferIterator(ConstBufferIterator const& rhs) noexcept;

        ConstBufferIterator operator++() noexcept;
        ConstBufferIterator operator++(int) noexcept;
        ConstBufferIterator operator--() noexcept;
        ConstBufferIterator operator--(int) noexcept;

        ConstBufferIterator operator+=(difference_type steps) noexcept;
        ConstBufferIterator operator-=(difference_type steps) noexcept;

        ConstBufferIterator operator=(ConstBufferIterator const& rhs) noexcept;

        bool operator>(ConstBufferIterator const& rhs) const noexcept;
        bool operator>=(ConstBufferIterator const& rhs) const noexcept;
        bool operator<(ConstBufferIterator const& rhs) const noexcept;
        bool operator<=(ConstBufferIterator const& rhs) const noexcept;

        bool operator==(ConstBufferIterator const& rhs) const noexcept;
        bool operator!=(ConstBufferIterator const& rhs) const noexcept;

        friend Buffer;

        template<typename T>
        friend class ConstBufferIteratorT; // Forward declaration

        friend ConstBufferIterator operator+(ConstBufferIterator const& lhs, difference_type rhs) noexcept;
        friend ConstBufferIterator operator+(const difference_type lhs, ConstBufferIterator const& rhs) noexcept;
        friend difference_type     operator-(ConstBufferIterator const& lhs, ConstBufferIterator const& rhs) noexcept;
        friend ConstBufferIterator operator-(ConstBufferIterator const& lhs, const difference_type rhs) noexcept;

    protected:
        ConstBufferIterator(Buffer const& buffer) noexcept;
        ConstBufferIterator(Buffer const& buffer, size_type offset) noexcept;

        size_type     m_offset {0};
        Buffer const* m_buffer;
    };

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
    DIAGNOSTIC_DATA_POSIT(bad_alloc, shmit::log::Level::eError, shmit::log::diagnostics::Times())
    DIAGNOSTIC_DATA_POSIT(bad_move, shmit::log::Level::eError, shmit::log::diagnostics::Times())

    DIAGNOSTIC_POSIT(increment_iterator, shmit::log::Level::eTrace)

    Buffer() = delete;

    bool empty() const noexcept;
    bool full() const noexcept;

    size_type size() const noexcept;
    size_type max_size() const noexcept;

    void clear() noexcept;

    template<typename T>
    friend class BufferIteratorT;
    template<typename T>
    friend class ConstBufferIteratorT;

    friend BufferIterator;
    friend ConstBufferIterator;

    friend BufferIterator  operator+(BufferIterator const& lhs, difference_type rhs) noexcept;
    friend BufferIterator  operator+(const difference_type lhs, BufferIterator const& rhs) noexcept;
    friend difference_type operator-(BufferIterator const& lhs, BufferIterator const& rhs) noexcept;
    friend BufferIterator  operator-(BufferIterator const& lhs, const difference_type rhs) noexcept;

    friend ConstBufferIterator operator+(ConstBufferIterator const& lhs, difference_type rhs) noexcept;
    friend ConstBufferIterator operator+(const difference_type lhs, ConstBufferIterator const& rhs) noexcept;
    friend difference_type     operator-(ConstBufferIterator const& lhs, ConstBufferIterator const& rhs) noexcept;
    friend ConstBufferIterator operator-(ConstBufferIterator const& lhs, const difference_type rhs) noexcept;

protected:
    Buffer(size_type element_size_bytes) noexcept;
    Buffer(char const* name, size_type element_size_bytes) noexcept;

    inline size_type RollIndexForward(size_type index, size_type steps) const noexcept
    {
        return (m_max_element_count) ? (index + steps) % m_max_element_count : index;
    }

    inline size_type RollIndexBackward(size_type index, size_type steps) const noexcept
    {
        return (m_max_element_count) ? ((index < steps) ? m_max_element_count - (steps - index) : index - steps) :
                                       index;
    }

    BufferIterator      IncrementBounded(BufferIterator iterator, size_type steps) const;
    ConstBufferIterator IncrementBounded(ConstBufferIterator iterator, size_type steps) const;
    BufferIterator      IncrementBoundless(BufferIterator iterator, size_type steps) const;
    ConstBufferIterator IncrementBoundless(ConstBufferIterator iterator, size_type steps) const;
    BufferIterator      DecrementBounded(BufferIterator iterator, size_type steps) const;
    ConstBufferIterator DecrementBounded(ConstBufferIterator iterator, size_type steps) const;
    BufferIterator      DecrementBoundless(BufferIterator iterator, size_type steps) const;
    ConstBufferIterator DecrementBoundless(ConstBufferIterator iterator, size_type steps) const;

    size_type WrapIndex(size_type index) const;
    size_type UnwrapPosition(size_type index) const;

    void      AtCapacity() noexcept;
    size_type OverfillGuard(size_type count) const noexcept;

    void PrepareForRandomPlacement(BufferIterator const& start, size_type n) noexcept;

    void PostInsertFront(BufferIterator insert_start);
    void PostInsertBack(BufferIterator insert_end);

    void MoveMemoryContents(Buffer&& rhs);

    void Swap(Buffer& rhs) noexcept;

    BufferIterator m_front {*this};
    BufferIterator m_back {*this};

    bool m_is_full {false};

    shmit::MemoryAddress m_start_address {nullptr};
    const size_type      m_element_size_bytes;
    size_type            m_max_element_count {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Templated Buffer iterator       ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class BufferIteratorT : public Buffer::BufferIterator
{
public:
    using difference_type = Buffer::difference_type;
    using size_type       = Buffer::size_type;

    using value_type = T;
    using pointer    = T*;
    using reference  = T&;

    BufferIteratorT(BufferIteratorT const& rhs) noexcept;
    BufferIteratorT(Buffer::BufferIterator const& rhs) noexcept;

    reference operator*() const noexcept;
    pointer   operator->() const noexcept;
};

template<typename T>
class ConstBufferIteratorT : public Buffer::ConstBufferIterator
{
public:
    using difference_type = Buffer::difference_type;
    using size_type       = Buffer::size_type;

    using value_type = T const;
    using pointer    = T const*;
    using reference  = T const&;

    ConstBufferIteratorT(ConstBufferIteratorT const& rhs) noexcept;
    ConstBufferIteratorT(Buffer::BufferIterator const& rhs) noexcept;

    reference operator*() const noexcept;
    pointer   operator->() const noexcept;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BufferIteratorT methods in alphabetical order       ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
BufferIteratorT<T>::BufferIteratorT(BufferIteratorT<T> const& rhs) noexcept
    : Buffer::BufferIterator(*rhs.m_buffer, rhs.m_offset)
{
}

template<typename T>
BufferIteratorT<T>::BufferIteratorT(Buffer::BufferIterator const& rhs) noexcept : Buffer::BufferIterator(rhs)
{
}

template<typename T>
typename BufferIteratorT<T>::reference BufferIteratorT<T>::operator*() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    T* buffer_start = static_cast<T*>(m_buffer->m_start_address);
    return *(buffer_start + m_offset);
}

template<typename T>
typename BufferIteratorT<T>::pointer BufferIteratorT<T>::operator->() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    T* buffer_start = static_cast<T*>(m_buffer->m_start_address);
    return *(buffer_start + m_offset);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ConstBufferIteratorT methods in alphabetical order      ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
ConstBufferIteratorT<T>::ConstBufferIteratorT(ConstBufferIteratorT<T> const& rhs) noexcept
    : Buffer::ConstBufferIterator(*rhs.m_buffer, rhs.m_offset)
{
}

template<typename T>
ConstBufferIteratorT<T>::ConstBufferIteratorT(Buffer::BufferIterator const& rhs) noexcept
    : Buffer::ConstBufferIterator(rhs)
{
}

template<typename T>
typename ConstBufferIteratorT<T>::reference ConstBufferIteratorT<T>::operator*() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    T* buffer_start = static_cast<T*>(m_buffer->m_start_address);
    return *(buffer_start + m_offset);
}

template<typename T>
typename ConstBufferIteratorT<T>::pointer ConstBufferIteratorT<T>::operator->() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    T* buffer_start = static_cast<T*>(m_buffer->m_start_address);
    return *(buffer_start + m_offset);
}

} // namespace detail
} // namespace shmit