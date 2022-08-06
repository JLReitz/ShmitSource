#pragma once

#include "BufferImpl.hpp"

#include <type_traits>

#define LOG(line)   std::cout << line << std::endl;

namespace shmit
{

template <typename T, class Allocator = std::allocator<T>>
class Buffer : public impl::BufferBase
{
    
    typedef __gnu_cxx::__alloc_traits<Allocator>    alloc_traits;

public:

    typedef typename alloc_traits::value_type       value_type;
    typedef typename alloc_traits::pointer	        pointer;
    typedef typename alloc_traits::const_pointer 	const_pointer;
    typedef typename alloc_traits::reference        reference;
    typedef typename alloc_traits::const_reference	const_reference;

    typedef Allocator allocator_type;

    typedef BufferIterator<T>                       iterator;
    typedef ConstBufferIterator<T>                  const_iterator;
    typedef std::reverse_iterator<iterator>		    reverse_iterator;
    typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;

    typedef typename impl::BufferBase::size_type        size_type;
    typedef typename impl::BufferBase::difference_type  difference_type;

    Buffer(const Allocator& allocator = Allocator()) noexcept;
    Buffer(size_t bufferSize, const Allocator& allocator = Allocator());
    Buffer(size_t bufferSize, const T& init, const Allocator& allocator = Allocator());

    Buffer(std::initializer_list<T> il, const Allocator& allocator = Allocator());

    Buffer(const Buffer& rhs);
    Buffer(Buffer&& rhs) noexcept;

    ~Buffer() noexcept;

    size_t size() const noexcept;
    void resize(size_t n);

    bool empty() const noexcept;
    bool full() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    reverse_iterator rbegin() noexcept;
    const_iterator cbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    reverse_iterator rend() noexcept;
    const_iterator cend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    T& at(size_t index);
    const T& at(size_t index) const; // TODO

    T& front() noexcept;
    const T& front() const noexcept; // TODO

    T& back() noexcept;
    const T& back() const noexcept; // TODO

    void pop_front() noexcept;
    void push_front(const T& value) noexcept;
    void push_front(T&& value) noexcept;

    void pop_back() noexcept;
    void push_back(const T& value) noexcept;
    void push_back(T&& value) noexcept;

    iterator insert(iterator position, const T& value) noexcept;
    iterator insert(iterator position, T&& value) noexcept;
    iterator insert(iterator position, size_t n, const T& value) noexcept;
    iterator insert(iterator position, size_t n, T&& value) noexcept;
    iterator insert(iterator position, iterator i, iterator j) noexcept;
    iterator insert(iterator position, std::initializer_list<T> il) noexcept; // TODO define
    
    // TODO
    template<typename... Args>
    iterator emplace(iterator position, Args&&... args) noexcept;

    template <typename... Args>
    void emplace_front(Args&&... args) noexcept;

    template <typename... Args>
    void emplace_back(Args&&... args) noexcept;

    void assign(size_t count, const T& value);
    void assign(iterator i, iterator j);
    void assign(std::initializer_list<T> il);

    void swap(Buffer& rhs) noexcept;

    iterator erase(iterator position) noexcept;
    iterator erase(iterator position, iterator i, iterator j) noexcept;

    void clear() noexcept;

    Allocator get_allocator() noexcept;

    Buffer& operator=(const Buffer& rhs);
    Buffer& operator=(Buffer&& rhs);
    Buffer& operator=(std::initializer_list<T> il);

    bool operator!=(const Buffer& lhs) noexcept;

    T& operator[](size_t index) noexcept;

