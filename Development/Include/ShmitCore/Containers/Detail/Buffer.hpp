#pragma once

#include <ShmitCore/Logging/Diagnostics.hpp>
#include <ShmitCore/Types/StdTypes.hpp>
#include <ShmitCore/Types/Traits/Named.hpp>

#include <iterator>
#include <limits>

namespace shmit
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer base class       ////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Base Buffer implementation. Has no knownledge of the value types it contains, it only handles raw memory.
 *
 */
class Buffer : public Named
{
public:
    using difference_type = int;
    using size_type       = size_t;

    static constexpr size_t kMaxAllowableSize = std::numeric_limits<difference_type>::max();

protected:
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Logging context and IDs         ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ESTABLISH_LOGGING_CONTEXT(Buffer)

    struct log_ids
    {
        static constexpr char const* kAssign = "assign";

        static constexpr char const* kInsertFront  = "insert-front";
        static constexpr char const* kInsertBack   = "insert-back";
        static constexpr char const* kInsertRandom = "insert-random";

        static constexpr char const* kEmplaceFront  = "emplace-front";
        static constexpr char const* kEmplaceBack   = "emplace-back";
        static constexpr char const* kEmplaceRandom = "emplace-random";

        static constexpr char const* kPopFront    = "pop-front";
        static constexpr char const* kPopBack     = "pop-back";
        static constexpr char const* kEraseFront  = "erase-front";
        static constexpr char const* kEraseBack   = "erase-back";
        static constexpr char const* kEraseRandom = "erase-random";

        static constexpr char const* kFrontOverflow = "front-overflow";
        static constexpr char const* kBackOverflow  = "back-overflow";
        static constexpr char const* kOverfill      = "overfill";
        static constexpr char const* kTruncation    = "truncation";
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //  Iterator base classes       ////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /**!
     * @brief Base standard iterator implementation. Like its container, it has no knowledge of the value types it
     * tracks.
     *
     */
    class Iterator
    {
    public:
        // DIAGNOSTIC_CONTEXT(Buffer::Iterator)
        // DIAGNOSTIC_POSIT(increment, shmit::log::Level::eTrace)
        // DIAGNOSTIC_POSIT(decrement, shmit::log::Level::eTrace)

        Iterator() = delete;
        Iterator(Iterator const& rhs) noexcept;

        bool operator>(Iterator const& rhs) const noexcept;
        bool operator>=(Iterator const& rhs) const noexcept;
        bool operator<(Iterator const& rhs) const noexcept;
        bool operator<=(Iterator const& rhs) const noexcept;

        bool operator==(Iterator const& rhs) const noexcept;
        bool operator!=(Iterator const& rhs) const noexcept;

        friend Buffer;
        friend class ConstIterator; // Forward declaration
        template<typename T>
        friend class BufferIteratorT; // Forward declaration

    protected:
        Iterator(Buffer& buffer) noexcept;
        Iterator(Buffer& buffer, size_type offset) noexcept;

        /**!
         * @brief Increases the Iterator's offset by the provided number of steps, bounded by the current size of its
         * tracked Buffer.
         *
         * @param steps Number of increments
         */
        void Increment(size_type steps);

        /**!
         * @brief Decreases the Iterator's offset by the provided number of steps. When it reaches the front, it will
         * wrap around to the back.
         *
         * @param steps Number of decrements
         */
        void Decrement(size_type steps);

        /**!
         * @brief Copies another Iterator
         *
         * @param rhs Iterator to copy
         */
        void Copy(Iterator const& rhs);

        size_type m_offset {0}; //!< Relative offset from front of buffer
        Buffer*   m_buffer;     //!< Pointer to tracked Buffer. No default initialization, must always be valid.
    };

    /**!
     * @brief Base constant iterator implementation. The iterator itself is not constant, but the Buffer elements it
     * tracks are. Raw memory and all as well... yada yada.
     */
    class ConstIterator
    {
    public:
        // DIAGNOSTIC_CONTEXT(Buffer::ConstIterator)
        // DIAGNOSTIC_POSIT(increment, shmit::log::Level::eTrace)
        // DIAGNOSTIC_POSIT(decrement, shmit::log::Level::eTrace)

