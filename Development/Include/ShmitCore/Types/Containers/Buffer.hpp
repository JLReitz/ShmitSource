#pragma once

#include "BufferImpl.hpp"

#define LOG(line)   std::cout << line << std::endl;

namespace shmit
{

template <typename T, class Allocator = std::allocator<T>>
class Buffer : public impl::BufferBase
{
    
    typedef __gnu_cxx::__alloc_traits<Allocator>    _alloc_traits;

public:

    typedef typename _alloc_traits::value_type      value_type;
    typedef typename _alloc_traits::pointer	        pointer;
    typedef typename _alloc_traits::const_pointer 	const_pointer;
    typedef typename _alloc_traits::reference       reference;
    typedef typename _alloc_traits::const_reference	const_reference;

    typedef Allocator allocator_type;

    typedef BufferIterator<T>                   iterator;
    typedef ConstBufferIterator<T>              const_iterator;
    typedef std::reverse_iterator<iterator>		    reverse_iterator;
    typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;

    typedef typename impl::BufferBase::size_type        size_type;
    typedef typename impl::BufferBase::difference_type  difference_type;

    Buffer(const Allocator& allocator = Allocator());
    Buffer(size_t bufferSize, const Allocator& allocator = Allocator());
    Buffer(size_t bufferSize, const T& init, const Allocator& allocator = Allocator());

    Buffer(std::initializer_list<T> il);

    Buffer(const Buffer& buffer, const Allocator& allocator = Allocator());
    Buffer(const Buffer&& buffer, const Allocator& allocator = Allocator());

    ~Buffer();

    size_t size() noexcept;
    bool empty() noexcept;
    bool full() noexcept;

    iterator begin() noexcept;
    reverse_iterator rbegin() noexcept;
    const_iterator cbegin() noexcept;
    const_reverse_iterator crbegin() noexcept;

    iterator end() noexcept;
    reverse_iterator rend() noexcept;
    const_iterator cend() noexcept;
    const_reverse_iterator crend() noexcept;

    T& at(size_t index);

    T& front();
    T& back();

    void pop_front() noexcept;
    void push_front(const T& value);
    void push_front(T&& value);

    void pop_back() noexcept;
    void push_back(const T& value);
    void push_back(T&& value);

    iterator insert(iterator position, const T& value);
    iterator insert(iterator position, T&& value);
    iterator insert(iterator position, size_t n, const T& value);
    iterator insert(iterator position, iterator i, iterator j);
    iterator insert(iterator position, std::initializer_list<T> il); // TODO define
    
    // TODO
    template<typename... Args>
    iterator emplace(iterator position, Args&&... args);

    template <typename... Args>
    void emplace_front(Args&&... args);

    template <typename... Args>
    void emplace_back(Args&&... args);

    void swap(Buffer& other);

    iterator erase(iterator position) noexcept;
    iterator erase(iterator position, iterator i, iterator j) noexcept;

    void clear() noexcept;

    Allocator get_allocator() noexcept;

    Buffer& operator=(const Buffer& rhs);
    Buffer& operator=(Buffer&& rhs);

    bool operator!=(const Buffer& lhs) noexcept;

    T& operator[](size_t index) noexcept;

    template <typename AnyDataType, typename AnyAllocatorType>
    friend bool operator==(const Buffer<AnyDataType, AnyAllocatorType>& rhs, const Buffer<AnyDataType, AnyAllocatorType>& lhs) noexcept;