    template <typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator==(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs, 
                           const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept;

    template <typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator!=(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs, 
                          const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept;

private:

    T* AllocateBuffer(size_t n);

    void CopyAssignmentWithAllocatorPropogation(const Buffer& rhs, std::true_type t);
    void CopyAssignmentWithAllocatorPropogation(const Buffer& rhs, std::false_type f);

    void DeallocateBuffer() noexcept;

    template <typename Arg>
    iterator FillInsert(iterator position, size_t n, Arg&& value) noexcept;

    template <typename Arg>
    iterator FillInsertFront(size_t n, Arg&& value) noexcept;

    template <typename Arg>
    iterator FillInsertBack(size_t n, Arg&& value) noexcept;

    template <typename Arg>
    void FillInForward(iterator position, size_t n, Arg&& value) noexcept;

    template <typename Arg>
    iterator FillInReverse(iterator position, size_t n, Arg&& value) noexcept;

    void MoveAssignmentWithAllocatorPropogation(Buffer&& rhs, std::true_type t);
    void MoveAssignmentWithAllocatorPropogation(Buffer&& rhs, std::false_type f);
    void MoveMemoryContents(Buffer&& rhs);

    Allocator mAllocator;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(const Allocator& allocator) noexcept
    :   BufferBase(), mAllocator(allocator)
{ }

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, const Allocator& allocator)
    :   BufferBase(), mAllocator(allocator)
{
    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress = AllocateBuffer(bufferSize);
        mBufferSize = bufferSize;

        LOG("Buffer of size " << mBufferSize << " created at " << std::hex << (uint64_t)mStartAddress << std::dec)
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, const T& init, const Allocator& allocator)
    :   BufferBase(), mAllocator(allocator)
{
    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        // Set back of buffer to the end of the available space and raise full flag
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress = AllocateBuffer(bufferSize);
        mBufferSize = bufferSize;
        mBackOfBuffer = bufferSize - 1;
        mIsFullFlag = true;

        // Construct elements in the buffer
        // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
        for (int i = 0; i < mBufferSize; i++)
            alloc_traits::construct(mAllocator, (T*)mStartAddress + i, init);
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(std::initializer_list<T> il, const Allocator& allocator)
    :   BufferBase(), mAllocator(allocator)
{
    // Only continue if the initializer list contains some data
    size_t listSize = il.size();
    if (listSize > 0)
    {
        // Allocate buffer with enough spage to store the list
        mStartAddress = AllocateBuffer(listSize);
        mBufferSize = listSize;

        // Copy list contents to buffer
        for (const T& value : il)
            push_back(value);
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(const Buffer& rhs)
    :   BufferBase(), mAllocator(alloc_traits::select_on_container_copy_construction(rhs.mAllocator))
{
    // Only continue if the right hand side buffer is initialized and has max size > 0
    if ((rhs.mStartAddress != nullptr) && (rhs.mBufferSize > 0))
    {
        // Allocate storage for the container
        // Set back of buffer to the end of the available space and raise full flag
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress = AllocateBuffer(rhs.mBufferSize);
        mBufferSize = rhs.mBufferSize;

        // Copy elements from the right hand side to this
        const_iterator rhsCurr = rhs.cbegin();
        while (rhsCurr != rhs.cend())
            push_back(*rhsCurr++);
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer&& rhs) noexcept
    :   BufferBase(), mAllocator(std::move(rhs.mAllocator))
{
    // Copy start address and other members from the right hand side to this
    mStartAddress = rhs.mStartAddress;
    mBufferSize = rhs.mBufferSize;
    mBackOfBuffer = rhs.mBackOfBuffer;
    mIsFullFlag = rhs.mIsFullFlag;

    // Clear right hand side, including the start address
    rhs.mStartAddress = nullptr;
    rhs.clear();
}

template <typename T, class Allocator>
Buffer<T, Allocator>::~Buffer() noexcept
{
    DeallocateBuffer();
}

//  Public  -----------------------------------------------------------------------------------------------------------

template <typename T, class Allocator>
T& Buffer<T, Allocator>::at(size_t index)
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= mBufferSize))
    {
        // TODO Throw exception or enter hard fault trap
    }

    iterator it = begin();
    std::advance(it, index);
    return *it;
}

template <typename T, class Allocator>
const T& Buffer<T, Allocator>::at(size_t index) const
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= mBufferSize))
    {
        // TODO Throw exception or enter hard fault trap
    }

