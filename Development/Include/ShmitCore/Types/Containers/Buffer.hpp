#pragma once

#include "detail/Buffer.hpp"

#include <ShmitCore/Logging/Diagnostics/Logging.hpp>
#include <ShmitCore/Logging/Events/Logging.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <iterator>
#include <type_traits>

namespace shmit
{

template<typename T, class Allocator = std::allocator<T>>
class Buffer : public detail::Buffer
{
    using alloc_traits = __gnu_cxx::__alloc_traits<Allocator>;

public:
    using value_type      = typename alloc_traits::value_type;
    using pointer         = typename alloc_traits::pointer;
    using const_pointer   = typename alloc_traits::const_pointer;
    using reference       = typename alloc_traits::reference;
    using const_reference = typename alloc_traits::const_reference;

    using allocator_type = Allocator;

    using iterator               = detail::BufferIteratorT<T>;
    using const_iterator         = detail::ConstBufferIteratorT<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type       = typename detail::Buffer::size_type;
    using difference_type = typename detail::Buffer::difference_type;

    /// @brief Default constructor. Creates an unallocated Buffer.
    Buffer() noexcept;

    /// @brief Chained constructor. Creates an allocated, empty Buffer.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param buffer_size Maximum size of the buffer
    Buffer(size_type buffer_size) noexcept;

    /// @brief Base constructor. Creates an allocated, empty Buffer and assigns the Allocator instance.
    /// @param buffer_size
    /// @param allocator
    Buffer(size_type buffer_size, Allocator const& allocator) noexcept;

    /// @brief Chained constructor. Fills a Buffer with one value.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param buffer_size Maximum size of the buffer
    /// @param init Initialized value of all elements
    Buffer(size_type buffer_size, T const& init) noexcept;

    /// @brief Chained constructor. Fills a Buffer with one value and assigns the Allocator instance.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param buffer_size Maximum size of the buffer
    /// @param init Initialized value of all elements
    /// @param allocator Allocator initializer
    Buffer(size_type buffer_size, T const& init, Allocator const& allocator) noexcept;

    /// @brief Chained constructor. Creates an unallocated Buffer that is named.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param name Name of this instance
    /// @param buffer_size Maximum size of the buffer
    Buffer(char const* name) noexcept;

    /// @brief Chained constructor. Fills a named Buffer with one value.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param name Name of this instance
    /// @param buffer_size Maximum size of the buffer
    /// @param init Initialized value of all elements
    Buffer(char const* name, size_type buffer_size) noexcept;

    /// @brief Chained constructor. Fills a named Buffer with one value and assigns the Allocator instance.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param name Name of this instance
    /// @param buffer_size Maximum size of the buffer
    /// @param init Initialized value of all elements
    /// @param allocator Allocator initializer
    Buffer(char const* name, size_type buffer_size, T const& init) noexcept;

    /// @brief Chained constructor. Creates a Buffer with max size and contents of a provided initializer list.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param il Initializer list
    Buffer(char const* name, size_type buffer_size, T const& init, Allocator const& allocator) noexcept;

    /// @brief Chained constructor. Creates a Buffer with max size and contents of a provided initializer list and
    /// assigns the Allocator instance.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param il Initializer list
    /// @param allocator Allocator initializer
    Buffer(std::initializer_list<T> il) noexcept;

    /// @brief Chained constructor. Creates a named Buffer with max size and contents of a provided initializer list.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param il Initializer list
    Buffer(std::initializer_list<T> il, Allocator const& allocator) noexcept;

    /// @brief Chained constructor. Creates a named Buffer with max size and contents of a provided initializer list and
    /// assigns the Allocator instance.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param il Initializer list
    Buffer(char const* name, std::initializer_list<T> il) noexcept;