        ConstIterator() = delete;

        ConstIterator(Iterator const& rhs) noexcept;
        ConstIterator(ConstIterator const& rhs) noexcept;

        bool operator>(ConstIterator const& rhs) const noexcept;
        bool operator>=(ConstIterator const& rhs) const noexcept;
        bool operator<(ConstIterator const& rhs) const noexcept;
        bool operator<=(ConstIterator const& rhs) const noexcept;

        bool operator==(ConstIterator const& rhs) const noexcept;
        bool operator!=(ConstIterator const& rhs) const noexcept;

        friend Buffer;

        template<typename T>
        friend class ConstBufferIteratorT; // Forward declaration

    protected:
        ConstIterator(Buffer const& buffer) noexcept;
        ConstIterator(Buffer const& buffer, size_type offset) noexcept;

        /**!
         * @brief Increases the ConstIterator's offset by the provided number of steps, bounded by the current size of
         * its tracked Buffer.
         *
         * @param steps Number of increments
         */
        void Increment(size_type steps);

        /**!
         * @brief Decreases the ConstIterator's offset by the provided number of steps. When it reaches the front, it
         * will wrap around to the back.
         *
         * @param steps Number of decrements
         */
        void Decrement(size_type steps);

        /**!
         * @brief Copies another ConstIterator
         *
         * @param rhs Iterator to copy
         */
        void Copy(Iterator const& rhs);

        size_type     m_offset {0}; //!< Relative offset from front of buffer
        Buffer const* m_buffer;     //!< Pointer to tracked Buffer. No default initialization, must always be valid.
    };

public:
    /**
     * @brief Default constructor is disabled
     *
     */
    Buffer() = delete;

    /**!
     * @brief Returns the empty state of the Buffer
     *
     * @retval true If the Buffer is empty
     * @retval false otherwise
     */
    bool empty() const noexcept;

    /**!
     * @brief Returns the full state of the Buffer
     *
     * @retval true If the Buffer is full
     * @retval false otherwise
     */
    bool full() const noexcept;

    /**!
     * @brief Returns the number of valid elements currently contained by the Buffer
     *
     * @return size_type Number of valid elements
     */
    size_type size() const noexcept;

    /**!
     * @brief Returns the max number of elements that the Buffer can contain at once
     *
     * @return size_type Max number of elements
     */
    size_type max_size() const noexcept;

    /**!
     * @brief Empties the buffer, does not destroy the contained elements
     *
     */
    void clear() noexcept;

    /**!
     * @brief Converts an absolute index to a position in reference to the front of the Buffer's current offset
     *
     * @param index Absolute index
     * @return size_type Wrapped position
     */
    size_type WrapIndex(size_type index) const;

    template<typename T>
    friend class BufferIteratorT; // Forward declaration
    template<typename T>
    friend class ConstBufferIteratorT; // Forward declaration

    friend Iterator;
    friend ConstIterator;

protected:
    Buffer(size_type element_size_bytes) noexcept;
    Buffer(char const* name, size_type element_size_bytes) noexcept;

    inline size_type RollIndexForward(size_type index, size_type steps) const noexcept
    {
        return (m_max_element_count) ? (index + steps) % m_max_element_count : index;
    }

    inline size_type RollIndexBackward(size_type index, size_type steps) const noexcept
    {
        return (m_max_element_count) ? ((index < steps) ? m_max_element_count - (steps - index) : index - steps) : index;
    }

    void          IncrementBounded(Iterator& iterator, size_type steps) const;
    void          IncrementBounded(ConstIterator& iterator, size_type steps) const;
    Iterator      IncrementBoundless(Iterator iterator, size_type steps) const;
    ConstIterator IncrementBoundless(ConstIterator iterator, size_type steps) const;

