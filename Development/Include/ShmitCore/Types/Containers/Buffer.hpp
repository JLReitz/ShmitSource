#pragma once

#include "BufferImpl.hpp"

#include <ShmitCore/Logging/Diagnostics/Logging.hpp>
#include <ShmitCore/Logging/Events/Logging.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <iterator>
#include <type_traits>

#define LOG(line) std::cout << line << std::endl;

namespace shmit
{

template<typename T, class Allocator = std::allocator<T>>
class Buffer : public impl::BufferBase, Named
{
    using alloc_traits = __gnu_cxx::__alloc_traits<Allocator>;

public:
    DIAGNOSTIC_CONTEXT(shmit::Buffer)
    DIAGNOSTIC_POSIT(Constructing, shmit::log::Level::eDebug, shmit::log::diagnostics::NoData())
    DIAGNOSTIC_POSIT(Destructing, shmit::log::Level::eDebug, shmit::log::diagnostics::NoData())
    DIAGNOSTIC_POSIT(Emplace_Back, shmit::log::Level::eDebug, shmit::log::diagnostics::Count())
    DIAGNOSTIC_POSIT(Emplace_Front, shmit::log::Level::eDebug, shmit::log::diagnostics::Count())
    DIAGNOSTIC_POSIT(Emplace_Random, shmit::log::Level::eDebug, shmit::log::diagnostics::Count())

    using value_type      = typename alloc_traits::value_type;
    using pointer         = typename alloc_traits::pointer;
    using const_pointer   = typename alloc_traits::const_pointer;
    using reference       = typename alloc_traits::reference;
    using const_reference = typename alloc_traits::const_reference;

    using allocator_type = Allocator;

    using iterator               = BufferIterator<T>;
    using const_iterator         = ConstBufferIterator<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type       = typename impl::BufferBase::size_type;
    using difference_type = typename impl::BufferBase::difference_type;

    Buffer(char const* name = nullptr, const Allocator& allocator = Allocator()) noexcept;
    Buffer(size_t bufferSize, char const* name = nullptr, const Allocator& allocator = Allocator());
    Buffer(size_t bufferSize, const T& init, char const* name = nullptr, const Allocator& allocator = Allocator());

    Buffer(std::initializer_list<T> il, char const* name = nullptr, const Allocator& allocator = Allocator());

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    Buffer(IteratorTypeArg i, IteratorTypeArg j, char const* name = nullptr, const Allocator& allocator = Allocator());

    Buffer(const Buffer& rhs);
    Buffer(Buffer&& rhs) noexcept;

    constexpr Buffer(T* const ext, size_t bufferSize);

    ~Buffer() noexcept;

    size_t size() const noexcept;
    void   resize(size_t n);

    bool empty() const noexcept;
    bool full() const noexcept;

    iterator               begin() noexcept;
    const_iterator         begin() const noexcept;
    reverse_iterator       rbegin() noexcept;
    const_iterator         cbegin() const noexcept;
    const_reverse_iterator crbegin() const noexcept;

    iterator               end() noexcept;
    const_iterator         end() const noexcept;
    reverse_iterator       rend() noexcept;
    const_iterator         cend() const noexcept;
    const_reverse_iterator crend() const noexcept;

    T&       at(size_t index);
    const T& at(size_t index) const;

    T&       front() noexcept;
    const T& front() const noexcept;

    T&       back() noexcept;
    const T& back() const noexcept;

    void assign(size_t count, const T& value);
    void assign(std::initializer_list<T> il);

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    void assign(IteratorTypeArg i, IteratorTypeArg j);

    template<typename... Args>
    iterator emplace(iterator position, Args&&... args) noexcept;

    template<typename... Args>
    void emplace_front(Args&&... args) noexcept;

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept;

    iterator insert(iterator position, const T& value) noexcept;
    iterator insert(iterator position, T&& value) noexcept;
    iterator insert(iterator position, size_t n, const T& value) noexcept;
    iterator insert(iterator position, std::initializer_list<T> il) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    iterator insert(iterator position, IteratorTypeArg i, IteratorTypeArg j) noexcept;

    void pop_front() noexcept;
    void push_front(const T& value) noexcept;
    void push_front(T&& value) noexcept;

    void pop_back() noexcept;
    void push_back(const T& value) noexcept;
    void push_back(T&& value) noexcept;

    void swap(Buffer& rhs) noexcept;

    iterator erase(iterator position) noexcept;
    iterator erase(iterator position, iterator i, iterator j) noexcept;

    void clear() noexcept;

    Buffer& operator=(const Buffer& rhs);
    Buffer& operator=(Buffer&& rhs);
    Buffer& operator=(std::initializer_list<T> il);