    /// @brief Chained constructor. Copy-creates a Buffer.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param rhs Copy subject
    Buffer(char const* name, std::initializer_list<T> il, Allocator const& allocator) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    Buffer(IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    Buffer(char const* name, IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    Buffer(char const* name, IteratorTypeArg i, IteratorTypeArg j, Allocator const& allocator) noexcept;

    /// @brief Chained constructor. Moves a Buffer's contents to this instance.
    /// @tparam T Contained type
    /// @tparam Allocator Allocator type
    /// @param rhs Move subject. Made invalid by this operation.
    Buffer(Buffer const& rhs) noexcept;
    Buffer(Buffer&& rhs) noexcept;

    ~Buffer() noexcept;

    void resize(size_t n) noexcept;

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

    T&       at(size_t index) noexcept;
    T const& at(size_t index) const noexcept;

    T&       front() noexcept;
    T const& front() const noexcept;

    T&       back() noexcept;
    T const& back() const noexcept;

    void assign(size_t count, T const& value) noexcept;
    void assign(std::initializer_list<T> il) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    void assign(IteratorTypeArg i, IteratorTypeArg j) noexcept;

    template<typename... Args>
    iterator emplace(iterator position, Args&&... args) noexcept;

    template<typename... Args>
    void emplace_front(Args&&... args) noexcept;

    template<typename... Args>
    void emplace_back(Args&&... args) noexcept;

    iterator insert(iterator position, T const& value) noexcept;
    iterator insert(iterator position, T&& value) noexcept;
    iterator insert(iterator position, size_t n, T const& value) noexcept;
    iterator insert(iterator position, std::initializer_list<T> il) noexcept;

    template<typename IteratorTypeArg, typename = std::_RequireInputIter<IteratorTypeArg>>
    iterator insert(iterator position, IteratorTypeArg i, IteratorTypeArg j) noexcept;

    void pop_front() noexcept;
    void push_front(T const& value) noexcept;
    void push_front(T&& value) noexcept;

    void pop_back() noexcept;
    void push_back(T const& value) noexcept;
    void push_back(T&& value) noexcept;

    void swap(Buffer& rhs) noexcept;

    iterator erase(iterator position) noexcept;
    iterator erase(iterator position, iterator i, iterator j) noexcept;

    Buffer& operator=(Buffer const& rhs) noexcept;
    Buffer& operator=(Buffer&& rhs) noexcept;
    Buffer& operator=(std::initializer_list<T> const& il) noexcept;

    bool operator!=(const Buffer& lhs) noexcept;

    T&       operator[](size_t index) noexcept;
    T const& operator[](size_t index) const noexcept;

    template<typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator==(Buffer<ValueTypeArg, AllocatorTypeArg> const& rhs,
                           Buffer<ValueTypeArg, AllocatorTypeArg> const& lhs) noexcept;

    template<typename ValueTypeArg, typename AllocatorTypeArg>
    friend bool operator!=(Buffer<ValueTypeArg, AllocatorTypeArg> const& rhs,
                           Buffer<ValueTypeArg, AllocatorTypeArg> const& lhs) noexcept;

private:
    T*   AllocateBuffer(size_t n) noexcept;
    void DeallocateBuffer() noexcept;

    void CopyBuffer(Buffer const& rhs) noexcept;
    void CopyAssignBufferWithAllocatorPropogation(const Buffer& rhs, std::true_type t) noexcept;
    void CopyAssignBufferWithAllocatorPropogation(const Buffer& rhs, std::false_type f) noexcept;

    void MoveAssignBufferWithAllocatorPropogation(Buffer&& rhs, std::true_type t) noexcept;
    void MoveAssignBufferWithAllocatorPropogation(Buffer&& rhs, std::false_type f) noexcept;

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
    detail::Buffer::BufferIterator FillInForward(iterator position, size_t n, Arg&& value) noexcept;

    template<typename IteratorTypeArg>
    detail::Buffer::BufferIterator FillRangeForward(iterator position, IteratorTypeArg i, IteratorTypeArg j) noexcept;

    Allocator m_allocator;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer Constructor definitions      ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer() noexcept : detail::Buffer(sizeof(T))
{
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer<T, Allocator>::size_type buffer_size) noexcept : Buffer()
{
    // Only continue if the given buffer size is > 0
    if (buffer_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(buffer_size);
        if (m_start_address)
            m_max_element_count = buffer_size;
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(size_type buffer_size, Allocator const& allocator) noexcept
    : detail::Buffer(sizeof(T)), m_allocator {allocator}
{
    // Only continue if the given buffer size is > 0
    if (buffer_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(buffer_size);
        if (m_start_address)
            m_max_element_count = buffer_size;
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer<T, Allocator>::size_type buffer_size, T const& init) noexcept : Buffer(buffer_size)
{
    if (m_start_address)
    {
        // Set back of buffer to the end of the available space and raise full flag
        m_back    = IncrementBoundless(m_front, m_max_element_count - 1);
        m_is_full = true;

        // Construct elements in the buffer
        // Need to typecast 'm_start_address' from void* before performing pointer arithmetic
        for (T& e : *this)
            alloc_traits::construct(m_allocator, (T*)&e, init);
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer<T, Allocator>::size_type buffer_size, T const& init,
                             Allocator const& allocator) noexcept
    : detail::Buffer(sizeof(T)), m_allocator {allocator}
{
    // Only continue if the given buffer size is > 0
    if (buffer_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(buffer_size);
        if (m_start_address)
            m_max_element_count = buffer_size;
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name) noexcept : detail::Buffer(name, sizeof(T))
{
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, Buffer<T, Allocator>::size_type buffer_size) noexcept
    : detail::Buffer(name, sizeof(T))
{
    // Only continue if the given buffer size is > 0
    if (buffer_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(buffer_size);
        if (m_start_address)
            m_max_element_count = buffer_size;
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, Buffer<T, Allocator>::size_type buffer_size, T const& init) noexcept
    : Buffer(name, buffer_size)
{
    if (m_start_address)
    {
        // Set back of buffer to the end of the available space and raise full flag
        m_back    = IncrementBoundless(m_front, m_max_element_count - 1);
        m_is_full = true;

        // Construct elements in the buffer
        // Need to typecast 'm_start_address' from void* before performing pointer arithmetic
        for (T& e : *this)
            alloc_traits::construct(m_allocator, (T*)&e, init);
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, Buffer<T, Allocator>::size_type buffer_size, T const& init,
                             Allocator const& allocator) noexcept
    : detail::Buffer(name, sizeof(T)), m_allocator {allocator}
{
    // Only continue if the given buffer size is > 0
    if (buffer_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(buffer_size);
        if (m_start_address)
        {
            m_max_element_count = buffer_size;

            // Set back of buffer to the end of the available space and raise full flag
            m_back    = IncrementBoundless(m_front, m_max_element_count - 1);
            m_is_full = true;

            // Construct elements in the buffer
            // Need to typecast 'm_start_address' from void* before performing pointer arithmetic
            for (T& e : *this)
                alloc_traits::construct(m_allocator, (T*)&e, init);
        }
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(std::initializer_list<T> il) noexcept : Buffer(il.size())
{
    if (m_start_address)
        assign(il);
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(std::initializer_list<T> il, Allocator const& allocator) noexcept
    : detail::Buffer(sizeof(T)), m_allocator {allocator}
{
    // Only continue if the list size is > 0
    size_type list_size = il.size();
    if (list_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(list_size);
        if (m_start_address)
        {
            m_max_element_count = list_size;
            assign(il);
        }
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, std::initializer_list<T> il) noexcept : Buffer(name, il.size())
{
    if (m_start_address)
        assign(il);
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(char const* name, std::initializer_list<T> il, Allocator const& allocator) noexcept
    : detail::Buffer(name, sizeof(T)), m_allocator {allocator}
{
    // Only continue if the list size is > 0
    size_type list_size = il.size();
    if (list_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(list_size);
        if (m_start_address)
        {
            m_max_element_count = list_size;
            assign(il);
        }
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
Buffer<T, Allocator>::Buffer(IteratorTypeArg i, IteratorTypeArg j) noexcept : Buffer(j - i)
{
    if (m_start_address)
        assign(i, j);
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
Buffer<T, Allocator>::Buffer(char const* name, IteratorTypeArg i, IteratorTypeArg j) noexcept : Buffer(name, (j - i))
{
    if (m_start_address)
        assign(i, j);
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
Buffer<T, Allocator>::Buffer(char const* name, IteratorTypeArg i, IteratorTypeArg j,
                             Allocator const& allocator) noexcept
    : detail::Buffer(name, sizeof(T)), m_allocator {allocator}
{
    // Only continue if the range size is > 0
    size_type range_size = j - i;
    if (range_size > 0)
    {
        // Allocate storage for the container
        m_start_address = AllocateBuffer(range_size);
        if (m_start_address)
        {
            m_max_element_count = range_size;
            assign(i, j);
        }
        // Failure to allocate here will post a diagnostic log posit
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer<T, Allocator> const& rhs) noexcept
    : detail::Buffer(rhs.GetName(), sizeof(T)),
      m_allocator(alloc_traits::select_on_container_copy_construction(rhs.m_allocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
    CopyBuffer(rhs);
}

template<typename T, class Allocator>
Buffer<T, Allocator>::Buffer(Buffer<T, Allocator>&& rhs) noexcept
    : detail::Buffer(rhs.GetName(), sizeof(T)), m_allocator(std::move(rhs.m_allocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
    MoveMemoryContents(std::forward<detail::Buffer&&>(rhs));
}

template<typename T, class Allocator>
Buffer<T, Allocator>::~Buffer() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(destructing)
    DeallocateBuffer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer method definitions in alphabetical order     ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename T, class Allocator>
void Buffer<T, Allocator>::assign(size_t count, T const& value) noexcept
{
    // Guard against overfill, then fill in the Buffer
    count                                         = OverfillGuard(count);
    detail::Buffer::BufferIterator assignment_end = FillInForward(m_front, count, value);

    if (assignment_end == m_front)
    {
        m_back = DecrementBoundless(assignment_end, 1);
        AtCapacity();
    }
    else if (assignment_end > m_back)
        m_back = assignment_end;
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::assign(std::initializer_list<T> il) noexcept
{
    assign(il.begin(), il.end());
}

template<typename T, class Allocator>
template<typename IteratorTypeArg, typename>
void Buffer<T, Allocator>::assign(IteratorTypeArg i, IteratorTypeArg j) noexcept
{
    // Guard against overfills
    size_t range_size          = j - i;
    size_t corrected_size      = OverfillGuard(range_size);
    size_t list_reduction_bias = range_size - corrected_size;
    j -= list_reduction_bias;

    // Fill in range, starting at front
    detail::Buffer::BufferIterator assignment_end = FillRangeForward(m_front, i, j);

    if (assignment_end == m_front)
    {
        m_back = DecrementBoundless(assignment_end, 1);
        AtCapacity();
    }
    else if (assignment_end > m_back)
        m_back = assignment_end;
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::at(size_t index) noexcept
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= m_max_element_count))
    {
        // TODO Throw exception or enter hard fault trap
    }

    iterator it = begin();
    std::advance(it, index);
    return *it;
}

template<typename T, class Allocator>
T const& Buffer<T, Allocator>::at(size_t index) const noexcept
{
    // Check for empty or out-of-bounds conditions
    if (empty() || (index >= m_max_element_count))
    {
        // TODO Post diagnostic posit
    }

    const_iterator it = cbegin();
    std::advance(it, index);
    return *it;
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::back() noexcept
{
    iterator back = end() - 1;
    return *back;
}

template<typename T, class Allocator>
T const& Buffer<T, Allocator>::back() const noexcept
{
    const_iterator back = cend() - 1;
    return *back;
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::begin() noexcept
{
    return iterator(m_front);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::begin() const noexcept
{
    return const_iterator(m_front);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cbegin() const noexcept
{
    return const_iterator(m_front);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::cend() const noexcept
{
    return const_iterator(m_back + 1);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crbegin() const noexcept
{
    return const_reverse_iterator(m_front);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_reverse_iterator Buffer<T, Allocator>::crend() const noexcept
{
    return const_reverse_iterator(m_back + 1);
}

template<typename T, class Allocator>
template<typename... Args>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::emplace(typename Buffer<T, Allocator>::iterator position,
                                                                      Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_random)
    PrepareForRandomPlacement(position, 1);
    alloc_traits::construct(m_allocator, &(*position), std::forward<Args>(args)...);

    return position;
}

template<typename T, class Allocator>
template<typename... Args>
void Buffer<T, Allocator>::emplace_back(Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_back)

    // Emplace position is the back of the Buffer, unless the buffer is full
    iterator emplace_position = m_back;
    if (m_is_full)
        emplace_position = IncrementBoundless(emplace_position, 1);

    // Emplace element
    alloc_traits::construct(m_allocator, &(*emplace_position), std::forward<Args>(args)...);

    // Detect overflow and capacity
    bool is_full = false;
    if (emplace_position == m_front)
    {
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("%s", "Count:1"))
        m_front = IncrementBoundless(m_front, 1);
        AtCapacity();
    }
    else if (IncrementBoundless(emplace_position, 1) == m_front)
        AtCapacity();

    // Update back of Buffer
    m_back = emplace_position;
}

template<typename T, class Allocator>
template<typename... Args>
void Buffer<T, Allocator>::emplace_front(Args&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_front)

    // Emplace position is 1 before the front of the Buffer, always
    iterator emplace_position = DecrementBoundless(m_front, 1);

    // Emplace element
    alloc_traits::construct(m_allocator, &(*emplace_position), std::forward<Args>(args)...);

    // Detect overflow and capacity
    bool is_full = false;
    if (emplace_position == m_back)
    {
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(front_overflow, shmit::log::diagnostics::Print("%s", "Count:1"))
        m_back = DecrementBoundless(m_back, 1);
        AtCapacity();
    }
    else if ((emplace_position - 1) == m_back)
        AtCapacity();

    // Update front of Buffer
    m_front = emplace_position;
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::end() noexcept
{
    return iterator(m_back + 1);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::const_iterator Buffer<T, Allocator>::end() const noexcept
{
    return const_iterator(m_back + 1);
}

template<typename T, class Allocator>
T& Buffer<T, Allocator>::front() noexcept
{
    iterator front = begin();
    return *front;
}

template<typename T, class Allocator>
T const& Buffer<T, Allocator>::front() const noexcept
{
    const_iterator front = cbegin();
    return *front;
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     T const& value) noexcept
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
        return FillPushFront(1, std::forward<T>(value));
    else if (position == end())
        return FillPushBack(1, std::forward<T>(value));

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, std::forward<T>(value));
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     size_t n, T const& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
        return FillPushFront(n, copyOfValue);
    else if (position == end())
        return FillPushBack(n, copyOfValue);

    // Otherwise call the random-access insert routine
    return FillInsert(position, n, copyOfValue);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::insert(typename Buffer<T, Allocator>::iterator position,
                                                                     std::initializer_list<T> il) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == begin())
        return RangePushFront(il.begin(), il.end());
    else if (position == end())
        return RangePushBack(il.begin(), il.end());

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
        return RangePushFront(i, j);
    else if (position == end())
        return RangePushBack(i, j);

    // Otherwise call the random-access insert routine
    return RangeInsert(position, i, j);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::pop_back() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(pop_back)

    // Check for empty condition
    if (!empty())
    {
        // Move back of buffer backwards 1 step unless full and clear full flag
        if (!full())
            m_back = DecrementBoundless(m_back, 1);

        m_is_full = false;
    }
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::pop_front() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(pop_front)

    // Check for empty condition
    if (!empty())
    {
        // Move front of buffer fowards 1 step and clear full flag
        m_front   = IncrementBoundless(m_front, 1);
        m_is_full = false;
    }
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_back(T const& value) noexcept
{
    FillPushBack(1, value);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_back(T&& value) noexcept
{
    FillPushBack(1, std::forward<T>(value));
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T const& value) noexcept
{
    FillPushFront(1, value);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::push_front(T&& value) noexcept
{
    FillPushFront(1, std::forward<T>(value));
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rbegin() noexcept
{
    return reverse_iterator(m_front);
}

template<typename T, class Allocator>
typename Buffer<T, Allocator>::reverse_iterator Buffer<T, Allocator>::rend() noexcept
{
    return reverse_iterator(m_back + 1);
}

// TODO refactor
template<typename T, class Allocator>
void Buffer<T, Allocator>::resize(size_t n) noexcept
{
    T* new_buffer = AllocateBuffer(n);
    if (new_buffer)
    {
        // Clear buffer
        clear();

        // Swap old Buffer start address for new
        T* old              = (T*)m_start_address;
        m_start_address     = new_buffer;
        m_max_element_count = n;

        // Deallocate old Buffer contents
        alloc_traits::deallocate(m_allocator, (T*)old, m_max_element_count);
    }
    // Failure to allocate here will post a diagnostic log posit
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::swap(Buffer& rhs) noexcept
{
    detail::Buffer::Swap(rhs);

    // If Allocator requires that it be propogated on container swap, do so now
    if (alloc_traits::propagate_on_container_swap::value == true)
    {
        Allocator tempAllocator = m_allocator;
        m_allocator             = rhs.m_allocator;
        rhs.m_allocator         = tempAllocator;
    }
}

template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(Buffer<T, Allocator> const& rhs) noexcept
{
    // Tag-dispatch the appropriate routine, selected at compile time
    CopyAssignBufferWithAllocatorPropogation(rhs, typename alloc_traits::propagate_on_container_copy_assignment {});

    return *this;
}

template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(Buffer<T, Allocator>&& rhs) noexcept
{
    // Tag-dispatch the appropriate routine, selected at compile time
    MoveAssignBufferWithAllocatorPropogation(std::forward<Buffer<T, Allocator>>(rhs),
                                             typename alloc_traits::propagate_on_container_move_assignment {});

    return *this;
}

// TODO refactor
template<typename T, class Allocator>
Buffer<T, Allocator>& Buffer<T, Allocator>::operator=(std::initializer_list<T> const& il) noexcept
{
    size_t il_size = il.size();
    if (m_max_element_count != il_size)
        resize(il_size);
    else
        clear();

    assign(il);

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
T const& Buffer<T, Allocator>::operator[](size_t index) const noexcept
{
    const_iterator it = cbegin();
    std::advance(it, index);
    return *it;
}

//  Private ============================================================================================================

template<typename T, class Allocator>
T* Buffer<T, Allocator>::AllocateBuffer(size_t n) noexcept
{
    T* bufferPtr = alloc_traits::allocate(m_allocator, n);
    if (bufferPtr == nullptr)
        LOG_MEMBER_DIAGNOSTIC_POSIT(bad_alloc)

    return bufferPtr;
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignBufferWithAllocatorPropogation(Buffer<T, Allocator> const& rhs,
                                                                    std::true_type              t) noexcept
{
    // Deallocate current buffer, the lhs allocator may not be able to later
    DeallocateBuffer();

    // Assign the right-hand-side's allocator to this instance, then copy-assign buffer
    m_allocator = rhs.m_allocator;
    CopyBuffer(rhs);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyAssignBufferWithAllocatorPropogation(Buffer<T, Allocator> const& rhs,
                                                                    std::false_type             f) noexcept
{
    if (m_max_element_count != rhs.m_max_element_count)
        resize(rhs.m_max_element_count);
    else
        clear(); // Just clear the current buffer

    // Copy Buffer contents
    for (T const& rhsValue : rhs)
        push_back(rhsValue);
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::CopyBuffer(Buffer const& rhs) noexcept
{
    if (rhs.m_start_address && rhs.m_max_element_count)
    {
        // Allocate new buffer with right-hand-side size, then copy contents across
        m_start_address = AllocateBuffer(rhs.m_max_element_count);
        if (m_start_address)
        {
            m_max_element_count = rhs.m_max_element_count;

            // Copy Buffer contents
            for (T const& rhsValue : rhs)
                push_back(rhsValue);
        }
    }
    // Failure to allocate here will post a diagnostic log posit
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::DeallocateBuffer() noexcept
{
    // Only continue if the start address is valid
    if (m_start_address)
    {
        // Destroy only elements that are initialized and stored in the buffer
        if (!empty())
        {
            for (T& e : *this)
                alloc_traits::destroy(m_allocator, (T*)(&e));

            clear(); // Reset in case Buffer is still in use
        }

        // Deallocate buffer
        // Need to typecast 'm_start_address' from void*
        alloc_traits::deallocate(m_allocator, (T*)m_start_address, m_max_element_count);
    }
}

template<typename T, class Allocator>
template<typename Arg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillInsert(Buffer<T, Allocator>::iterator position,
                                                                         size_t n, Arg&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_random, shmit::log::diagnostics::Print("Count:%d", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Prepare for placement at random position, this adjusts the front and back of the Buffer internally
    PrepareForRandomPlacement(position, n);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillInForward(position, n, std::forward<Arg>(value));

    return position;
}

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillPushBack(size_t n, Arg&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_back, shmit::log::diagnostics::Print("Count:%d", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Insert start position is the back of the Buffer, unless the buffer is full
    detail::Buffer::BufferIterator insert_start = m_back;
    if (m_is_full)
        insert_start = IncrementBoundless(insert_start, 1);

    // Perform insert
    detail::Buffer::BufferIterator insert_end = FillInForward(insert_start, n, std::forward<Arg>(value));
    PostInsertBack(insert_end);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::FillPushFront(size_t n, Arg&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_front, shmit::log::diagnostics::Print("Count:%d", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Insert start is n places before the front of the Buffer
    detail::Buffer::BufferIterator insert_start = DecrementBoundless(m_front, n);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillInForward(insert_start, n, std::forward<Arg>(value));
    PostInsertFront(insert_start);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class Allocator>
template<typename Arg> // T& or T&&
detail::Buffer::BufferIterator Buffer<T, Allocator>::FillInForward(typename Buffer<T, Allocator>::iterator position,
                                                                   size_t n, Arg&& value) noexcept
{
    for (size_t count = 0; count < n; count++)
    {
        *position = value;
        position  = IncrementBoundless(position, 1);
    }

    // Upcast to base class on return
    return position;
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
detail::Buffer::BufferIterator Buffer<T, Allocator>::FillRangeForward(typename Buffer<T, Allocator>::iterator position,
                                                                      IteratorTypeArg i, IteratorTypeArg j) noexcept
{
    size_type range_size = j - i;
    for (size_t count = 0; count < range_size; count++)
    {
        *position = *i++;
        position  = IncrementBoundless(position, 1);
    }

    // Upcast to base class on return
    return position;
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignBufferWithAllocatorPropogation(Buffer<T, Allocator>&& rhs,
                                                                    std::true_type         t) noexcept
{
    // Deallocate the current buffer, move the allocator from the right hand side to here, and move memory contents
    DeallocateBuffer();
    m_allocator = std::move(rhs.m_allocator);
    MoveMemoryContents(std::forward<detail::Buffer>(rhs));
}

template<typename T, class Allocator>
void Buffer<T, Allocator>::MoveAssignBufferWithAllocatorPropogation(Buffer<T, Allocator>&& rhs,
                                                                    std::false_type        f) noexcept
{
    if (m_allocator == rhs.m_allocator)
    {
        // Deallocate the current buffer and move memory contents
        DeallocateBuffer();
        MoveMemoryContents(std::forward<detail::Buffer>(rhs));
    }
    else
        LOG_MEMBER_DIAGNOSTIC_POSIT(bad_move)
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::RangeInsert(
    typename Buffer<T, Allocator>::iterator position, IteratorTypeArg i, IteratorTypeArg j) noexcept
{
    // Guard against overfill
    size_t range_size          = j - i;
    size_t corrected_size      = OverfillGuard(range_size);
    size_t list_reduction_bias = range_size - corrected_size;
    j -= list_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_random, shmit::log::diagnostics::Print("Count:%d", corrected_size))

    // Prepare for placement at random position, this adjusts the front and back of the Buffer internally
    PrepareForRandomPlacement(position, corrected_size);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillRangeForward(position, i, j);

    return position;
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::RangePushBack(IteratorTypeArg i,
                                                                            IteratorTypeArg j) noexcept
{
    // Guard against overfill
    size_t range_size           = j - i;
    size_t corrected_size       = OverfillGuard(range_size);
    size_t range_reduction_bias = range_size - corrected_size;
    j -= range_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_back, shmit::log::diagnostics::Print("Count:%d", corrected_size))

    // Insert start position is the back of the Buffer, unless the buffer is full
    detail::Buffer::BufferIterator insert_start = m_back;
    if (m_is_full)
        insert_start = IncrementBoundless(insert_start, 1);

    // Perform insert
    detail::Buffer::BufferIterator insert_end = FillRangeForward(insert_start, i, j);
    PostInsertBack(insert_end);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class Allocator>
template<typename IteratorTypeArg>
typename Buffer<T, Allocator>::iterator Buffer<T, Allocator>::RangePushFront(IteratorTypeArg i,
                                                                             IteratorTypeArg j) noexcept
{
    // Guard against overfill
    size_t range_size           = j - i;
    size_t corrected_size       = OverfillGuard(range_size);
    size_t range_reduction_bias = range_size - corrected_size;
    j -= range_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_front, shmit::log::diagnostics::Print("Count:%d", corrected_size))

    // Insert start is n places before the front of the Buffer
    detail::Buffer::BufferIterator insert_start = DecrementBoundless(m_front, corrected_size);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillRangeForward(insert_start, i, j);
    PostInsertBack(insert_start);

    // Construct typed iterator and return
    return iterator(insert_start);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Global method definitions in alphabetical order    /////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ValueTypeArg, typename AllocatorTypeArg = std::allocator<ValueTypeArg>>
bool operator==(Buffer<ValueTypeArg, AllocatorTypeArg> const& rhs,
                Buffer<ValueTypeArg, AllocatorTypeArg> const& lhs) noexcept
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
bool operator!=(Buffer<ValueTypeArg, AllocatorTypeArg> const& rhs,
                Buffer<ValueTypeArg, AllocatorTypeArg> const& lhs) noexcept
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