    void          DecrementBounded(Iterator& iterator, size_type steps) const;
    void          DecrementBounded(ConstIterator& iterator, size_type steps) const;
    Iterator      DecrementBoundless(Iterator iterator, size_type steps) const;
    ConstIterator DecrementBoundless(ConstIterator iterator, size_type steps) const;

    size_type UnwrapPosition(size_type index) const;

    void AtCapacity() noexcept;
    void ClearFullState() noexcept;

    size_type OverfillGuard(size_type count) const noexcept;

    Iterator ShiftContents(Iterator const& src, Iterator const& dest, size_type n);

    void PrepareForRandomPlacement(Iterator const& start, size_type n) noexcept;

    void PostInsertFront(Iterator insert_start);
    void PostInsertBack(Iterator insert_end);

    void MoveMemoryContents(Buffer&& rhs);

    void Swap(Buffer& rhs) noexcept;

    Iterator m_front {*this};
    Iterator m_back {*this};

    bool m_is_full {false};

    shmit::MemoryAddress m_start_address {nullptr};
    const size_type      m_element_size_bytes;
    size_type            m_max_element_count {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Templated Buffer iterator       ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class BufferIteratorT : public Buffer::Iterator
{
public:
    using difference_type = Buffer::difference_type;
    using size_type       = Buffer::size_type;

    using value_type = T;
    using pointer    = value_type*;
    using reference  = value_type&;

    using iterator_category = std::random_access_iterator_tag;

    BufferIteratorT(BufferIteratorT const& rhs) noexcept;
    BufferIteratorT(Buffer::Iterator const& rhs) noexcept;

    reference operator*() const noexcept;
    pointer   operator->() const noexcept;

    BufferIteratorT  operator++() noexcept;
    BufferIteratorT& operator++(int) noexcept;
    BufferIteratorT  operator--() noexcept;
    BufferIteratorT& operator--(int) noexcept;

    BufferIteratorT& operator+=(difference_type steps) noexcept;
    BufferIteratorT& operator-=(difference_type steps) noexcept;

    BufferIteratorT& operator=(BufferIteratorT const& rhs) noexcept;

    template<typename TArg>
    friend BufferIteratorT<TArg> operator+(BufferIteratorT<TArg> const& lhs, difference_type rhs) noexcept;

    template<typename TArg>
    friend BufferIteratorT<TArg> operator+(const difference_type lhs, BufferIteratorT<TArg> const& rhs) noexcept;

    template<typename TArg>
    friend difference_type operator-(BufferIteratorT<TArg> const& lhs, BufferIteratorT<TArg> const& rhs) noexcept;

    template<typename TArg>
    friend BufferIteratorT<TArg> operator-(BufferIteratorT<TArg> const& lhs, const difference_type rhs) noexcept;
};

template<typename T>
class ConstBufferIteratorT : public Buffer::ConstIterator
{
public:
    using difference_type = Buffer::difference_type;
    using size_type       = Buffer::size_type;

    using value_type = T const;
    using pointer    = value_type*;
    using reference  = value_type&;

    using iterator_category = std::random_access_iterator_tag;

    ConstBufferIteratorT(ConstBufferIteratorT const& rhs) noexcept;
    ConstBufferIteratorT(Buffer::Iterator const& rhs) noexcept;

    reference operator*() const noexcept;
    pointer   operator->() const noexcept;

    ConstBufferIteratorT  operator++() noexcept;
    ConstBufferIteratorT& operator++(int) noexcept;
    ConstBufferIteratorT  operator--() noexcept;
    ConstBufferIteratorT& operator--(int) noexcept;

    ConstBufferIteratorT& operator+=(difference_type steps) noexcept;
    ConstBufferIteratorT& operator-=(difference_type steps) noexcept;

    ConstBufferIteratorT& operator=(ConstBufferIteratorT const& rhs) noexcept;

    template<typename TArg>
    friend ConstBufferIteratorT<TArg> operator+(ConstBufferIteratorT<TArg> const& lhs, difference_type rhs) noexcept;

    template<typename TArg>
    friend ConstBufferIteratorT<TArg> operator+(const difference_type lhs, ConstBufferIteratorT<TArg> const& rhs) noexcept;

    template<typename TArg>
    friend difference_type operator-(ConstBufferIteratorT<TArg> const& lhs, ConstBufferIteratorT<TArg> const& rhs) noexcept;

    template<typename TArg>
    friend ConstBufferIteratorT<TArg> operator-(ConstBufferIteratorT<TArg> const& lhs, const difference_type rhs) noexcept;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BufferIteratorT constructor definitions         ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
BufferIteratorT<T>::BufferIteratorT(BufferIteratorT<T> const& rhs) noexcept
    : Buffer::Iterator(*rhs.m_buffer, rhs.m_offset)
{
}

template<typename T>
BufferIteratorT<T>::BufferIteratorT(Buffer::Iterator const& rhs) noexcept : Buffer::Iterator(rhs)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BufferIteratorT method definitions in alphabetical order        ////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
typename BufferIteratorT<T>::reference BufferIteratorT<T>::operator*() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    pointer element_ptr = static_cast<pointer>(m_buffer->m_start_address) + m_offset;
    return *element_ptr;
}

template<typename T>
typename BufferIteratorT<T>::pointer BufferIteratorT<T>::operator->() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    pointer element_ptr = static_cast<pointer>(m_buffer->m_start_address) + m_offset;
    return element_ptr;
}

template<typename T>
BufferIteratorT<T> BufferIteratorT<T>::operator++() noexcept
{
    BufferIteratorT<T> tmp {*this};
    Increment(1);
    return tmp;
}

template<typename T>
BufferIteratorT<T>& BufferIteratorT<T>::operator++(int) noexcept
{
    Increment(1);
    return *this;
}

template<typename T>
BufferIteratorT<T> BufferIteratorT<T>::operator--() noexcept
{
    BufferIteratorT<T> tmp {*this};
    Decrement(1);
    return tmp;
}

template<typename T>
BufferIteratorT<T>& BufferIteratorT<T>::operator--(int) noexcept
{
    Decrement(1);
    return *this;
}

template<typename T>
BufferIteratorT<T>& BufferIteratorT<T>::operator+=(difference_type steps) noexcept
{
    if (steps > 0)
        Increment((unsigned)steps);
    else
        Decrement((unsigned)(steps * -1));

    return *this;
}

template<typename T>
BufferIteratorT<T>& BufferIteratorT<T>::operator-=(difference_type steps) noexcept
{
    if (steps > 0)
        Decrement((unsigned)steps);
    else
        Increment((unsigned)(steps * -1));

    return *this;
}

template<typename T>
BufferIteratorT<T>& BufferIteratorT<T>::operator=(BufferIteratorT<T> const& rhs) noexcept
{
    Copy(rhs);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ConstBufferIteratorT constructor definitions        ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
ConstBufferIteratorT<T>::ConstBufferIteratorT(ConstBufferIteratorT<T> const& rhs) noexcept
    : Buffer::ConstIterator(*rhs.m_buffer, rhs.m_offset)
{
}

template<typename T>
ConstBufferIteratorT<T>::ConstBufferIteratorT(Buffer::Iterator const& rhs) noexcept : Buffer::ConstIterator(rhs)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ConstBufferIteratorT methods in alphabetical order      ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename T>
typename ConstBufferIteratorT<T>::reference ConstBufferIteratorT<T>::operator*() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    pointer element_ptr = static_cast<pointer>(m_buffer->m_start_address) + m_offset;
    return *element_ptr;
}

template<typename T>
typename ConstBufferIteratorT<T>::pointer ConstBufferIteratorT<T>::operator->() const noexcept
{
    // Need to typecast buffer start address from void* before performing pointer arithmetic
    pointer element_ptr = static_cast<pointer>(m_buffer->m_start_address) + m_offset;
    return element_ptr;
}

template<typename T>
ConstBufferIteratorT<T> ConstBufferIteratorT<T>::operator++() noexcept
{
    ConstBufferIteratorT<T> tmp {*this};
    Increment(1);
    return tmp;
}

template<typename T>
ConstBufferIteratorT<T>& ConstBufferIteratorT<T>::operator++(int) noexcept
{
    Increment(1);
    return *this;
}

template<typename T>
ConstBufferIteratorT<T> ConstBufferIteratorT<T>::operator--() noexcept
{
    ConstBufferIteratorT<T> tmp {*this};
    Decrement(1);
    return tmp;
}

template<typename T>
ConstBufferIteratorT<T>& ConstBufferIteratorT<T>::operator--(int) noexcept
{
    Decrement(1);
    return *this;
}

template<typename T>
ConstBufferIteratorT<T>& ConstBufferIteratorT<T>::operator+=(difference_type steps) noexcept
{
    if (steps > 0)
        Increment((unsigned)steps);
    else
        Decrement((unsigned)(steps * -1));

    return *this;
}

template<typename T>
ConstBufferIteratorT<T>& ConstBufferIteratorT<T>::operator-=(difference_type steps) noexcept
{
    if (steps > 0)
        Decrement((unsigned)steps);
    else
        Increment((unsigned)(steps * -1));

    return *this;
}

template<typename T>
ConstBufferIteratorT<T>& ConstBufferIteratorT<T>::operator=(ConstBufferIteratorT<T> const& rhs) noexcept
{
    Copy(rhs);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  External operator overload definitions          ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename TArg>
BufferIteratorT<TArg> operator+(BufferIteratorT<TArg> const& lhs, Buffer::difference_type rhs) noexcept
{
    BufferIteratorT tmp = lhs;
    tmp += rhs;
    return tmp;
}

template<typename TArg>
BufferIteratorT<TArg> operator+(const Buffer::difference_type lhs, BufferIteratorT<TArg> const& rhs) noexcept
{
    BufferIteratorT tmp = rhs;
    tmp += lhs;
    return tmp;
}

template<typename TArg>
Buffer::difference_type operator-(BufferIteratorT<TArg> const& lhs, BufferIteratorT<TArg> const& rhs) noexcept
{
    if (lhs < rhs)
        return 0;

    Buffer::difference_type diff = lhs.m_buffer->WrapIndex(lhs.m_offset) - rhs.m_buffer->WrapIndex(rhs.m_offset);
    return diff;
}

template<typename TArg>
BufferIteratorT<TArg> operator-(BufferIteratorT<TArg> const& lhs, const Buffer::difference_type rhs) noexcept
{
    BufferIteratorT tmp = lhs;
    tmp -= rhs;
    return tmp;
}

template<typename TArg>
ConstBufferIteratorT<TArg> operator+(ConstBufferIteratorT<TArg> const& lhs, Buffer::difference_type rhs) noexcept
{
    ConstBufferIteratorT tmp = lhs;
    tmp += rhs;
    return tmp;
}

template<typename TArg>
ConstBufferIteratorT<TArg> operator+(const Buffer::difference_type lhs, ConstBufferIteratorT<TArg> const& rhs) noexcept
{
    ConstBufferIteratorT tmp = rhs;
    tmp += lhs;
    return tmp;
}

template<typename TArg>
Buffer::difference_type operator-(ConstBufferIteratorT<TArg> const& lhs, ConstBufferIteratorT<TArg> const& rhs) noexcept
{
    if (lhs < rhs)
        return 0;

    Buffer::difference_type diff = (signed)(lhs.m_buffer->WrapIndex(lhs.m_offset) - rhs.m_buffer->WrapIndex(rhs.m_offset));
    return diff;
}

template<typename TArg>
ConstBufferIteratorT<TArg> operator-(ConstBufferIteratorT<TArg> const& lhs, const Buffer::difference_type rhs) noexcept
{
    ConstBufferIteratorT tmp = lhs;
    tmp -= rhs;
    return tmp;
}

} // namespace detail
} // namespace shmit