    void PrintDiagnostic()
    {
        LOG("Buffer contents: ");
        for (auto i : *this)
        {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }

private:

    T* _M_allocate_buffer(size_t n);

    template <typename Arg>
    iterator _M_insert_fill(iterator position, size_t n, Arg&& value);

    template <typename Arg>
    iterator _M_insert_fill_front(size_t n, Arg&& value);

    template <typename Arg>
    iterator _M_insert_fill_back(size_t n, Arg&& value);

    template <typename Arg>
    iterator _M_insert_single(iterator position, Arg&& value);

    template <typename Arg>
    void _M_insert_fill_reverse(iterator start, size_t n, Arg&& value);

    Allocator mAllocator;
};

template <typename T, class Allocator>
void swap(Buffer<T, Allocator>& rhs, Buffer<T, Allocator>& lhs)
{
    rhs.swap(lhs);
}

template <typename AnyDataType, class AnyAllocatorType>
bool operator==(const Buffer<AnyDataType, AnyAllocatorType>& rhs, const Buffer<AnyDataType, AnyAllocatorType>& lhs) noexcept
{
    // TODO implement
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(const Allocator& allocator)
    : BufferBase(), mAllocator(allocator)
{ }

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, const Allocator& allocator)
    : BufferBase(), mAllocator(allocator)
{
    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        mStartAddress = _M_allocate_buffer(bufferSize);
        mBufferSize = bufferSize;

        LOG("Buffer of size " << mBufferSize << " created at " << std::hex << (uint64_t)mStartAddress.load() << std::dec)
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, const T& init, const Allocator& allocator)
    : BufferBase(), mAllocator(allocator)
{
    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        mStartAddress = _M_allocate_buffer(bufferSize);
        mBufferSize = bufferSize;

        // Construct elements in the buffer
        for (int i = 0; i < mBufferSize; i++)
        {
            _alloc_traits::construct(mAllocator, (T*)mStartAddress + i, init);
        }
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::~Buffer()
{
    // Destroy only elements that are initialized and stored in the buffer
    size_t curr = mFrontOfBuffer;
    while (curr != mBufferSize)
    {
        _alloc_traits::destroy(mAllocator, (T*)mStartAddress.load() + curr);
        curr = _M_move_index_forward(curr, 1);
    }

    // Deallocate buffer
    if (mStartAddress)
        _alloc_traits::deallocate(mAllocator, (T*)mStartAddress.load(), mBufferSize);
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::at(size_t index)
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= mBufferSize))
    {
        // TODO Throw exception or enter hard fault trap
    }

    uint16_t frontOfBuffer = mFrontOfBuffer;
    uint16_t backOfBuffer = _M_normalize_index(mBackOfBuffer);
    index += frontOfBuffer;

    // Check for out-of-bounds conditions past the back-of-buffer index
    // If the index extends past the back, it is not accessible
    // If the index equals the back, it is only accessible when the buffer is full
    if ((index > backOfBuffer) || ((index == backOfBuffer) && !full()))
    {
        // TODO Throw exception or enter hard fault trap
    }
    