    bool operator!=(const Buffer& lhs) noexcept;

    T&       operator[](size_t index) noexcept;
    const T& operator[](size_t index) const noexcept;

    template<typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator==(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs,
                           const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept;

    template<typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator!=(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs,
                           const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept;

private:
    T*   AllocateBuffer(size_t n);
    void DeallocateBuffer() noexcept;

    void CopyAssignBufferWithAllocatorPropogation(const Buffer& rhs, std::true_type t);
    void CopyAssignBufferWithAllocatorPropogation(const Buffer& rhs, std::false_type f);

    void CopyAssignElement(size_t position, const T& value);

    void MoveAssignBufferWithAllocatorPropogation(Buffer&& rhs, std::true_type t);
    void MoveAssignBufferWithAllocatorPropogation(Buffer&& rhs, std::false_type f);
    void MoveMemoryContents(Buffer&& rhs);

    template<typename Arg>
    iterator FillInsert(iterator position, size_t n, Arg&& value) noexcept;

    template<typename IteratorTypeArg>
    iterator RangeInsert(iterator position, IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename Arg>
    iterator FillPushFront(size_t n, Arg&& value) noexcept;

    template<typename Arg>
    iterator FillPushBack(size_t n, Arg&& value) noexcept;

    template<typename IteratorTypeArg>
    iterator RangePushFront(IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename IteratorTypeArg>
    iterator RangePushBack(IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename Arg>
    void FillInForward(iterator position, size_t n, Arg&& value) noexcept;

    template<typename Arg>
    iterator FillInReverse(iterator position, size_t n, Arg&& value) noexcept;

    void PrepareForRandomInsert(iterator position, size_t n);

    Allocator mAllocator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer Constructor definitions      ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, const Allocator& allocator) noexcept
    : BufferBase(), Named(name), mAllocator(allocator)
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, char const* name, const Allocator& allocator)
    : BufferBase(), Named(name), mAllocator(allocator)
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress  = AllocateBuffer(bufferSize);
        mBufferMaxSize = bufferSize;
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_t bufferSize, const T& init, char const* name, const Allocator& allocator)
    : BufferBase(), Named(name), mAllocator(allocator)
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    // Only continue if the given buffer size is > 0
    if (bufferSize > 0)
    {
        // Allocate storage for the container
        // Set back of buffer to the end of the available space and raise full flag
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress  = AllocateBuffer(bufferSize);
        mBufferMaxSize = bufferSize;
        mBackOfBuffer  = bufferSize - 1;
        mIsFullFlag    = true;

        // Construct elements in the buffer
        // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
        // TODO replace with emplace-fill routine eventually
        for (int i = 0; i < mBufferMaxSize; i++)
            alloc_traits::construct(mAllocator, (T*)mStartAddress + i, init);
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(std::initializer_list<T> il, char const* name, const Allocator& allocator)
    : BufferBase(), Named(name), mAllocator(allocator)
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    // Only continue if the initializer list contains some data
    size_t listSize = il.size();
    if (listSize > 0)
    {
        // Allocate buffer with enough spage to store the list
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress  = AllocateBuffer(listSize);
        mBufferMaxSize = listSize;

        // Copy list contents to buffer
        for (const T& value : il)
            push_back(value);
    }
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
Buffer<T, Allocator>::Buffer(IteratorTypeArg i, IteratorTypeArg j, char const* name, const Allocator& allocator)
    : BufferBase(), Named(name), mAllocator(allocator)
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    size_t rangeSize = j - i;
    if (rangeSize > 0)
    {
        // Allocate buffer with enough spage to store the range
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress  = AllocateBuffer(rangeSize);
        mBufferMaxSize = rangeSize;

        // Copy range contents to buffer
        while (i != j)
            push_back(*i++);
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(const Buffer& rhs)
    : BufferBase(), mAllocator(alloc_traits::select_on_container_copy_construction(rhs.mAllocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    // Only continue if the right hand side buffer is initialized and has max size > 0
    if ((rhs.mStartAddress != nullptr) && (rhs.mBufferMaxSize > 0))
    {
        // Allocate storage for the container
        // Set back of buffer to the end of the available space and raise full flag
        // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
        mStartAddress  = AllocateBuffer(rhs.mBufferMaxSize);
        mBufferMaxSize = rhs.mBufferMaxSize;

        // Copy elements from the right hand side to this
        const_iterator rhsCurr = rhs.cbegin();
        while (rhsCurr != rhs.cend())
            push_back(*rhsCurr++);
    }

    // Copy name from rhs to this
    char const* rhs_name = rhs.GetName();
    SetName(rhs_name);
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer&& rhs) noexcept : BufferBase(), mAllocator(std::move(rhs.mAllocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Constructing)

    // Copy start address and other members from the right hand side to this
    mStartAddress  = rhs.mStartAddress;
    mBufferMaxSize = rhs.mBufferMaxSize;
    mBackOfBuffer  = rhs.mBackOfBuffer;
    mIsFullFlag    = rhs.mIsFullFlag;

    // Clear right hand side, including the start address
    rhs.mStartAddress = nullptr;
    rhs.clear();

    // Copy name from rhs to this
    char const* rhs_name = rhs.GetName();
    SetName(rhs_name);
}

template<typename T, class Allocator>
Buffer<T, Allocator>::~Buffer() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Destructing)
    DeallocateBuffer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer method definitions in alphabetical order     ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename T, class Allocator>
void Buffer<T, Allocator>::assign(size_t count, const T& value)
{
    // Replace current elements with the value
    iterator curr = begin();
    while ((curr != end()) && count)
    {
        *curr++ = value;
        count--;
    }

    // Push back the remainder of the count or spaces left in the buffer, whichever is smaller
    size_t spacesLeft = mBufferMaxSize - size();
    count             = (spacesLeft >= count) ? count : spacesLeft;
    FillPushBack(count, value);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::assign(std::initializer_list<T> il)
{
    size_t listSize          = il.size();
    size_t listReductionBias = 0;
    if (listSize > mBufferMaxSize)
    {
        // If the range is larger than the size of the buffer, reduce it to fit
        listReductionBias = listSize - mBufferMaxSize;
    }

    typename std::initializer_list<T>::const_iterator i = il.begin();
    typename std::initializer_list<T>::const_iterator j = il.end() - listReductionBias;

    // Replace current elements with those in the range
    iterator curr = begin();
    while ((curr != end()) && (i != j))
        *curr++ = *i++;

    // Push back the remainder of the range
    RangePushBack(i, j);
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
void Buffer<T, Allocator>::assign(IteratorTypeArg i, IteratorTypeArg j)
{
    // Guard against overflow
    size_t rangeSize = j - i;
    if (rangeSize > mBufferMaxSize)
    {
        // If the range is larger than the size of the buffer, reduce it to fit
        size_t diff = rangeSize - mBufferMaxSize;
        j -= diff;
    }

    // Replace current elements with those in the range
    iterator curr = begin();
    while ((curr != end()) && (i != j))
        *curr++ = *i++;

    // Push back the remainder of the range
    RangePushBack(i, j);
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::at(size_t index)
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= mBufferMaxSize))
    {
        // TODO Throw exception or enter hard fault trap
    }

    iterator it = begin();
    std::advance(it, index);
    return *it;
}

template<typename T, class Allocator>
const T& Buffer<T, Allocator>::at(size_t index) const
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= mBufferMaxSize))
    {
        // TODO Throw out_of_range exception or enter hard fault trap
    }

    const_iterator it = cbegin();
    std::advance(it, index);
    return *it;
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::back() noexcept
{
    return *(end() - 1);
}

template<typename T, class Allocator>
const T& Buffer<T, Allocator>::back() const noexcept
{
    return *(cend() - 1);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::begin() noexcept
{
    if (empty())
        return iterator(*this,
                        mBufferMaxSize); // Return iterator to end instead (TODO there's probably a better way)

    return iterator(*this);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::begin() const noexcept
{
    if (empty())
        return const_iterator(*this,
                              mBufferMaxSize); // Return iterator to end instead (TODO there's probably a better way)

    return const_iterator(*this);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cbegin() const noexcept
{
    if (empty())
        return const_iterator(*this, mBufferMaxSize); // Return iterator to end instead

    return const_iterator(*this);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cend() const noexcept
{
    return const_iterator(*this, mBufferMaxSize);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::clear() noexcept
{
    // Iterator positions will remain constant by retaining the front of buffer
    mBackOfBuffer = mFrontOfBuffer;
    mIsFullFlag   = false;
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crbegin() const noexcept
{
    if (empty())
        return const_reverse_iterator(*this, mBufferMaxSize); // Return iterator to end instead

    return const_reverse_iterator(*this);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(*this, mBufferMaxSize);
}

template<typename T, class Allocator>
template<typename... Args>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::emplace(typename Buffer<T, Allocator>::iterator position,
                                                                      Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Emplace_Random)

    PrepareForRandomInsert(position, 1);
    alloc_traits::construct(mAllocator, &(*position), std::forward<Args>(args)...);

    return position;
}

template<typename T, class Allocator>
template<typename... Args>
void Buffer<T, Allocator>::emplace_back(Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Emplace_Back)

    size_t emplaceStep  = (full()) ? 1 : 0;
    size_t emplaceIndex = _M_roll_over_forward(mBackOfBuffer, emplaceStep);

    // Check for overflow
    if (emplaceIndex == mFrontOfBuffer)
    {
        // Increment front of buffer forward 1 step to accomodate new data
        mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, 1);
        mIsFullFlag    = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }

    // Emplace element
    LOG("Starting emplacement")
    iterator emplacePosition(*this, emplaceIndex);
    alloc_traits::construct(mAllocator, &(*emplacePosition), std::forward<Args>(args)...);

    // Now update back of buffer
    size_t newBackOfBuffer = _M_roll_over_forward(emplaceIndex, 1);
    if (newBackOfBuffer == mFrontOfBuffer) // Re-do this check in case the buffer has been accessed in the meantime
    {
        mBackOfBuffer = emplaceIndex;
        mIsFullFlag   = true;
        LOG("Setting full flag")
    }
    else
        mBackOfBuffer = newBackOfBuffer;
}

template<typename T, class Allocator>
template<typename... Args>
void Buffer<T, Allocator>::emplace_front(Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(Emplace_Front)

    size_t emplaceIndex = _M_roll_over_backward(mFrontOfBuffer, 1);

    // Check for overflow
    if (full())
    {
        // Decrement the back of buffer forward 1 step to accomodate new data
        mBackOfBuffer = _M_roll_over_backward(mBackOfBuffer, 1);
        mIsFullFlag   = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Emplace element
    LOG("Starting emplacement")
    iterator emplacePosition(*this, emplaceIndex);
    alloc_traits::construct(mAllocator, &(*emplacePosition), std::forward<Args>(args)...);

    // Now update front of buffer
    mFrontOfBuffer = emplaceIndex;
    if (_M_roll_over_backward(mFrontOfBuffer, 1) == mBackOfBuffer)
    {
        LOG("Setting full flag")
        mIsFullFlag = true;
    }
}

template<typename T, class Allocator>
bool Buffer<T, Allocator>::empty() const noexcept
{
    return (mFrontOfBuffer == mBackOfBuffer);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::end() noexcept
{
    return iterator(*this, mBufferMaxSize);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::end() const noexcept
{
    return const_iterator(*this, mBufferMaxSize);
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::front() noexcept
{
    return *begin();
}

template<typename T, class Allocator>
const T& Buffer<T, Allocator>::front() const noexcept
{
    return *cbegin();
}

template<typename T, class Allocator>
bool Buffer<T, Allocator>::full() const noexcept
{
    return mIsFullFlag;
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     const T& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
        return FillPushFront(1, copyOfValue);
    else if (position == end())
        return FillPushBack(1, copyOfValue);

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, copyOfValue);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     T&& value) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillPushFront(1, std::forward<T>(value));
    }
    else if (position == end())
    {
        return FillPushBack(1, std::forward<T>(value));
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, std::forward<T>(value));
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     size_t n, const T& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return FillPushFront(n, copyOfValue);
    }
    else if (position == end())
    {
        return FillPushBack(n, copyOfValue);
    }

    // Otherwise call the random-access insert routine
    return FillInsert(position, n, copyOfValue);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     std::initializer_list<T> il) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return RangePushFront(il.begin(), il.end());
    }
    else if (position == end())
    {
        return RangePushBack(il.begin(), il.end());
    }

    // Otherwise call the random-access insert routine
    return RangeInsert(position, il.begin(), il.end());
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     IteratorTypeArg i, IteratorTypeArg j) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
    {
        return RangePushFront(i, j);
    }
    else if (position == end())
    {
        return RangePushBack(i, j);
    }

    // Otherwise call the random-access insert routine
    return RangeInsert(position, i, j);
}

template<typename T, class Allocator>
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

template<typename T, class Allocator>
void Buffer<T, Allocator>::pop_front() noexcept
{
    LOG("Popping front")
    // Check for empty condition
    if (!empty())
    {
        bool isFullAtStart = full();

        // Move front of buffer fowards 1 step
        mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, 1);
        LOG("Moving front of buffer -> " << mFrontOfBuffer)

        // If the buffer is full, the back of buffer must be advanced to the now vacant position
        if (isFullAtStart)
        {
            mBackOfBuffer = _M_roll_over_forward(mBackOfBuffer, 1);
            LOG("Moving back of buffer -> " << mBackOfBuffer)
        }

        // Clear the full flag
        mIsFullFlag = false;
    }
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_back(const T& value) noexcept
{
    LOG("Pushing back")
    FillPushBack(1, value);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_back(T&& value) noexcept
{
    LOG("Pushing back")
    FillPushBack(1, std::forward<T>(value));
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_front(const T& value) noexcept
{
    LOG("Pushing front")
    FillPushFront(1, value);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T&& value) noexcept
{
    LOG("Pushing front")
    FillPushFront(1, std::forward<T>(value));
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rbegin() noexcept
{
    if (empty())
        return reverse_iterator(*this, mBufferMaxSize); // Return iterator to end instead

    return reverse_iterator(*this);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rend() noexcept
{
    return reverse_iterator(*this, mBufferMaxSize);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::resize(size_t n)
{
    T* newBuffer = AllocateBuffer(n);

    // Copy current buffer data to the new buffer
    size_t   destinationIndex = mFrontOfBuffer;
    iterator currentIt        = begin();
    iterator endIt            = end();
    while (currentIt != endIt)
    {
        newBuffer[destinationIndex] = *currentIt;
        currentIt++;
        destinationIndex = _M_roll_over_forward(destinationIndex, 1);
    }

    // Swap old buffer start address for new
    T* old         = (T*)mStartAddress;
    mStartAddress  = newBuffer;
    mBufferMaxSize = n;

    alloc_traits::deallocate(mAllocator, (T*)old, mBufferMaxSize);
}

template<typename T, class Allocator>
size_t Buffer<T, Allocator>::size() const noexcept
{
    if (mIsFullFlag)
        return mBufferMaxSize;

    return _M_wrap_index(mBackOfBuffer) - _M_wrap_index(mFrontOfBuffer);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::swap(Buffer& rhs) noexcept
{
    // Cache this buffer's memory contents
    bool   tempIsFullFlag    = mIsFullFlag;
    size_t tempFrontOfBuffer = mFrontOfBuffer;
    size_t tempBackOfBuffer  = mBackOfBuffer;
    size_t tempBufferSize    = mBufferMaxSize;
    T*     tempStartAddress  = (T*)mStartAddress;

    // Copy memory contents from right hand side to here
    mIsFullFlag    = rhs.mIsFullFlag;
    mFrontOfBuffer = rhs.mFrontOfBuffer;
    mBackOfBuffer  = rhs.mBackOfBuffer;
    mBufferMaxSize = rhs.mBufferMaxSize;
    mStartAddress  = rhs.mStartAddress;

    // Copy cached memory contents to right hand side
    rhs.mIsFullFlag    = tempIsFullFlag;
    rhs.mFrontOfBuffer = tempFrontOfBuffer;
    rhs.mBackOfBuffer  = tempBackOfBuffer;
    rhs.mBufferMaxSize = tempBufferSize;
    rhs.mStartAddress  = tempStartAddress;

    // If Allocator requires that it be propogated on container swap, do so now
    if (alloc_traits::propagate_on_container_swap::value == true)
    {
        Allocator tempAllocator = mAllocator;
        mAllocator              = rhs.mAllocator;
        rhs.mAllocator          = tempAllocator;
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(const Buffer<T, Allocator>& rhs)
{
    // Tag-dispatch the appropriate routine, selected at compile time
    CopyAssignBufferWithAllocatorPropogation(rhs, typename alloc_traits::propagate_on_container_copy_assignment {});

    return *this;
}

template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(Buffer<T, Allocator>&& rhs)
{
    // Tag-dispatch the appropriate routine, selected at compile time
    MoveAssignBufferWithAllocatorPropogation(std::forward<Buffer<T, Allocator>>(rhs),
                                             typename alloc_traits::propagate_on_container_move_assignment {});

    return *this;
}

template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(std::initializer_list<T> il)
{
    size_t ilSize = il.size();
    if (mBufferMaxSize != ilSize)
        resize(ilSize);
    else
        clear();

    for (T value : il)
        push_back(value);

    return *this;
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::operator[](size_t index) noexcept
{
    iterator it = begin();
    std::advance(it, index);
    return *it;
}

template<typename T, class Allocator>
const T& Buffer<T, Allocator>::operator[](size_t index) const noexcept
{
    const_iterator it = cbegin();
    std::advance(it, index);
    return *it;
}

//  Private ============================================================================================================

template<typename T, class Allocator>
T* Buffer<T, Allocator>::AllocateBuffer(size_t n)
{
    T* bufferPtr = alloc_traits::allocate(mAllocator, n);
    if (bufferPtr == nullptr)
    {
        // TODO Throw exception or enter hard fault trap
    }

    return bufferPtr;
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignBufferWithAllocatorPropogation(const Buffer<T, Allocator>& rhs, std::true_type t)
{
    // Deallocate current buffer, the lhs allocator may not be able to later
    DeallocateBuffer();

    mAllocator = rhs.mAllocator;

    // Allocate new buffer with new allocator
    // _M_allocate_buffer() is guaranteed to return a valid address or throw an exception
    mStartAddress  = AllocateBuffer(rhs.mBufferMaxSize);
    mBufferMaxSize = rhs.mBufferMaxSize;

    // Copy the buffer contents
    for (const T& rhsValue : rhs)
        push_back(rhsValue);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignBufferWithAllocatorPropogation(const Buffer<T, Allocator>& rhs, std::false_type f)
{
    if (mBufferMaxSize != rhs.mBufferMaxSize)
        resize(rhs.mBufferMaxSize);
    else
        clear(); // Just clear the current buffer

    // Copy the buffer contents
    for (const T& rhsValue : rhs)
        push_back(rhsValue);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignElement(size_t position, const T& value)
{
    // Need to typecast 'mStartAddress' from void* before performing pointer arithmetic
    *((T*)mStartAddress + _M_unwrap_index(position)) = value;
    LOG("Assigned " << *((T*)mStartAddress + _M_unwrap_index(position)) << " to position " << position)
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::DeallocateBuffer() noexcept
{
    // Only continue if the start address is valid
    if (mStartAddress)
    {
        // Destroy only elements that are initialized and stored in the buffer
        if (!empty())
        {
            size_t curr = mFrontOfBuffer;
            while (curr != mBufferMaxSize)
            {
                alloc_traits::destroy(mAllocator, (T*)mStartAddress + curr);
                curr = _M_move_index_forward(curr, 1);
            }

            clear(); // Reset tracking indexes and flags in case buffer is still in use
        }

        // Deallocate buffer
        // Need to typecast 'mStartAddress' from void*
        alloc_traits::deallocate(mAllocator, (T*)mStartAddress, mBufferMaxSize);
    }
}

template<typename T, class Allocator>
template<typename Arg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInsert(Buffer<T, Allocator>::iterator position,
                                                                         size_t n, Arg&& value) noexcept
{
    // LOG("Inserting " << n << " element[s]")
    LOG_INFO_EVENT("Buffer", "fill-insert", "Inserting %d elements", n)

    if (n >= mBufferMaxSize)
    {
        LOG_WARNING_EVENT_STRING("Buffer", "fill-insert", "Buffer overfill detected")

        // Fill in the entire buffer starting and set the full flag
        size_t mBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1); // Back of buffer sits behind front when full
        mIsFullFlag          = true;

        LOG("Moving back of buffer -> " << mBackOfBuffer)
        LOG("Setting full flag")

        // Cap n at the buffer size, anything over will be redundant
        n = mBufferMaxSize;
    }
    else
        PrepareForRandomInsert(position, n);

    // Fill in the insert zone
    LOG("Filling insert zone")
    FillInForward(position, n, std::forward<Arg>(value));

    return position;
}

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillPushBack(size_t n, Arg&& value) noexcept
{
    LOG("Inserting " << n << " element[s] back")

    // Guard against inserts larger than the buffer
    if (n > mBufferMaxSize)
        n = mBufferMaxSize;

    size_t insertSteps      = (full()) ? n : n - 1;
    size_t endOfInsertIndex = _M_roll_over_forward(mBackOfBuffer, insertSteps);
    LOG("Post-insert back of buffer -> " << endOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(endOfInsertIndex) < _M_wrap_index(mBackOfBuffer))
    {
        // Increment front of buffer 1 past the end of insert index to accomodate new data
        mFrontOfBuffer = _M_roll_over_forward(endOfInsertIndex, 1);
        mIsFullFlag    = false; // Clear full flag since data has been truncated and is not filled back in yet
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
    if (newBackOfBuffer == mFrontOfBuffer) // Re-do this check in case the buffer has been accessed in the meantime
    {
        mBackOfBuffer = endOfInsertIndex;
        mIsFullFlag   = true;
        LOG("Setting full flag")
    }
    else
        mBackOfBuffer = newBackOfBuffer;

    return insertZoneStart;
}

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillPushFront(size_t n, Arg&& value) noexcept
{
    LOG("Inserting " << n << " element[s] front")

    // Guard against inserts larger than the buffer
    if (n > mBufferMaxSize)
        n = mBufferMaxSize;

    // Establish the start and end of the insert zone
    // End should be before the front of buffer unless it is empty
    // Start is n - 1 steps ahead as the end index counts towards 1 element in the insert
    size_t endOfInsertIndex   = (empty()) ? mFrontOfBuffer : _M_roll_over_backward(mFrontOfBuffer, 1);
    size_t startOfInsertIndex = _M_roll_over_backward(endOfInsertIndex, n - 1);
    LOG("Inserting from " << startOfInsertIndex << " -> " << endOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(startOfInsertIndex) <= _M_wrap_index(mBackOfBuffer))
    {
        // Decrement the back of buffer 1 past the start of insert index to accomodate new data
        mBackOfBuffer = _M_roll_over_backward(startOfInsertIndex, 1);
        mIsFullFlag   = false; // Clear full flag since data has been truncated and is not filled back in yet
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

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
void Buffer<T, Allocator>::FillInForward(typename Buffer<T, Allocator>::iterator position, size_t n,
                                         Arg&& value) noexcept
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

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator
    Buffer<T, Allocator>::FillInReverse(typename Buffer<T, Allocator>::iterator position, size_t n,
                                        Arg&& value) noexcept
{
    bool   sentinelReached = false;
    size_t count           = 0;
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

template<typename T, class Allocator>
void Buffer<T, Allocator>::MoveMemoryContents(Buffer<T, Allocator>&& rhs)
{
    // Copy start address and other members from the right hand side to this
    mStartAddress  = rhs.mStartAddress;
    mBufferMaxSize = rhs.mBufferMaxSize;
    mBackOfBuffer  = rhs.mBackOfBuffer;
    mIsFullFlag    = rhs.mIsFullFlag;

    // Clear right hand side, including the start address
    rhs.mStartAddress = nullptr;
    rhs.clear();
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignBufferWithAllocatorPropogation(Buffer<T, Allocator>&& rhs, std::true_type t)
{
    // Deallocate the current buffer, move the allocator from the right hand side to here, and move memory contents
    DeallocateBuffer();
    mAllocator = std::move(rhs.mAllocator);
    MoveMemoryContents(std::forward<Buffer<T>>(rhs));
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignBufferWithAllocatorPropogation(Buffer<T, Allocator>&& rhs, std::false_type f)
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

template<typename T, class Allocator>
void Buffer<T, Allocator>::PrepareForRandomInsert(typename Buffer<T, Allocator>::iterator position, size_t n)
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
        mIsFullFlag     = true;                                     // The resultant buffer will be full
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
        size_t overFlowDiff    = n - openInsertSlots;
        LOG("Moving front and back forward by " << overFlowDiff)
        mFrontOfBuffer = _M_roll_over_forward(mFrontOfBuffer, overFlowDiff);
        mBackOfBuffer  = _M_roll_over_backward(mFrontOfBuffer, 1);
        LOG("Front of buffer -> " << mFrontOfBuffer)
        LOG("Back of buffer -> " << mBackOfBuffer)
    }
    else
    {
        // Else, there is room after the insert zone for more data
        // Copy as much preexisting data behind 'position' as is possible without truncating anything before
        iterator dest = end() - 1;                     // New back of buffer
        iterator source(*this, lastValidElementIndex); // Original back of buffer

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

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator
    Buffer<T, Allocator>::RangeInsert(typename Buffer<T, Allocator>::iterator position, IteratorTypeArg i,
                                      IteratorTypeArg j) noexcept
{
    size_t rangeSize = j - i;
    LOG("Inserting range of size " << rangeSize)

    if (rangeSize >= mBufferMaxSize)
    {
        // Only the first N elements in the range will be inserted, where N is the buffer max size
        // Decrease the range from the back by the difference
        size_t diff = rangeSize - mBufferMaxSize;
        j -= diff;
        rangeSize -= diff;

        // Fill in the entire buffer and set the full flag
        size_t mBackOfBuffer = _M_roll_over_backward(mFrontOfBuffer, 1); // Back of buffer sits behind front when full
        mIsFullFlag          = true;

        LOG("Moving back of buffer -> " << mBackOfBuffer)
        LOG("Setting full flag")
    }
    else
        PrepareForRandomInsert(position, rangeSize);

    // Since IteratorTypeArg is only guaranteed to be at least a `LegacyInputIterator`, we cannot iterate in
    // reverse over the range. We must instead increment through, one at a time, from i to j exclusively.
    LOG("Starting insert")
    iterator curr = position;
    while (i < j)
        *curr++ = *i++;

    return position;
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::RangePushBack(IteratorTypeArg i,
                                                                            IteratorTypeArg j) noexcept
{
    size_t rangeSize = j - i;
    LOG("Inserting range of size " << rangeSize << " back")

    if (rangeSize >= mBufferMaxSize)
    {
        // Only the first N elements in the range will be inserted, where N is the buffer max size
        // Decrease the range from the back by the difference
        size_t diff = rangeSize - mBufferMaxSize;
        j -= diff;
        rangeSize -= diff;
    }

    size_t insertSteps      = (full()) ? rangeSize : rangeSize - 1;
    size_t endOfInsertIndex = _M_roll_over_forward(mBackOfBuffer, insertSteps);
    LOG("Post-insert back of buffer -> " << endOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(endOfInsertIndex) < _M_wrap_index(mBackOfBuffer))
    {
        // Increment front of buffer 1 past the end of insert index to accomodate new data
        mFrontOfBuffer = _M_roll_over_forward(endOfInsertIndex, 1);
        mIsFullFlag    = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving front of buffer -> " << mFrontOfBuffer)
    }

    // Since IteratorTypeArg is only guaranteed to be at least a `LegacyInputIterator`, we cannot iterate in
    // reverse over the range. We must instead increment through, one at a time, from i to j exclusively.
    LOG("Starting insert")
    size_t startOfInsertIndex = (full()) ? _M_roll_over_forward(mBackOfBuffer, 1) : mBackOfBuffer;
    size_t currentInsertIndex = startOfInsertIndex;
    while (i < j)
        CopyAssignElement(currentInsertIndex++, *i++);

    // Now update back of buffer
    size_t newBackOfBuffer = _M_roll_over_forward(endOfInsertIndex, 1);
    if (newBackOfBuffer == mFrontOfBuffer) // Re-do this check in case the buffer has been accessed in the meantime
    {
        mBackOfBuffer = endOfInsertIndex;
        mIsFullFlag   = true;
        LOG("Setting full flag")
    }
    else
        mBackOfBuffer = newBackOfBuffer;

    return iterator(*this, startOfInsertIndex);
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::RangePushFront(IteratorTypeArg i,
                                                                             IteratorTypeArg j) noexcept
{
    size_t rangeSize = j - i;
    LOG("Inserting range of size " << rangeSize << " back")

    if (rangeSize >= mBufferMaxSize)
    {
        // Only the first N elements in the range will be inserted, where N is the buffer max size
        // Decrease the range from the back by the difference
        size_t diff = rangeSize - mBufferMaxSize;
        j -= diff;
        rangeSize -= diff;
    }

    size_t endOfInsertIndex   = (empty()) ? mFrontOfBuffer : _M_roll_over_backward(mFrontOfBuffer, 1);
    size_t startOfInsertIndex = _M_roll_over_backward(endOfInsertIndex, rangeSize - 1);
    LOG("Post-insert front of buffer -> " << startOfInsertIndex)

    // Check for overflow
    if (_M_wrap_index(startOfInsertIndex) <= _M_wrap_index(mBackOfBuffer))
    {
        // Decrement the back of buffer 1 past the start of insert index to accomodate new data
        mBackOfBuffer = _M_roll_over_backward(startOfInsertIndex, 1);
        mIsFullFlag   = false; // Clear full flag since data has been truncated and is not filled back in yet
        LOG("Overflow detected")
        LOG("Moving back of buffer -> " << mBackOfBuffer)
    }

    // Since IteratorTypeArg is only guaranteed to be at least a `LegacyInputIterator`, we cannot iterate in
    // reverse over the range. We must instead increment through, one at a time, from i to j exclusively.
    LOG("Starting insert")
    size_t currentInsertIndex = startOfInsertIndex;
    while (i < j)
        CopyAssignElement(currentInsertIndex++, *i++);

    // Now update front of buffer
    mFrontOfBuffer = startOfInsertIndex;
    if (_M_roll_over_backward(mFrontOfBuffer, 1) == mBackOfBuffer)
    {
        LOG("Setting full flag")
        mIsFullFlag = true;
    }

    return iterator(*this, startOfInsertIndex);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// End Buffer method definitions    ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Global method definitions in alphabetical order    /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ValueTypeArg, typename AllocatorTypeArg = std::allocator<ValueTypeArg>>
bool operator==(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs,
                const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept
{
    using iterator       = typename Buffer<ValueTypeArg, AllocatorTypeArg>::iterator;
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

template<typename ValueTypeArg, typename AllocatorTypeArg = std::allocator<ValueTypeArg>>
bool operator!=(const Buffer<ValueTypeArg, AllocatorTypeArg>& rhs,
                const Buffer<ValueTypeArg, AllocatorTypeArg>& lhs) noexcept
{
    return !(rhs == lhs);
}

} // End namespace shmit

namespace std
{
template<typename T, class Allocator>
void swap(shmit::Buffer<T, Allocator>& rhs, shmit::Buffer<T, Allocator>& lhs) noexcept
{
    rhs.swap(lhs);
}

} // End namespace std
