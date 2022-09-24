#pragma once

#include <ShmitCore/Types/StdTypes.hpp>

#include <initializer_list>
#include <iterator>
#include <memory>

// TODO remove
#include <iostream>

namespace shmit
{
namespace impl
{

class BufferBase
{
public:
    using size_type       = size_t;
    using difference_type = size_t;

protected:
    size_t               mBufferMaxSize;
    shmit::MemoryAddress mStartAddress;

    size_t mFrontOfBuffer; // First poppable index
    size_t mBackOfBuffer;  // Last poppable index

    bool mIsFullFlag; // Flag to signal full condition

    size_t _M_roll_over_forward(size_t index, size_t steps) const
    {
        return ((index + steps) % mBufferMaxSize);
    }

    size_t _M_roll_over_backward(size_t index, size_t steps) const
    {
        return ((index < steps) ? mBufferMaxSize - (steps - index) : index - steps);
    }

public:
    BufferBase() noexcept
        : mStartAddress(nullptr), mBufferMaxSize(0), mFrontOfBuffer(0), mBackOfBuffer(0), mIsFullFlag(false)
    {
    }

    size_t max_size() const noexcept
    {
        return mBufferMaxSize;
    }

    shmit::MemoryAddress _M_start_addr() const noexcept
    {
        return mStartAddress;
    }

    size_t _M_move_index_forward(size_t index, size_t steps) const noexcept
    {
        // Guard against steps == 0
        if (steps == 0)
            return index;

        // If index points at the end of buffer already, do not increment.
        // If it points to the back of the buffer, increment to end of buffer.
        // If the new index points to the back of the buffer but the buffer is not full, increment to end of buffer.
        // If the new index would point past the back of the buffer, increment to end of buffer.
        // Otherwise increment normally while accounting for wrap-around.
        size_t newIndex = _M_roll_over_forward(index, steps);
        // std::cout << " (rolled over = " << newIndex << ")";
        if ((index == mBufferMaxSize) || (index == mBackOfBuffer) || (!mIsFullFlag && (newIndex == mBackOfBuffer)) ||
            ((_M_wrap_index(mBackOfBuffer) - _M_wrap_index(index)) < steps))
        {
            newIndex = mBufferMaxSize;
        }

        return newIndex;
    }

    size_t _M_move_index_backward(size_t index, size_t steps) const noexcept
    {
        // If index points at the end of buffer, treat it as the back of buffer
        // Otherwise, decrement normally while accounting for wrap-around
        if ((index == mBufferMaxSize) && (steps > 0))
        {
            // If the buffer is full, the last element is stored at the back of buffer
            // Otherwise, the last element is one index before the back
            if (mIsFullFlag)
                return _M_roll_over_backward(mBackOfBuffer, steps - 1);
            else
                return _M_roll_over_backward(mBackOfBuffer, steps);
        }

        return _M_roll_over_backward(index, steps);
    }

    /**
     * @brief Converts an absolute index to its posiion within the buffer
     *
     * @param index
     * @return size_t
     */
    size_t _M_wrap_index(size_t index) const noexcept
    {
        // If the index points to the end of buffer, return that
        // Else return the current index position in reference to the front of the buffer.
        if (index == mBufferMaxSize)
            return mBufferMaxSize;
        else
            return (index < mFrontOfBuffer) ? mBufferMaxSize - (mFrontOfBuffer - index) : index - mFrontOfBuffer;
    }

    /**
     * @brief Converts a position within the buffer to its absolute index within the data
     *
     * @param index
     * @return size_t
     */
    size_t _M_unwrap_index(size_t index) const noexcept
    {
        if (index == mBufferMaxSize)
            return index;

        return (mFrontOfBuffer + index) % mBufferMaxSize;
    }
};

} // End namespace impl

// Forward declaration of ConstBufferIterator
template<typename T>
class ConstBufferIterator;

template<typename T>
class BufferIterator
{
    impl::BufferBase& mBufferRef;
    size_t            mCurrentIndex;

public:
    using iterator_category = std::random_access_iterator_tag;

    using difference_type = size_t;

    using value_type = T;
    using pointer    = T*;
    using reference  = T&;

