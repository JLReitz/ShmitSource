#pragma once

#include <ShmitCore/StdIncludes.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

#include <atomic>
#include <initializer_list>
#include <iterator>
#include <memory>

#include <iostream>

namespace shmit
{
namespace impl
{

class BufferBase
{
public:

    typedef size_t  size_type;
    typedef size_t  difference_type;

protected:

    size_t mBufferSize;
    std::atomic<shmit::MemoryAddress> mStartAddress;

    std::atomic_uint mFrontOfBuffer; // First poppable index
    std::atomic_uint mBackOfBuffer; // Last poppable index
    
    std::atomic_bool mIsFullFlag; // Flag to signal full condition

    size_t _M_roll_over_forward(size_t index, size_t steps) const
    {
        return ((index + steps) % mBufferSize);
    }

    size_t _M_roll_over_backward(size_t index, size_t steps) const
    {
        return ((index < steps) ? mBufferSize - (steps - index) : index - steps);
    }

public:

    BufferBase() noexcept
        : mStartAddress(nullptr), mBufferSize(0), mFrontOfBuffer(0), mBackOfBuffer(0), mIsFullFlag(false)
    { }

    size_t max_size() const noexcept
    {
        return mBufferSize;
    }

    shmit::MemoryAddress _M_start_addr() const noexcept
    {
        return mStartAddress;
    }

    size_t _M_move_index_forward(size_t index, size_t steps) const noexcept
    {
        // If index points at the end of buffer already, do not increment.
        // If it points to the back of the buffer, increment to end of buffer.
        // If the new index points to the back of the buffer but the buffer is not full, increment to end of buffer.
        // If the new index would point past the back of the buffer, increment to end of buffer.
        // Otherwise increment normally while accounting for wrap-around.
        size_t newIndex = _M_roll_over_forward(index, steps);
        //std::cout << " (rolled over = " << newIndex << ")";
        if ((index == mBufferSize) ||
            (index == mBackOfBuffer) ||
            (!mIsFullFlag && (newIndex == mBackOfBuffer)) ||
            ((_M_normalize_index(mBackOfBuffer) - _M_normalize_index(index)) < steps))
        {
            newIndex = mBufferSize;
        }

        return newIndex;
    }

    size_t _M_move_index_backward(size_t index, size_t steps) const noexcept
    {
        // If index points at the end of buffer, decrement to the back of buffer
        // Otherwise, decrement normally while accounting for wrap-around
        size_t newIndex = _M_roll_over_backward(index, steps);
        //std::cout << " (rolled over = " << newIndex << ")";
        if (index == mBufferSize)
            newIndex = mBackOfBuffer;
        
        return newIndex;
    }

    size_t _M_normalize_index(size_t index) const noexcept
    {
        // If the index points to the end of buffer, return that
        // Else return the current index position in reference to the front of the buffer.
        if (index == mBufferSize)
            return mBufferSize;
        else
            return (index < mFrontOfBuffer) ? mBufferSize - (mFrontOfBuffer - index) : index - mFrontOfBuffer;
    }

    size_t _M_get_index_relative_to_front(size_t index)
    {
        if (index == mBufferSize)
            return index;

        return (mFrontOfBuffer + index) % mBufferSize;
    }
};

} // End namespace impl

// Forward declaration of ConstBufferIterator
template <typename T>
class ConstBufferIterator;

template <typename T>
class BufferIterator
{
    impl::BufferBase& mBufferRef;
    size_t mCurrentIndex;

public:

