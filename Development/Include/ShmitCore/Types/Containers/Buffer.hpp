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
    void resize(size_t n);

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
    iterator insert(iterator position, size_t n, T&& value);
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
    void _M_fill_forward(iterator position, size_t n, Arg&& value);

    template <typename Arg>
    void _M_fill_reverse(iterator position, size_t n, Arg&& value);

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

        LOG("Buffer of size " << mBufferSize << " created at " << std::hex << (uint64_t)mStartAddress << std::dec)
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
        // Set back of buffer to the end of the available space and raise full flag
        mStartAddress = _M_allocate_buffer(bufferSize);
        mBufferSize = bufferSize;
        mBackOfBuffer = bufferSize - 1;
        mIsFullFlag = true;

        // Construct elements in the buffer
        // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
        for (int i = 0; i < mBufferSize; i++)
            _alloc_traits::construct(mAllocator, (T*)mStartAddress + i, init);
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::~Buffer()
{
    // Destroy only elements that are initialized and stored in the buffer
    size_t curr = mFrontOfBuffer;
    while (curr != mBufferSize)
    {
        _alloc_traits::destroy(mAllocator, (T*)mStartAddress + curr);
        curr = _M_move_index_forward(curr, 1);
    }

    // Deallocate buffer
    // Need to typecast 'mStartAddress' from void*
    if (mStartAddress)
        _alloc_traits::deallocate(mAllocator, (T*)mStartAddress, mBufferSize);
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
    uint16_t backOfBuffer = _M_wrap_index(mBackOfBuffer);
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
    return *((T*)mStartAddress + mBackOfBuffer);
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
    return *((T*)mStartAddress + mFrontOfBuffer);
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
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return _M_insert_fill_front(1, copyOfValue);
    }
    else if (position == end())
    {
        return _M_insert_fill_back(1, copyOfValue);
    }

    // Otherwise call the random-access insert routine
    return _M_insert_fill(position, 1, copyOfValue);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, T&& value)
{    
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return _M_insert_fill_front(1, std::forward<T>(value));
    }
    else if (position == end())
    {
        return _M_insert_fill_back(1, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return _M_insert_fill(position, 1, std::forward<T>(value));
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, size_t n, const T& value)
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return _M_insert_fill_front(n, copyOfValue);
    }
    else if (position == end())
    {
        return _M_insert_fill_back(n, copyOfValue);
    }

    // Otherwise call the random-access insert routine
    return _M_insert_fill(position, n, copyOfValue);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, size_t n, T&& value)
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return _M_insert_fill_front(n, std::forward<T>(value));
    }
    else if (position == end())
    {
        return _M_insert_fill_back(n, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return _M_insert_fill(position, n, std::forward<T>(value));
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

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_back(const T& value)
{
    LOG("Pushing back")

    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;
    _M_insert_fill_back(1, copyOfValue);
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

    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;
    _M_insert_fill_front(1, copyOfValue);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T&& value)
{
    LOG("Pushing front")
    _M_insert_fill_front(1, std::forward<T>(value));
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::resize(size_t n)
{
    T* newBuffer = _M_allocate_buffer(n);

    // Copy current buffer data to the new buffer
    size_t destinationIndex = mFrontOfBuffer;
    iterator currentIt = begin();
    iterator endIt = end();
    while (currentIt != endIt)
    {
        newBuffer[destinationIndex] = *currentIt;
        currentIt++;
        destinationIndex = _M_roll_over_forward(destinationIndex, 1);
    }

    // Swap old buffer start address for new
    T* old = (T*)mStartAddress;
    mStartAddress = newBuffer;
    mBufferSize = n;

    _alloc_traits::deallocate(mAllocator, (T*)old, mBufferSize);
}

template <typename T, class Allocator>
size_t Buffer<T, Allocator>::size() noexcept
{
    if (mIsFullFlag)
        return mBufferSize;

    return _M_wrap_index(mBackOfBuffer) - _M_wrap_index(mFrontOfBuffer);
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
    // Offset the index from the start of buffer
    index = _M_roll_over_forward(mFrontOfBuffer, index);
    
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress + index);
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
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill(Buffer<T, Allocator>::iterator position,
                                                                             size_t n, Arg&& value)
{
    LOG("Inserting " << n << " element[s]")

    if (n >= mBufferSize)
    {
        LOG("Buffer overfill detected")

        // Fill in the entire buffer starting from the updated back and set the full flag
        size_t mBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1); // Back of buffer sits behind front when full
        mIsFullFlag = true;

        LOG("Moving back of buffer -> " << mBackOfBuffer)
        LOG("Setting full flag")

        // Cap n at the buffer size, anything over will be redundant
        n = mBufferSize;
    }
    else
    {
        // Cache the index where the last valid element is in the original data contents
        size_t lastValidElementIndex = (mIsFullFlag) ? mBackOfBuffer : _M_roll_over_backward(mBackOfBuffer, 1);
        LOG("Caching last valid element at " << lastValidElementIndex)
        
        // Update the back of buffer, do not allow overflow
        size_t newBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, n);
        LOG("New back of buffer -> " << newBackOfBuffer)
        // If the new back of buffer has overflowed the front its wrapped position will be less than the original's
        if (_M_wrap_index(newBackOfBuffer) < _M_wrap_index(mBackOfBuffer))
        {
            LOG("New back of buffer overflows front")
            newBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1); // Keep back behind front
            mIsFullFlag = true;                                         // The resultant buffer will be full
            LOG("New back of buffer -> " << newBackOfBuffer)
        }

        mBackOfBuffer = newBackOfBuffer;
        LOG("Back of buffer -> " << mBackOfBuffer)

        // 'position' is the first slot of the insert zone, find the last
        iterator insertZoneEnd = position + (n - 1);
        if (insertZoneEnd == end())
        {
            LOG("End of insert zone overflows front")

            // If elements to be inserted will overflow the front, we must truncate
            size_t openInsertSlots = insertZoneEnd - position;
            size_t overFlowDiff = n - openInsertSlots;
            LOG("Moving front and back forward by " << overFlowDiff)
            mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, overFlowDiff);
            mBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1);
            LOG("Front of buffer -> " << mFrontOfBuffer)
            LOG("Back of buffer -> " << mBackOfBuffer)
        }
        else
        {
            // Else, there is room after the insert zone for more data
            // Copy as much preexisting data behind 'position' as is possible without truncating anything before
            iterator dest = end() - 1;                      // New back of buffer
            iterator source(*this, lastValidElementIndex);  // Original back of buffer

            // Determine if any of the slots to copy will need to be truncated
            size_t slotsToCopy = (source - position) + 1;
            LOG("Slots to copy: " << slotsToCopy)
            size_t openCopySlots = dest - insertZoneEnd;
            LOG("Open copy slots: " << openCopySlots)
            if (openCopySlots < slotsToCopy)
            {
                size_t backTruncateDiff = slotsToCopy - openCopySlots;
                source -= backTruncateDiff;
                LOG("Back truncated by " << backTruncateDiff << " slots")
            }

            LOG("Moving old data")
            while (dest > insertZoneEnd)
                *dest-- = *source--;
        }
    }

    // Fill in the insert zone
    LOG("Filling insert zone")
    _M_fill_forward(position, n, std::forward<Arg>(value));

    return position;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill_back(size_t n, Arg&& value)
{
    LOG("Inserting " << n << " element[s] back")

    iterator insertZoneStart(*this, mBackOfBuffer);
    size_t resultSize = size() + n;
    LOG("Resulting buffer size " << resultSize)
    if (resultSize >= mBufferSize)
    {
        if (resultSize > mBufferSize)
        {        
            // Increment front of buffer by the rollover difference to accomodate new data
            size_t rollOverDiff = resultSize - mBufferSize;
            LOG("Buffer overflow detected -> rollover: " << rollOverDiff)
            mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, (size_t)rollOverDiff);
            LOG("Moving front of buffer -> " << mFrontOfBuffer)
        }
        
        // The resulting buffer will be full, set the back of buffer 1 ahead of the front
        mBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1);
        mIsFullFlag = true;
        LOG("Setting full flag")
    }
    else
        mBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, n);

    
    LOG("Moving back of buffer -> " << mBackOfBuffer)

    // Fill in insert zone
    _M_fill_forward(insertZoneStart, n, std::forward<Arg>(value));

    return insertZoneStart;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::_M_insert_fill_front(size_t n, Arg&& value)
{
    LOG("Inserting " << n << " element[s] front")

    size_t resultSize = size() + n;
    LOG("Resulting buffer size " << resultSize)
    if (resultSize >= mBufferSize)
    {
        // The resulting buffer will be full
        size_t rollOverDiff = 0;
        if (resultSize > mBufferSize)
        {
            // If the insertion will overflow the buffer, determine how many elements need to be truncated
            rollOverDiff = resultSize - mBufferSize;
            LOG("Buffer overflow detected -> rollover: " << rollOverDiff)
        }
        else if (!mIsFullFlag) // If the buffer is not currently full, back of buffer can be decremented by 1
            rollOverDiff++; 
        
        // Truncate back elements if necessary
        // The front of buffer is then 1 behind the back of buffer
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, rollOverDiff);
        mFrontOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
        mIsFullFlag = true;
        LOG("Setting full flag")
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }
    else
        mFrontOfBuffer = _M_roll_over_backward(mFrontOfBuffer, n);

    
    LOG("Moving front of buffer -> " << mFrontOfBuffer)

    // Fill in insert zone
    iterator insertZoneStart(*this, mFrontOfBuffer);
    _M_fill_forward(insertZoneStart, n, std::forward<Arg>(value));

    return insertZoneStart;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
void Buffer<T, Allocator>::_M_fill_forward(typename Buffer<T, Allocator>::iterator position, size_t n, Arg&& value)
{
    for (size_t i = 0; i < n; i++)
    {
        // Guard against assigning to invalid position
        if (position == end())
            return;

        *position = value;
        LOG("Assigned " << *position << " to buffer element")
        position++;
    }
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
void Buffer<T, Allocator>::_M_fill_reverse(typename Buffer<T, Allocator>::iterator position, size_t n, Arg&& value)
{
    for (size_t i = 0; i < n; i++)
    {
        *position = value;
        LOG("Assigned " << *position << " to buffer element")
        position--;
    }
}

// TODO remove
template <typename T, class Allocator>
void PrintBuf(Buffer<T, Allocator>& buf)
{
    for (auto e : buf)
    {
        LOG(e)
    }
}

}