    BufferIterator(impl::BufferBase& base) noexcept : mBufferRef(base), mCurrentIndex(base._M_unwrap_index(0))
    {
        std::cout << "Creating buffer iterator at index 0 (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    BufferIterator(impl::BufferBase& base, size_t index) noexcept
        : mBufferRef(base), mCurrentIndex((index == base.max_size()) ? index : index % base.max_size())
    {
        std::cout << "Creating buffer iterator at absolute index " << index << std::endl;
    }

    BufferIterator(const BufferIterator& rhs) : mBufferRef(rhs.mBufferRef), mCurrentIndex(rhs.mCurrentIndex)
    {
    }

    T& operator*() const noexcept
    {
        // Need to typecast buffer start address from void* before performing pointer arithmetic
        return *((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    T* operator->() const noexcept
    {
        // Need to typecast buffer start address from void* before performing pointer arithmetic
        return ((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    BufferIterator operator++() noexcept
    {
        std::cout << "Incrementing iterator index by 1 from " << mCurrentIndex;
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return *this;
    }

    BufferIterator operator++(int) noexcept
    {
        std::cout << "Incrementing iterator index by 1 from " << mCurrentIndex;
        BufferIterator tmp = *this;
        mCurrentIndex      = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return tmp;
    }

    BufferIterator operator--() noexcept
    {
        std::cout << "Decrementing iterator index by 1 from " << mCurrentIndex;
        mCurrentIndex = mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return *this;
    }

    BufferIterator operator--(int) noexcept
    {
        std::cout << "Decrementing iterator index by 1 from " << mCurrentIndex;
        BufferIterator tmp = *this;
        mCurrentIndex      = mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return tmp;
    }

    BufferIterator& operator+=(impl::BufferBase::difference_type rhs) noexcept
    {
        std::cout << "Incrementing iterator index by " << rhs << " from " << mCurrentIndex;
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, rhs);
        std::cout << " to " << mCurrentIndex << std::endl;
        return *this;
    }

    BufferIterator& operator-=(impl::BufferBase::difference_type rhs) noexcept
    {
        std::cout << "Decrementing iterator index by " << rhs << " from " << mCurrentIndex;
        mCurrentIndex = mBufferRef._M_move_index_backward(mCurrentIndex, rhs);
        std::cout << " to " << mCurrentIndex << std::endl;
        return *this;
    }

    BufferIterator& operator=(const BufferIterator& rhs)
    {
        mBufferRef    = rhs.mBufferRef;
        mCurrentIndex = rhs.mCurrentIndex;
        return *this;
    }

    bool operator<(const BufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) < rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator>(const BufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) > rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator<=(const BufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) <= rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator>=(const BufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) >= rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator!=(const BufferIterator& rhs) const noexcept
    {
        // Check base addresses first, then index numbers
        bool baseAddressesMatch = (mBufferRef._M_start_addr() == rhs.mBufferRef._M_start_addr());
        bool indexesMatch =
            (mBufferRef._M_wrap_index(mCurrentIndex) == rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
        return (!baseAddressesMatch || !indexesMatch);
    }

    T& operator[](impl::BufferBase::difference_type n) const noexcept
    {
        BufferIterator tmp = *this;
        tmp += n;
        return *tmp;
    }

    friend ConstBufferIterator<T>;

    template<typename AnyTypename>
    friend BufferIterator<AnyTypename> operator+(const BufferIterator<AnyTypename>& lhs,
                                                 impl::BufferBase::difference_type  rhs) noexcept;

    template<typename AnyTypename>
    friend BufferIterator<AnyTypename> operator+(const impl::BufferBase::difference_type lhs,
                                                 const BufferIterator<AnyTypename>&      rhs) noexcept;

    template<typename AnyTypename>
    friend impl::BufferBase::difference_type operator-(const BufferIterator<AnyTypename>& lhs,
                                                       const BufferIterator<AnyTypename>& rhs) noexcept;

    template<typename AnyTypename>
    friend BufferIterator<AnyTypename> operator-(const BufferIterator<AnyTypename>&      lhs,
                                                 const impl::BufferBase::difference_type rhs) noexcept;

    template<typename AnyTypename>
    friend bool operator==(const BufferIterator<AnyTypename>& lhs, const BufferIterator<AnyTypename>& rhs) noexcept;
};

template<typename T>
class ConstBufferIterator
{
    const impl::BufferBase& mBufferRef;
    size_t                  mCurrentIndex;

public:
    using iterator_category = std::random_access_iterator_tag;

    using difference_type = size_t;

    using value_type = T;
    using pointer    = T*;
    using reference  = T&;

    ConstBufferIterator(const impl::BufferBase& base) noexcept
        : mBufferRef(base), mCurrentIndex(base._M_unwrap_index(0))
    {
        std::cout << "Creating const buffer iterator at index 0 (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    ConstBufferIterator(const impl::BufferBase& base, size_t index) noexcept
        : mBufferRef(base), mCurrentIndex((index == base.max_size()) ? index : index % base.max_size())
    {
        std::cout << "Creating const buffer iterator at absolute index " << index << std::endl;
    }

    ConstBufferIterator(const ConstBufferIterator& rhs) : mBufferRef(rhs.mBufferRef), mCurrentIndex(rhs.mCurrentIndex)
    {
    }

    ConstBufferIterator(const BufferIterator<T>& rhs) : mBufferRef(rhs.mBufferRef), mCurrentIndex(rhs.mCurrentIndex)
    {
        std::cout << "Copy-creating const buffer iterator at index " << mBufferRef._M_wrap_index(mCurrentIndex)
                  << " (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    BufferIterator<T> undo_const() const noexcept
    {
        return BufferIterator<T>(mBufferRef, mCurrentIndex);
    }

    const T& operator*() const noexcept
    {
        // Need to typecast buffer start address from void* before performing pointer arithmetic
        return *((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    const T* operator->() const noexcept
    {
        // Need to typecast buffer start address from void* before performing pointer arithmetic
        return ((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    ConstBufferIterator operator++() noexcept
    {
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
        return *this;
    }

    ConstBufferIterator operator++(int) noexcept
    {
        ConstBufferIterator tmp = *this;
        mCurrentIndex           = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
        return tmp;
    }

    ConstBufferIterator operator--() noexcept
    {
        mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        return *this;
    }

    ConstBufferIterator operator--(int) noexcept
    {
        std::cout << "Decrementing iterator index by 1 from " << mCurrentIndex;
        ConstBufferIterator tmp = *this;
        mCurrentIndex           = mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return tmp;
    }

    ConstBufferIterator& operator+=(impl::BufferBase::difference_type rhs) noexcept
    {
        mBufferRef._M_move_index_forward(mCurrentIndex, rhs);
        return *this;
    }

    ConstBufferIterator& operator-=(impl::BufferBase::difference_type rhs) noexcept
    {
        mBufferRef._M_move_index_backward(mCurrentIndex, rhs);
        return *this;
    }

    ConstBufferIterator& operator=(const ConstBufferIterator& rhs)
    {
        mBufferRef    = rhs.mBufferRef;
        mCurrentIndex = rhs.mCurrentIndex;
        return *this;
    }

    bool operator<(const ConstBufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) < rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator>(const ConstBufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) > rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator<=(const ConstBufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) <= rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator>=(const ConstBufferIterator& rhs) const noexcept
    {
        return (mBufferRef._M_wrap_index(mCurrentIndex) >= rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    }

    bool operator!=(const ConstBufferIterator& rhs) const noexcept
    {
        // Check base addresses first, then index numbers
        bool baseAddressesMatch = (mBufferRef._M_start_addr() == rhs.mBufferRef._M_start_addr());
        bool indexesMatch =
            (mBufferRef._M_wrap_index(mCurrentIndex) == rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
        return (!baseAddressesMatch || !indexesMatch);
    }

    const T& operator[](impl::BufferBase::difference_type n) const noexcept
    {
        ConstBufferIterator tmp = *this;
        tmp += n;
        return *tmp;
    }

    template<typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator+(const ConstBufferIterator<AnyTypename>& lhs,
                                                      impl::BufferBase::difference_type       rhs) noexcept;

    template<typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator+(const impl::BufferBase::difference_type lhs,
                                                      const ConstBufferIterator<AnyTypename>& rhs) noexcept;

    template<typename AnyTypename>
    friend impl::BufferBase::difference_type operator-(const ConstBufferIterator<AnyTypename>& lhs,
                                                       const ConstBufferIterator<AnyTypename>& rhs) noexcept;

    template<typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator-(const ConstBufferIterator<AnyTypename>& lhs,
                                                      const impl::BufferBase::difference_type rhs) noexcept;

    template<typename AnyTypename>
    friend bool operator==(const ConstBufferIterator<AnyTypename>& lhs,
                           const ConstBufferIterator<AnyTypename>& rhs) noexcept;
};

//  BufferIterator friends  ///////////////////////////////////////////////////////////////////////////////////////////

template<typename AnyTypename>
BufferIterator<AnyTypename> operator+(const BufferIterator<AnyTypename>&         lhs,
                                      typename impl::BufferBase::difference_type rhs) noexcept
{
    BufferIterator<AnyTypename> tmp = lhs;
    tmp += rhs;
    return tmp;
}

template<typename AnyTypename>
BufferIterator<AnyTypename> operator+(const typename impl::BufferBase::difference_type lhs,
                                      const BufferIterator<AnyTypename>&               rhs) noexcept
{
    BufferIterator<AnyTypename> tmp = rhs;
    tmp += lhs;
    return tmp;
}

template<typename AnyTypename>
BufferIterator<AnyTypename> operator-(const BufferIterator<AnyTypename>&               lhs,
                                      const typename impl::BufferBase::difference_type rhs) noexcept
{
    BufferIterator<AnyTypename> tmp = lhs;
    tmp -= rhs;
    return tmp;
}

template<typename AnyTypename>
typename impl::BufferBase::difference_type operator-(const BufferIterator<AnyTypename>& lhs,
                                                     const BufferIterator<AnyTypename>& rhs) noexcept
{
    size_t lhCurrentPosition = lhs.mBufferRef._M_wrap_index(lhs.mCurrentIndex);
    size_t rhCurrentPosition = rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex);

    // If the right or left hand side is at the end of their buffer, they will be equivalent to that buffer's max size
    if (lhCurrentPosition == lhs.mBufferRef.max_size())
    {
        // Decrement the end index backwards by 1 to determine the back of buffer index
        size_t backOfBufferIndex = lhs.mBufferRef._M_move_index_backward(lhs.mCurrentIndex, 1);

        // The end iterator really represents 1 past the back of buffer, so add 1 to the position
        lhCurrentPosition = lhs.mBufferRef._M_wrap_index(backOfBufferIndex) + 1;
    }
    if (rhCurrentPosition == rhs.mBufferRef.max_size())
    {
        // Duplicate of above branch for right hand side iterator position
        size_t backOfBufferIndex = rhs.mBufferRef._M_move_index_backward(rhs.mCurrentIndex, 1);
        rhCurrentPosition        = rhs.mBufferRef._M_wrap_index(backOfBufferIndex) + 1;
    }

    return lhCurrentPosition - rhCurrentPosition;
}

template<typename AnyTypename>
bool operator==(const BufferIterator<AnyTypename>& rhs, const BufferIterator<AnyTypename>& lhs) noexcept
{
    // Make sure buffer base addresses match, then compare current index
    bool baseAddressesMatch = (rhs.mBufferRef._M_start_addr() == lhs.mBufferRef._M_start_addr());
    bool indexesMatch =
        (rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex) == lhs.mBufferRef._M_wrap_index(lhs.mCurrentIndex));
    return (baseAddressesMatch && indexesMatch);
}

//  ConstBufferIterator friends ///////////////////////////////////////////////////////////////////////////////////////

template<typename AnyTypename>
ConstBufferIterator<AnyTypename> operator+(const ConstBufferIterator<AnyTypename>&    lhs,
                                           typename impl::BufferBase::difference_type rhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = lhs;
    tmp += rhs;
    return tmp;
}

template<typename AnyTypename>
ConstBufferIterator<AnyTypename> operator+(const typename impl::BufferBase::difference_type lhs,
                                           const ConstBufferIterator<AnyTypename>&          rhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = rhs;
    tmp += lhs;
    return tmp;
}

template<typename AnyTypename>
ConstBufferIterator<AnyTypename> operator-(const ConstBufferIterator<AnyTypename>&          lhs,
                                           const typename impl::BufferBase::difference_type rhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = lhs;
    tmp -= rhs;
    return tmp;
}

template<typename AnyTypename>
typename impl::BufferBase::difference_type operator-(const ConstBufferIterator<AnyTypename>& lhs,
                                                     const ConstBufferIterator<AnyTypename>& rhs) noexcept
{
    size_t lhCurrentPosition = lhs.mBufferRef._M_wrap_index(lhs.mCurrentIndex);
    size_t rhCurrentPosition = rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex);

    // If the right or left hand side is at the end of their buffer, they will be equivalent to that buffer's max size
    if (lhCurrentPosition == lhs.mBufferRef.max_size())
    {
        // Decrement the end index backwards by 1 to determine the back of buffer index
        size_t backOfBufferIndex = lhs.mBufferRef._M_move_index_backward(lhs.mCurrentIndex, 1);

        // The end iterator really represents 1 past the back of buffer, so add 1 to the position
        lhCurrentPosition = lhs.mBufferRef._M_wrap_index(backOfBufferIndex) + 1;
    }
    if (rhCurrentPosition == rhs.mBufferRef.max_size())
    {
        // Duplicate of above branch for right hand side iterator position
        size_t backOfBufferIndex = rhs.mBufferRef._M_move_index_backward(rhs.mCurrentIndex, 1);
        rhCurrentPosition        = rhs.mBufferRef._M_wrap_index(backOfBufferIndex) + 1;
    }

    return lhCurrentPosition - rhCurrentPosition;
}

template<typename AnyTypename>
bool operator==(const ConstBufferIterator<AnyTypename>& lhs, const ConstBufferIterator<AnyTypename>& rhs) noexcept
{
    // Make sure buffer base addresses match, then compare current index
    bool baseAddressesMatch = (lhs.mBufferRef._M_start_addr() == rhs.mBufferRef._M_start_addr());
    bool indexesMatch =
        (lhs.mBufferRef._M_wrap_index(lhs.mCurrentIndex) == rhs.mBufferRef._M_wrap_index(rhs.mCurrentIndex));
    return (baseAddressesMatch && indexesMatch);
}

} // namespace shmit