    // Account for index wrap-around
    // Because of the out-of-bounds checks above, we are assured that any wrapped index is <= back-of-buffer
    index %= mBufferSize;

    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress + index);
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::back()
{
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress.load() + mBackOfBuffer);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::begin() noexcept
{
    return iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cbegin() noexcept
{
    return const_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cend() noexcept
{
    return const_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crbegin() noexcept
{
    return const_reverse_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crend() noexcept
{
    return const_reverse_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
bool Buffer<T, Allocator>::empty() noexcept
{
    return (mFrontOfBuffer == mBackOfBuffer);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::end() noexcept
{
    return iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::front()
{
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress.load() + mFrontOfBuffer);
}

template <typename T, class Allocator>
bool Buffer<T, Allocator>::full() noexcept
{
    return mIsFullFlag;
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, const T& value)
{
    // If the position is the end or beginning of the buffer, perform a push.
    // These access operations won't move any data around and thus no temporary copy of the value reference is
    // required.
    if (position == begin())
    {
        return _M_insert_fill_front(1, value);
    }
    else if (position == end())
    {
        return _M_insert_fill_back(1, value);
    }

    // Otherwise call the random-access insert routine.
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the moving of 
    // elements.
    T copyOfValue = value;
    return _M_insert_single(position, std::move(copyOfValue));
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, T&& value)
{    
    // If the position is the end or beginning of the buffer, perform a push
    if (position == begin())
    {
        return _M_insert_fill_front(1, std::forward<T>(value));
    }
    else if (position == end())
    {
        return _M_insert_fill_back(1, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return _M_insert_single(position, std::forward<T>(value));
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, size_t n, const T& value)
{
    
}

// template <typename T, class Allocator>
// typename Buffer<T, Allocator>::iterator 
//     Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, 
//                                  typename Buffer<T, Allocator>::iterator i, 
//                                  typename Buffer<T, Allocator>::iterator j)
// {
    
// }

template <typename T, class Allocator>
void Buffer<T, Allocator>::pop_back() noexcept
{
    LOG("Popping back")
    // Check for empty condition
    if (!empty())
    {
        // Move back of buffer backwards 1 step, clear full flag
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, 1);
        LOG("Moving back of buffer -> " << mBackOfBuffer)
        mIsFullFlag = false;
    }
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::pop_front() noexcept
{
    LOG("Popping front")
    // Check for empty condition
    if (!empty())
    {    
        // Move front of buffer fowards 1 step
        mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, 1);
        LOG("Moving front of buffer -> " << mFrontOfBuffer)

        // If the buffer is full, the back of buffer must be advanced to the now vacant position
        if (mIsFullFlag)
        {
            mBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
            LOG("Moving back of buffer -> " << mBackOfBuffer)
        }

        // Clear the full flag
        mIsFullFlag = false;
    }
}

// template <typename T>
// T ClearLeaveOrAdd(std::atomic<T>& value, bool clear, bool leave)
// {
//     T tmp = value;
//     if (clear)
//     {
//         value = 0;
//     }
//     else if (!leave)
//     {
//         value++;
//     }

//     return value;
// }

// // Checks clear condition and leave condition, then adds if neither passes
// #define ClearLeaveOrAdd_Sequential(value, clearCondition, leaveCondition) \
//     (clearCondition) ? ClearLeaveOrAdd(value, true, false) : \
//     ((leaveCondition) ? ClearLeaveOrAdd(value, false, true) : ClearLeaveOrAdd(value, false, false))

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_back(const T& value)
{
    LOG("Pushing back")
    // These access operations won't move any data around and thus no temporary copy of the value reference is
    // required.
    _M_insert_fill_back(1, value);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_back(T&& value)
{
    LOG("Pushing back")
    _M_insert_fill_back(1, std::forward<T>(value));
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_front(const T& value)
{
    LOG("Pushing front")
    // These access operations won't move any data around and thus no temporary copy of the value reference is
    // required.
    _M_insert_fill_front(1, value);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T&& value)
{
    LOG("Pushing front")
    _M_insert_fill_front(1, std::forward<T>(value));
}

template <typename T, class Allocator>
size_t Buffer<T, Allocator>::size() noexcept
{
    if (mIsFullFlag)
        return mBufferSize;

    return _M_normalize_index(mBackOfBuffer) - _M_normalize_index(mFrontOfBuffer);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rend() noexcept
{
    return reverse_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::operator[](size_t index) noexcept
{
    // Offset the index from the start of buffer, then account for wrap-around
    index += mFrontOfBuffer;
    index %= mBufferSize;
    
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress.load() + index);
}

template <typename T, class Allocator>
T* Buffer<T, Allocator>::_M_allocate_buffer(size_t n)
{
    T* bufferPtr = _alloc_traits::allocate(mAllocator, n);
    if (bufferPtr == nullptr)
    {
        // TODO Throw exception or enter hard fault trap
    }

    return bufferPtr;
}

template <typename T, class Allocator>
template <typename Arg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill(Buffer<T, Allocator>::iterator position, size_t n,
                                                                    Arg&& value)
{

}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_single(typename Buffer<T, Allocator>::iterator position, 
                                                                               Arg&& value)
{
    LOG("Inserting single element")
    // First, allocate a destination for the current buffer contents + the inserted value
    T* bufferUpdate = _M_allocate_buffer(mBufferSize);
    LOG("Allocated replacement buffer at " << std::hex << (uint64_t)bufferUpdate << std::dec)

    // Check for insert overflow
    // If the buffer is full, start by decrementing the back of buffer by 1 to make room for this new element
    if (mIsFullFlag)
    {
        LOG("Buffer overflow detected")
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, 1);
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Next, copy data in to the destination, including the inserted value at the provided position
    size_t i = mFrontOfBuffer;
    size_t insertIndex = 0;
    iterator current = begin();
    iterator currentEnd = end();
    while (current != currentEnd)
    {
        if (current == position)
        {
            // Insert the new value in to the provided position
            // Cache the abosolute index of that position
            insertIndex = i;
            bufferUpdate[insertIndex] = value;
            i = _M_roll_over_forward(i, 1);
            LOG("Inserting " << value << " at index " << insertIndex)
        }

        bufferUpdate[i] = *current;
        current++;
        i = _M_roll_over_forward(i, 1);
    }

    // Increment the back of buffer or set the full flag if necessary
    size_t newBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
    if (newBackOfBuffer == mFrontOfBuffer)
    {
        mIsFullFlag = true;
        LOG("Setting full flag")
    }
    else
    {
        mBackOfBuffer = newBackOfBuffer;
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Finally, store the address of the updated buffer contents and delete the old buffer
    T* oldBuffer = (T*)mStartAddress.load();
    mStartAddress = bufferUpdate;

    LOG("Removing old buffer contents at " << std::hex << (uint64_t)oldBuffer << std::dec)
    _alloc_traits::deallocate(mAllocator, oldBuffer, mBufferSize);

    // Return an iterator representing the inserted element
    return iterator(*this, insertIndex);
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill_back(size_t n, Arg&& value)
{
    LOG("Inserting " << n << " element[s] back")
    long rollOverDiff = (size() + n) - max_size();
    if (rollOverDiff > 0)
    {
        LOG("Buffer overflow detected")

        // Increment front of buffer by the rollover difference to accomodate new data
        // Increment the back of buffer by 1
        mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, rollOverDiff);
        mBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Calculate the last index to insert at
    size_t endIndex = _M_roll_over_forward(mBackOfBuffer, n - 1);
    LOG("Insert from " << mBackOfBuffer << " -> " << endIndex)

    // Insert in reverse, starting from the final insert index to the starting back of buffer
    iterator justInserted = iterator(*this, endIndex);
    _M_insert_fill_reverse(justInserted, n, std::forward<Arg>(value));
    
    for (size_t i = 0; i < n; i++)
    {
        *justInserted = std::forward<T>(value);
        LOG("Assigned " << *justInserted << " to back of buffer")
        justInserted--;
    }

    if (rollOverDiff >= 0)
    {
        mIsFullFlag = true;
        mBackOfBuffer = endIndex;
        LOG("Setting full flag")
    }
    else
    {
        mBackOfBuffer = _M_roll_over_forward(endIndex, 1);
    }
    
    LOG("Moving back of buffer -> " << mBackOfBuffer)

    // Return an iterator to the first of the newly inserted values
    return justInserted;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill_front(size_t n, Arg&& value)
{
    LOG("Inserting " << n << " element[s] front")
    long rollOverDiff = (size() + n) - max_size();
    if (rollOverDiff > 0)
    {
        LOG("Buffer overflow detected")

        // Decrement back of buffer by the rollover difference to accomodate new data
        // Decrement front of buffer once
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, rollOverDiff);
        mFrontOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1);
        LOG("Moving back of buffer -> " << mBackOfBuffer)
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }

    // Calculate the last index to insert at
    size_t endIndex = _M_roll_over_backward(mFrontOfBuffer, n - 1);
    LOG("Insert from " << endIndex << " -> " << mFrontOfBuffer)

    // Insert 

    // Start at the front of the buffer
    size_t startFrontOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1);

    if (_M_roll_over_forward(mBackOfBuffer, 1) == mFrontOfBuffer)
    {
        mIsFullFlag = true;
        LOG("Setting full flag")
    }

    // OLD

    size_t newFrontOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1);
    bool frontUpdateIsOutOfBounds = (newFrontOfBuffer == mBackOfBuffer);

    // Check for insert overflow
    if (frontUpdateIsOutOfBounds && mIsFullFlag)
    {
        LOG("Buffer overflow detected")

        // Decrement back of buffer by 1 to accomodate new data
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, 1);
        LOG("Moving back of buffer -> " << mBackOfBuffer)
        mFrontOfBuffer = newFrontOfBuffer;
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }

    // Assign value to front of buffer, then update front index if the buffer is not full
    iterator justInserted = iterator(*this, mFrontOfBuffer);
    *justInserted = std::forward<T>(value);

    LOG("Assigned " << *justInserted << " to front of buffer")

    if (!frontUpdateIsOutOfBounds)
    {
        mFrontOfBuffer = newFrontOfBuffer;
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }
    else
    {
        mIsFullFlag = true;
        LOG("Setting full flag")
    }

    // Return an iterator representing the value that was just inserted
    return justInserted;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
void Buffer<T, Allocator>::_M_insert_fill_reverse(typename Buffer<T, Allocator>::iterator start, size_t n, Arg&& value)
{
    // Insert in reverse, starting at the provided iterator
    for (size_t i = 0; i < n; i++)
    {
        *start = std::forward<T>(value);
        LOG("Assigned " << *justInserted << " to back of buffer")
        start--;
    }
}

}