    const_iterator it = begin();
    std::advance(it, index);
    return *it;
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::back() noexcept
{
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    size_t lastPopulatedIndex = (full()) ? mBackOfBuffer : _M_roll_over_backward(mBackOfBuffer, 1);
    return *((T*)mStartAddress + lastPopulatedIndex);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::begin() noexcept
{
    if (empty())
        return iterator(*this, mBufferSize); // Return iterator to end instead (TODO there's probably a better way)

    return iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::begin() const noexcept
{
    if (empty())
        return const_iterator(*this, mBufferSize); // Return iterator to end instead (TODO there's probably a better way)

    return const_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cbegin() const noexcept
{
    if (empty())
        return const_iterator(*this, mBufferSize); // Return iterator to end instead

    return const_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cend() const noexcept
{
    return const_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::clear() noexcept
{
    mFrontOfBuffer = mBackOfBuffer = 0;
    mIsFullFlag = false;
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crbegin() const noexcept
{
    if (empty())
        return const_reverse_iterator(*this, mBufferSize); // Return iterator to end instead
        
    return const_reverse_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
bool Buffer<T, Allocator>::empty() const noexcept
{
    return (mFrontOfBuffer == mBackOfBuffer);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::end() noexcept
{
    return iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::end() const noexcept
{
    return const_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::front() noexcept
{
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress + mFrontOfBuffer);
}

template <typename T, class Allocator>
bool Buffer<T, Allocator>::full() const noexcept
{
    return mIsFullFlag;
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, const T& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillInsertFront(1, copyOfValue);
    }
    else if (position == end())
    {
        return FillInsertBack(1, copyOfValue);
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, copyOfValue);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, T&& value) noexcept
{    
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillInsertFront(1, std::forward<T>(value));
    }
    else if (position == end())
    {
        return FillInsertBack(1, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, std::forward<T>(value));
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
    Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, size_t n, const T& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillInsertFront(n, copyOfValue);
    }
    else if (position == end())
    {
        return FillInsertBack(n, copyOfValue);
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, n, copyOfValue);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::iterator 
Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position, size_t n, T&& value) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillInsertFront(n, std::forward<T>(value));
    }
    else if (position == end())
    {
        return FillInsertBack(n, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, n, std::forward<T>(value));
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
        // Move back of buffer backwards 1 step unless full and clear full flag
        if (!full())
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
        if (full())
        {
            mBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
            LOG("Moving back of buffer -> " << mBackOfBuffer)
        }

        // Clear the full flag
        mIsFullFlag = false;
    }
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_back(const T& value) noexcept
{
    LOG("Pushing back")

    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;
    FillInsertBack(1, copyOfValue);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_back(T&& value) noexcept
{
    LOG("Pushing back")
    FillInsertBack(1, std::forward<T>(value));
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_front(const T& value) noexcept
{
    LOG("Pushing front")

    // Make a copy of the reference value, it could exist in the buffer already and be affected by the 
    // moving/truncating of elements
    T copyOfValue = value;
    FillInsertFront(1, copyOfValue);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T&& value) noexcept
{
    LOG("Pushing front")
    FillInsertFront(1, std::forward<T>(value));
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rbegin() noexcept
{
    if (empty())
        return reverse_iterator(*this, mBufferSize); // Return iterator to end instead
        
    return reverse_iterator(*this);
}

template <typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rend() noexcept
{
    return reverse_iterator(*this, mBufferSize);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::resize(size_t n)
{
    T* newBuffer = AllocateBuffer(n);

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

    alloc_traits::deallocate(mAllocator, (T*)old, mBufferSize);
}

template <typename T, class Allocator>
size_t Buffer<T, Allocator>::size() const noexcept
{
    if (mIsFullFlag)
        return mBufferSize;

    return _M_wrap_index(mBackOfBuffer) - _M_wrap_index(mFrontOfBuffer);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::swap(Buffer& rhs) noexcept
{
    // Cache this buffer's memory contents
    bool tempIsFullFlag = mIsFullFlag;
    size_t tempFrontOfBuffer = mFrontOfBuffer;
    size_t tempBackOfBuffer = mBackOfBuffer;
    size_t tempBufferSize = mBufferSize;
    T* tempStartAddress = (T*)mStartAddress;

    // Copy memory contents from right hand side to here
    mIsFullFlag = rhs.mIsFullFlag;
    mFrontOfBuffer = rhs.mFrontOfBuffer;
    mBackOfBuffer = rhs.mBackOfBuffer;
    mBufferSize = rhs.mBufferSize;
    mStartAddress = rhs.mStartAddress;

    // Copy cached memory contents to right hand side
    rhs.mIsFullFlag = tempIsFullFlag;
    rhs.mFrontOfBuffer = tempFrontOfBuffer;
    rhs.mBackOfBuffer = tempBackOfBuffer;
    rhs.mBufferSize = tempBufferSize;
    rhs.mStartAddress = tempStartAddress;

    // If Allocator requires that it be propogated on container swap, do so now
    if (alloc_traits::propagate_on_container_swap::value == true)
    {
        Allocator tempAllocator = mAllocator;
        mAllocator = rhs.mAllocator;
        rhs.mAllocator = tempAllocator;
    }
}

template <typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(const Buffer<T, Allocator>& rhs)
{
    // Tag-dispatch the appropriate routine, selected at compile time
    CopyAssignmentWithAllocatorPropogation(std::forward<const Buffer<T>>(rhs), 
                                       typename alloc_traits::propagate_on_container_copy_assignment{});
    
    return *this;
}

template <typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(Buffer<T, Allocator>&& rhs)
{
    // Tag-dispatch the appropriate routine, selected at compile time
    MoveAssignmentWithAllocatorPropogation(std::forward<Buffer<T>>(rhs),
                                       typename alloc_traits::propagate_on_container_move_assignment{});

    return *this;
}

template <typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(std::initializer_list<T> il)
{

}

template <typename T, class Allocator>
T& Buffer<T, Allocator>::operator[](size_t index) noexcept
{
    // Offset the index from the start of buffer
    index = _M_roll_over_forward(mFrontOfBuffer, index);
    
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    return *((T*)mStartAddress + index);
}

//  Private -----------------------------------------------------------------------------------------------------------

template <typename T, class Allocator>
T* Buffer<T, Allocator>::AllocateBuffer(size_t n)
{
    T* bufferPtr = alloc_traits::allocate(mAllocator, n);
    if (bufferPtr == nullptr)
    {
        // TODO Throw exception or enter hard fault trap
    }

    return bufferPtr;
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignmentWithAllocatorPropogation(const Buffer<T, Allocator>& rhs, std::true_type t)
{
    // Deallocate current buffer, the lhs allocator may not be able to later
    DeallocateBuffer();

    mAllocator = rhs.mAllocator;

    // Allocate new buffer with new allocator
    // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
    mStartAddress = AllocateBuffer(rhs.mBufferSize);
    mBufferSize = rhs.mBufferSize;

    // Copy the buffer contents
    for (const T& rhsValue : rhs)
        push_back(rhsValue);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignmentWithAllocatorPropogation(const Buffer<T, Allocator>& rhs, std::false_type f)
{
    if (mBufferSize != rhs.mBufferSize)
    {    
        // Deallocate current buffer, the lhs allocator may not be able to later
        DeallocateBuffer();

        // Allocate new buffer with new allocator
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress = AllocateBuffer(rhs.mBufferSize);
        mBufferSize = rhs.mBufferSize;
    }
    else
        clear(); // Just clear the current buffer

    // Copy the buffer contents
    for (const T& rhsValue : rhs)
        push_back(rhsValue);
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::DeallocateBuffer() noexcept
{
    // Only continue if the start address is valid
    if (mStartAddress)
    {    
        // Destroy only elements that are initialized and stored in the buffer
        if (!empty())
        {
            size_t curr = mFrontOfBuffer;
            while (curr != mBufferSize)
            {
                alloc_traits::destroy(mAllocator, (T*)mStartAddress + curr);
                curr = _M_move_index_forward(curr, 1);
            }

            clear(); // Reset tracking indexes and flags in case buffer is still in use
        }

        // Deallocate buffer
        // Need to typecast 'mStartAddress' from void*
        alloc_traits::deallocate(mAllocator, (T*)mStartAddress, mBufferSize);
    }
}

template <typename T, class Allocator>
template <typename Arg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInsert(Buffer<T, Allocator>::iterator position,
                                                                             size_t n, Arg&& value) noexcept
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
    FillInForward(position, n, std::forward<Arg>(value));

    return position;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInsertBack(size_t n, Arg&& value) noexcept
{
    LOG("Inserting " << n << " element[s] back")

    // Guard against inserts larger than the buffer
    if (n > mBufferSize)
        n = mBufferSize;

    size_t insertSteps = (full()) ? n : n - 1;
    size_t endOfInsertIndex = _M_roll_over_forward(mBackOfBuffer, insertSteps);
    LOG("Post-insert back of buffer -> " << endOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(endOfInsertIndex) < _M_wrap_index(mBackOfBuffer))
    {
        // Increment front of buffer 1 past the end of insert index to accomodate new data
        mFrontOfBuffer = _M_roll_over_forward(endOfInsertIndex, 1);
        mIsFullFlag = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }

    // Fill the insert zone in reverse.
    // This takes advantage of _M_move_index_backward() which is called by BufferIterator::operator--() and does not
    // care if the iterator's index is past the back of buffer so long as it is not the literal end-iterator.
    LOG("Starting insert")
    iterator insertZoneEnd(*this, endOfInsertIndex);
    iterator insertZoneStart = FillInReverse(insertZoneEnd, n, std::forward<Arg>(value));

    // Now update back of buffer
    size_t newBackOfBuffer = _M_roll_over_forward(endOfInsertIndex, 1);
    if (newBackOfBuffer == mFrontOfBuffer)
    {
        mBackOfBuffer = endOfInsertIndex;
        mIsFullFlag = true;
        LOG("Setting full flag")
    }
    else
        mBackOfBuffer = newBackOfBuffer;

    return insertZoneStart;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInsertFront(size_t n, Arg&& value) noexcept
{
    LOG("Inserting " << n << " element[s] front")

    // Guard against inserts larger than the buffer
    if (n > mBufferSize)
        n = mBufferSize;

    // Establish the start and end of the insert zone
    // End should be before the front of buffer unless it is empty
    // Start is n - 1 steps ahead as the end index counts towards 1 element in the insert
    size_t endOfInsertIndex = (empty()) ? mFrontOfBuffer : _M_roll_over_backward(mFrontOfBuffer, 1);
    size_t startOfInsertIndex = _M_roll_over_backward(endOfInsertIndex, n - 1);
    LOG("Inserting from " << startOfInsertIndex << " -> " << endOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(startOfInsertIndex) <= _M_wrap_index(mBackOfBuffer))
    {
        // Decrement the back of buffer 1 past the start of insert index to accomodate new data
        mBackOfBuffer = _M_roll_over_backward(startOfInsertIndex, 1);
        mIsFullFlag = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Fill the insert zone in reverse
    // See comments in _M_insert_fill_front() for an explanation as to why
    LOG("Starting insert")
    iterator insertZoneEnd(*this, endOfInsertIndex);
    iterator insertZoneStart = FillInReverse(insertZoneEnd, n, std::forward<Arg>(value));

    // Now update front of buffer
    mFrontOfBuffer = startOfInsertIndex;
    if (_M_roll_over_backward(mFrontOfBuffer, 1) == mBackOfBuffer)
    {
        LOG("Setting full flag")
        mIsFullFlag = true;
    }

    return insertZoneStart;
}

template <typename T, class Allocator>
template <typename Arg> // T& or T&&
void Buffer<T, Allocator>::FillInForward(typename Buffer<T, Allocator>::iterator position, size_t n, Arg&& value) noexcept
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
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInReverse(
                                                                typename Buffer<T, Allocator>::iterator position,
                                                                size_t n, Arg&& value) noexcept
{
    bool sentinelReached = false;
    size_t count = 0;
    while (!sentinelReached)
    {
        *position = value;
        count++;
        LOG("Assigned " << *position << " to buffer element")
        if (count < n)
            position--;
        else
            sentinelReached = true;
    }
    
    return position;
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::MoveMemoryContents(Buffer<T, Allocator>&& rhs)
{
    // Copy start address and other members from the right hand side to this
    mStartAddress = rhs.mStartAddress;
    mBufferSize = rhs.mBufferSize;
    mBackOfBuffer = rhs.mBackOfBuffer;
    mIsFullFlag = rhs.mIsFullFlag;

    // Clear right hand side, including the start address
    rhs.mStartAddress = nullptr;
    rhs.clear();
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignmentWithAllocatorPropogation(Buffer<T, Allocator>&& rhs, std::true_type t)
{
    // Deallocate the current buffer, move the allocator from the right hand side to here, and move memory contents
    DeallocateBuffer();
    mAllocator = std::move(rhs.mAllocator);
    MoveMemoryContents(std::forward<Buffer<T>>(rhs));
}

template <typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignmentWithAllocatorPropogation(Buffer<T, Allocator>&& rhs, std::false_type f)
{
    if (mAllocator == rhs.mAllocator)
    {
        // Deallocate the current buffer and move memory contents
        DeallocateBuffer();
        MoveMemoryContents(std::forward<Buffer<T>>(rhs));
    }
    else
    {
        // TODO Throw exception or enter hard fault trap
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Global method definitions in alphabetical order    ////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename ValueTypeArg, typename AllocatorTypeArg = std::allocator<ValueTypeArg>>
bool operator==(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs, 
                const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept
{
    using iterator = typename Buffer<ValueTypeArg, AllocatorTypeArg>::iterator;
    using const_iterator = typename Buffer<ValueTypeArg, AllocatorTypeArg>::const_iterator;

    // Compare sizes first
    if (rhs.size() == lhs.size())
    {
        // Compare buffer contents
        const_iterator rhsCurr = rhs.cbegin();
        for (const ValueTypeArg& lhsVal : lhs)
        {
            if (*rhsCurr != lhsVal)
                return false;
        }

        // If here, the contents of the buffers are equivalent
        return true;
    }

    return false;
}

template <typename ValueTypeArg, typename AllocatorTypeArg = std::allocator<ValueTypeArg>>
bool operator!=(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs, 
                const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept
{
    return !(rhs == lhs);
}

} // End namespace shmit

namespace std
{

template <typename T, class Allocator>
void swap(shmit::Buffer<T, Allocator>& rhs, shmit::Buffer<T, Allocator>& lhs) noexcept
{
    rhs.swap(lhs);
}

} // End namespace std