    BufferIterator(impl::BufferBase& base) noexcept
        : mBufferRef(base), mCurrentIndex(base._M_get_index_relative_to_front(0))
    { 
        std::cout << "Creating buffer iterator at index 0 (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    BufferIterator(impl::BufferBase& base, size_t index) noexcept
        : mBufferRef(base), mCurrentIndex((index == base.max_size()) ? index : index % base.max_size())
    {
        std::cout << "Creating buffer iterator at absolute index " << index << std::endl;
    }

    T& operator*() const noexcept
    {
        // Need to cast to T pointer type before adding the index offset
        return *((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    T* operator->() const noexcept
    {
        // Need to cast to T pointer type before adding the index offset
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
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
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
        mCurrentIndex = mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return tmp;
    }

    BufferIterator& operator+=(impl::BufferBase::difference_type lhs) noexcept
    {
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, lhs);
        return *this;
    }

    BufferIterator& operator-=(impl::BufferBase::difference_type lhs) noexcept
    {
        mCurrentIndex = mBufferRef._M_move_index_backward(mCurrentIndex, lhs);
        return *this;
    }

    bool operator<(const BufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) < lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator>(const BufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) > lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator<=(const BufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) <= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator>=(const BufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) >= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator!=(const BufferIterator& lhs) const noexcept
    {
        // Check base addresses first, then index numbers
        bool baseAddressesMatch = (mBufferRef._M_start_addr() == lhs.mBufferRef._M_start_addr());
        bool indexesMatch = 
            (mBufferRef._M_normalize_index(mCurrentIndex) == lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
        return (!baseAddressesMatch || !indexesMatch);
    }

    T& operator[](impl::BufferBase::difference_type n) const noexcept
    {
        BufferIterator tmp = *this;
        tmp += n;
        return *tmp;
    }

    friend ConstBufferIterator<T>;

    template <typename AnyTypename>
    friend BufferIterator<AnyTypename> operator+(const BufferIterator<AnyTypename>& rhs, impl::BufferBase::difference_type lhs) noexcept;

    template <typename AnyTypename>
    friend BufferIterator<AnyTypename> operator+(const impl::BufferBase::difference_type rhs, const BufferIterator<AnyTypename>& lhs) noexcept;

    template <typename AnyTypename>
    friend impl::BufferBase::difference_type operator-(const BufferIterator& rhs, const BufferIterator<AnyTypename>& lhs) noexcept;

    template <typename AnyTypename>
    friend BufferIterator<AnyTypename> operator-(const BufferIterator<AnyTypename>& rhs, const impl::BufferBase::difference_type lhs) noexcept;

    template <typename AnyTypename>
    friend bool operator==(const BufferIterator<AnyTypename>& rhs, const BufferIterator<AnyTypename>& lhs) noexcept;
};

template <typename T>
class ConstBufferIterator
{
    impl::BufferBase& mBufferRef;
    size_t mCurrentIndex;

public:

    ConstBufferIterator(impl::BufferBase& base) noexcept
        : mBufferRef(base), mCurrentIndex(base._M_get_index_relative_to_front(0))
    { 
        std::cout << "Creating const buffer iterator at index 0 (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    ConstBufferIterator(impl::BufferBase& base, size_t index) noexcept
        : mBufferRef(base), mCurrentIndex((index == base.max_size()) ? index : index % base.max_size())
    { 
        std::cout << "Creating const buffer iterator at absolute index " << index << std::endl;
    }

    ConstBufferIterator(const BufferIterator<T>& copy)
        : mBufferRef(copy.mBufferRef), mCurrentIndex(copy.mCurrentIndex)
    {
        std::cout << "Copy-creating const buffer iterator at index " << mBufferRef._M_normalize_index(mCurrentIndex) << " (absolute = " << mCurrentIndex << ")" << std::endl;
    }

    BufferIterator<T> undo_const() const noexcept
    {
        return BufferIterator<T>(mBufferRef, mCurrentIndex);
    }

    const T& operator*() const noexcept
    {
        // Need to cast to T pointer type before adding the index offset
        return *((T*)mBufferRef._M_start_addr() + mCurrentIndex);
    }

    const T* operator->() const noexcept
    {
        // Need to cast to T pointer type before adding the index offset
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
        mCurrentIndex = mBufferRef._M_move_index_forward(mCurrentIndex, 1);
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
        mCurrentIndex = mBufferRef._M_move_index_backward(mCurrentIndex, 1);
        std::cout << " to " << mCurrentIndex << std::endl;
        return tmp;
    }

    ConstBufferIterator& operator+=(impl::BufferBase::difference_type lhs) noexcept
    {
        mBufferRef._M_move_index_forward(mCurrentIndex, lhs);
        return *this;
    }

    ConstBufferIterator& operator-=(impl::BufferBase::difference_type lhs) noexcept
    {
        mBufferRef._M_move_index_backward(mCurrentIndex, lhs);
        return *this;
    }

    bool operator<(const ConstBufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) < lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator>(const ConstBufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) > lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator<=(const ConstBufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) <= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator>=(const ConstBufferIterator& lhs) const noexcept
    {
        return (mBufferRef._M_normalize_index(mCurrentIndex) >= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    }

    bool operator!=(const ConstBufferIterator& lhs) const noexcept
    {
        // Check base addresses first, then index numbers
        bool baseAddressesMatch = (mBufferRef._M_start_addr() == lhs.mBufferRef._M_start_addr());
        bool indexesMatch = 
            (mBufferRef._M_normalize_index(mCurrentIndex) == lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
        return (!baseAddressesMatch || !indexesMatch);
    }

    const T& operator[](impl::BufferBase::difference_type n) const noexcept
    {
        ConstBufferIterator tmp = *this;
        tmp += n;
        return *tmp;
    }

    template <typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator+(const ConstBufferIterator<AnyTypename>& rhs, impl::BufferBase::difference_type lhs) noexcept;

    template <typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator+(const impl::BufferBase::difference_type rhs, const ConstBufferIterator<AnyTypename>& lhs) noexcept;

    template <typename AnyTypename>
    friend impl::BufferBase::difference_type operator-(const ConstBufferIterator<AnyTypename>& rhs, const ConstBufferIterator<AnyTypename>& lhs) noexcept;

    template <typename AnyTypename>
    friend ConstBufferIterator<AnyTypename> operator-(const ConstBufferIterator<AnyTypename>& rhs, const impl::BufferBase::difference_type lhs) noexcept;

    template <typename AnyTypename>
    friend bool operator==(const ConstBufferIterator<AnyTypename>& rhs, const ConstBufferIterator<AnyTypename>& lhs) noexcept;
};

//  BufferIterator friends  ///////////////////////////////////////////////////////////////////////////////////////////

template <typename AnyTypename>
BufferIterator<AnyTypename> operator+(const BufferIterator<AnyTypename>& rhs, 
                            typename impl::BufferBase::difference_type lhs) noexcept
{
    BufferIterator<AnyTypename> tmp = rhs;
    tmp += lhs;
    return tmp;
}

template <typename AnyTypename>
BufferIterator<AnyTypename> operator+(const typename impl::BufferBase::difference_type rhs, 
                            const BufferIterator<AnyTypename>& lhs) noexcept
{
    BufferIterator<AnyTypename> tmp = lhs;
    tmp += rhs;
    return tmp;
}

template <typename AnyTypename>
BufferIterator<AnyTypename> operator-(const BufferIterator<AnyTypename>& rhs, 
                            const typename impl::BufferBase::difference_type lhs) noexcept
{
    BufferIterator<AnyTypename> tmp = rhs;
    tmp -= lhs;
    return tmp;
}

template <typename AnyTypename>
typename impl::BufferBase::difference_type operator-(const BufferIterator<AnyTypename>& rhs, 
                                                      const BufferIterator<AnyTypename>& lhs) noexcept
{
    BufferIterator<AnyTypename> tmp = rhs;
    tmp -= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex); // Subtract by lhs relative position within its buffer
    return tmp;
}

template <typename AnyTypename>
bool operator==(const BufferIterator<AnyTypename>& rhs, const BufferIterator<AnyTypename>& lhs) noexcept
{
    // Make sure buffer base addresses match, then compare current index
    bool baseAddressesMatch = (rhs.mBufferRef._M_start_addr() == lhs.mBufferRef._M_start_addr());
    bool indexesMatch = 
        (rhs.mBufferRef._M_normalize_index(rhs.mCurrentIndex) == lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    return (baseAddressesMatch && indexesMatch);
}

//  ConstBufferIterator friends ///////////////////////////////////////////////////////////////////////////////////////

template <typename AnyTypename>
ConstBufferIterator<AnyTypename> operator+(const ConstBufferIterator<AnyTypename>& rhs, 
                                 typename impl::BufferBase::difference_type lhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = rhs;
    tmp += lhs;
    return tmp;
}

template <typename AnyTypename>
ConstBufferIterator<AnyTypename> operator+(const typename impl::BufferBase::difference_type rhs, 
                                 const ConstBufferIterator<AnyTypename>& lhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = lhs;
    tmp += rhs;
    return tmp;
}

template <typename AnyTypename>
ConstBufferIterator<AnyTypename> operator-(const ConstBufferIterator<AnyTypename>& rhs, 
                                 const typename impl::BufferBase::difference_type lhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = rhs;
    tmp -= lhs;
    return tmp;
}

template <typename AnyTypename>
typename impl::BufferBase::difference_type operator-(const ConstBufferIterator<AnyTypename>& rhs, 
                                                           const ConstBufferIterator<AnyTypename>& lhs) noexcept
{
    ConstBufferIterator<AnyTypename> tmp = rhs;
    tmp -= lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex); // Subtract by lhs relative position within its buffer
    return tmp;
}

template <typename AnyTypename>
bool operator==(const ConstBufferIterator<AnyTypename>& rhs, const ConstBufferIterator<AnyTypename>& lhs) noexcept
{
    // Make sure buffer base addresses match, then compare current index
    bool baseAddressesMatch = (rhs.mBufferRef._M_start_addr() == lhs.mBufferRef._M_start_addr());
    bool indexesMatch = 
        (rhs.mBufferRef._M_normalize_index(rhs.mCurrentIndex) == lhs.mBufferRef._M_normalize_index(lhs.mCurrentIndex));
    return (baseAddressesMatch && indexesMatch);
}

}