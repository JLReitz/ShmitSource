#pragma once

#include "Detail/Buffer.hpp"

#include <ShmitCore/Help/Integer.hpp>
#include <ShmitCore/Logging/Diagnostics/Logging.hpp>
#include <ShmitCore/Logging/Events/Logging.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <algorithm>
#include <iterator>
#include <type_traits>

namespace shmit
{

template<typename T, class AllocatorType = std::allocator<T>>
class Buffer : public detail::Buffer
{
    using alloc_traits = __gnu_cxx::__alloc_traits<AllocatorType>;

public:
    using value_type      = typename alloc_traits::value_type;
    using pointer         = typename alloc_traits::pointer;
    using const_pointer   = typename alloc_traits::const_pointer;
    using reference       = typename alloc_traits::reference;
    using const_reference = typename alloc_traits::const_reference;

    using allocator_type = AllocatorType;

    using iterator               = detail::BufferIteratorT<T>;
    using const_iterator         = detail::ConstBufferIteratorT<T>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using size_type       = typename detail::Buffer::size_type;
    using difference_type = typename detail::Buffer::difference_type;

    /**!
     * @brief Constructs an unallocated Buffer.
     */
    Buffer() noexcept;

    /**
     * @brief Constructs an unallocated Buffer and assigns the Allocator instance
     *
     * @param[in][in] allocator
     */
    Buffer(AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs an allocated, empty Buffer
     *
     * @param[in] buffer_size Maximum size of the buffer
     */
    Buffer(size_type buffer_size) noexcept;

    /**!
     * @brief Constructs an allocated, empty Buffer and assigns the Allocator instance
     *
     * @param[in] buffer_size Maximum size of the buffer
     * @param[in] allocator Allocator initializer
     */
    Buffer(size_type buffer_size, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a Buffer and fills it with one value.
     *
     * @param[in] buffer_size Maximum size of the buffer
     * @param[in] init Initialized value of all elements
     */
    Buffer(size_type buffer_size, T const& init) noexcept;

    /**!
     * @brief Constructs a Buffer, fills it with one value, and assigns the Allocator instance
     *
     * @param[in] buffer_size Maximum size of the buffer
     * @param[in] init Initialized value of all elements
     * @param[in] allocator Allocator initializer
     */
    Buffer(size_type buffer_size, T const& init, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a named, unallocated Buffer
     *
     * @param[in] name Name of the Buffer instance
     */
    Buffer(char const* name) noexcept;

    /**!
     * @brief Constructs a named, unallocated Buffer and assigns the Allocator instance
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] allocator Allocator initializer
     */
    Buffer(char const* name, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a named, allocated, empty Buffer
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] buffer_size Maximum size of the buffer
     */
    Buffer(char const* name, size_type buffer_size) noexcept;

    /**!
     * @brief Constructs a named Buffer and fills it with one value.
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] buffer_size Maximum size of the buffer
     * @param[in] init Initialized value of all elements
     */
    Buffer(char const* name, size_type buffer_size, T const& init) noexcept;

    /**!
     * @brief Construct a new Buffer object
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] buffer_size Maximum size of the buffer
     * @param[in] init Initialized value of all elements
     * @param[in] allocator Allocator initializer
     */
    Buffer(char const* name, size_type buffer_size, T const& init, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a Buffer with max size and contents of a provided initializer list
     *
     * @param[in] il Initializer list
     */
    Buffer(std::initializer_list<T> il) noexcept;

    /**!
     * @brief Constructs a Buffer with max size and contents of a provided initializer list and assigns the
     * Allocator instance
     *
     * @param[in] il Initializer list
     * @param[in] allocator Allocator initializer
     */
    Buffer(std::initializer_list<T> il, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a named Buffer with max size and contents of a provided initializer list
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] il Initializer list
     */
    Buffer(char const* name, std::initializer_list<T> il) noexcept;

    /**!
     * @brief Constructs a named Buffer with max size and contents of a provided initializer list and assigns the
     * Allocator instance
     *
     * @param[in] name Name of the Buffer instance
     * @param[in] il Initializer list
     * @param[in] allocator Allocator initializer
     */
    Buffer(char const* name, std::initializer_list<T> il, AllocatorType const& allocator) noexcept;

    /**!
     * @brief Constructs a Buffer with max size and contents of a provided range
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param[in] i Range start
     * @param[in] j Range end (not included in contents). Must compare >= i.
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    Buffer(IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Constructs a named Buffer with max size and contents of a provided range
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param[in] name Name of the Buffer instance
     * @param[in] i Range start
     * @param[in] j Range end (not included in contents). Must compare >= i.
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    Buffer(char const* name, IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Constructs a named Buffer with max size and contents of a provided range and assigns the Allocator
     * instance
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param[in] name Name of the Buffer instance
     * @param[in] i Range start
     * @param[in] j Range end (not included in contents). Must compare >= i.
     * @param[in] allocator Allocator initializer
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    Buffer(char const* name, IteratorType i, IteratorType j, AllocatorType const& allocator) noexcept;

    Buffer(Buffer const& rhs) noexcept;
    Buffer(Buffer&& rhs) noexcept;

    ~Buffer() noexcept;

    /**!
     * @brief Returns an iterator to the beginning of the Buffer
     *
     * @return iterator Front of buffer
     */
    iterator begin() noexcept;

    /**!
     * @brief Returns a const_iterator to the beginning of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_iterator begin() const noexcept;

    /**!
     * @brief Returns a reverse_iterator to the beginning of the Buffer
     *
     * @return iterator Front of buffer
     */
    reverse_iterator rbegin() noexcept;

    /**!
     * @brief Returns a const_iterator to the beginning of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_iterator cbegin() const noexcept;

    /**!
     * @brief Returns a const_reverse_iterator to the beginning of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_reverse_iterator crbegin() const noexcept;

    /**!
     * @brief Returns an iterator to the end of the Buffer
     *
     * @return iterator Front of buffer
     */
    iterator end() noexcept;

    /**!
     * @brief Returns a const_iterator to the end of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_iterator end() const noexcept;

    /**!
     * @brief Returns a reverse_iterator to the end of the Buffer
     *
     * @return iterator Front of buffer
     */
    reverse_iterator rend() noexcept;

    /**!
     * @brief Returns a const_iterator to the end of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_iterator cend() const noexcept;

    /**!
     * @brief Returns a const_reverse_iterator to the end of the Buffer
     *
     * @return iterator Front of buffer
     */
    const_reverse_iterator crend() const noexcept;

    /**!
     * @brief Returns the value of the element at a valid index relative to the front of the Buffer. Invalid positions
     * will result in undefined behavior.
     *
     * @param[in] index Relative to front
     * @return T& Reference to element value
     */
    T& at(size_type index) noexcept;

    /**!
     * @brief Returns the value of the element at a valid index relative to the front of the Buffer. Invalid positions
     * will result in undefined behavior.
     *
     * @param[in] index Relative to front
     * @return T const& Reference to element value
     */
    T const& at(size_type index) const noexcept;

    /**!
     * @brief Returns the value of the element at the front of the Buffer. Will produce undefined behavior if the Buffer
     * is empty.
     *
     * @return T const& Reference to element value
     */
    T& front() noexcept;

    /**!
     * @brief Returns the value of the element at the front of the Buffer. Will produce undefined behavior if the Buffer
     * is empty.
     *
     * @return T const& Reference to element value
     */
    T const& front() const noexcept;

    /**!
     * @brief Returns the value of the element at the back of the Buffer. Will produce undefined behavior if the Buffer
     * is empty.
     *
     * @return T const& Reference to element value
     */
    T& back() noexcept;

    /**!
     * @brief Returns the value of the element at the back of the Buffer. Will produce undefined behavior if the Buffer
     * is empty.
     *
     * @return T const& Reference to element value
     */
    T const& back() const noexcept;

    /**!
     * @brief Assigns a value to n elements within the Buffer, starting at the front and capped at the Buffer's max
     * size.
     *
     * @param[in] n Number of elements to assign
     * @param[in] value Value to assign to elements
     */
    void assign(size_type n, T const& value) noexcept;

    /**!
     * @brief Assigns the contents of an initializer list to elements within the Buffer, starting at the front and
     * capped at the Buffer's max size.
     *
     * @param[in] il Initializer list
     */
    void assign(std::initializer_list<T> il) noexcept;

    /**!
     * @brief Assigns the contents of a range to elements within the buffer, starting at the front and capped at the
     * Buffer's max size.
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param[in] i Range start
     * @param[in] j Range end (not included in contents). Must compare >= i.
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    void assign(IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Constructs an element in-place at any position within the Buffer. Valid elements behind that position will
     * be shifted back, with those that do not fit being truncated backmost-first. Emplacing at the back when the Buffer
     * is full will truncate the front element.
     *
     * @tparam ValueTypes Element constructor argument parameter types
     * @param[in] position Emplace location
     * @param[in] args Element constructor argument parameters
     * @return iterator Position of emplaced element
     */
    template<typename... ValueTypes>
    iterator emplace(iterator position, ValueTypes&&... args) noexcept;

    /**!
     * @brief Constructs an element in-place at the front of the Buffer
     *
     * @tparam ValueTypes Element constructor argument parameter types
     * @param[in] args Element constructor argument parameters
     */
    template<typename... ValueTypes>
    void emplace_front(ValueTypes&&... args) noexcept;

    /**!
     * @brief Constructs an element in-place at the back of the Buffer
     *
     * @tparam ValueTypes Element constructor argument parameter types
     * @param[in] args Element constructor argument parameters
     */
    template<typename... ValueTypes>
    void emplace_back(ValueTypes&&... args) noexcept;

    /**!
     * @brief Inserts a value at any position within the Buffer. Pre-existing elements behind that position will be
     * shifted back, with those that do not fit being truncated backmost-first. Inserting at the back when the Buffer is
     * full will truncate the front element.
     *
     * @param[in] position Insert position
     * @param[in] value To insert
     * @return iterator Position of inserted element
     */
    iterator insert(iterator position, T const& value) noexcept;

    /**!
     * @brief Inserts a value at any position within the Buffer using move semantics. Pre-existing elements behind that
     * position will be shifted back, with those that do not fit being truncated backmost-first. Inserting at the back
     * when the Buffer is full will truncate the front element.
     *
     * @param[in] position Insert position
     * @param[in] value Instance of value_type to insert
     * @return iterator Position of inserted element
     */
    iterator insert(iterator position, T&& value) noexcept;

    /**!
     * @brief Inserts n values at any position within the Buffer. Pre-existing elements behind the insertion zone
     * [position, position + n) will be shifted back, with those that do not fit being truncated backmost-first. If the
     * insertion itself wraps around the back of the buffer, the front will be truncated by the difference.
     *
     * @param[in] position Insert position
     * @param[in] n Number of values to insert
     * @param[in] value To insert
     * @return iterator First inserted position
     */
    iterator insert(iterator position, size_type n, T const& value) noexcept;

    /**!
     * @brief Inserts the contents of an initializer list at any position within the Buffer. Pre-existing elements
     * behind the insertion zone [position, position + list size) will be shifted back, with those that do not fit being
     * truncated backmost-first.  If the insertion itself wraps around the back of the buffer, the front will be
     * truncated by the difference.
     *
     * @param[in] position Insert position
     * @param[in] il Initializer list
     * @return iterator First inserted position
     */
    iterator insert(iterator position, std::initializer_list<T> il) noexcept;

    /**!
     * @brief Inserts the contents of a range at any position within the Buffer. Pre-existing elements behind the
     * insertion zone [position, position + range size) will be shifted back, with those that do not fit being truncated
     * backmost-first.  If the insertion itself wraps around the back of the buffer, the front will be truncated by the
     * difference.
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param[in] position Insert position
     * @param[in] i Range start
     * @param[in] j Range end (not included in contents). Must compare >= i.
     * @return iterator First inserted position
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    iterator insert(iterator position, IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Pops the front element from the Buffer, destroying it
     *
     */
    void pop_front() noexcept;

    /**!
     * @brief Pushes a new element to the front of the Buffer
     *
     * @param value Value of element
     */
    void push_front(T const& value) noexcept;

    /**!
     * @brief Pushes a new element to the front of the Buffer using move semantics
     *
     * @param value Instance of value_type to push
     */
    void push_front(T&& value) noexcept;

    /**!
     * @brief Pops the back element from the Buffer, destroying it
     *
     */
    void pop_back() noexcept;

    /**!
     * @brief Pushes a new element to the back of the Buffer
     *
     * @param value Value of element
     */
    void push_back(T const& value) noexcept;

    /**!
     * @brief Pushes a new element to the back of the Buffer using move semantics
     *
     * @param value Instance of value_type to push
     */
    void push_back(T&& value) noexcept;

    /**!
     * @brief Swaps the contents of the Buffer with another
     *
     * @param rhs Buffer to swap contents with
     */
    void swap(Buffer& rhs) noexcept;

    /**!
     * @brief Erases an element at a valid position within the Buffer, destorying it
     *
     * @param position Erasure position
     * @return iterator Element that was immediately following 'position' prior to erasure
     */
    iterator erase(iterator position) noexcept;

    /**!
     * @brief Erases valid elements in range (i, j], destroying them
     *
     * @param i Start of erasure
     * @param j End of erasure (not destroyed). Must compare >= i.
     * @return iterator Element that was immediately following 'j' prior to erasure
     */
    iterator erase(iterator i, iterator j) noexcept;

    /**!
     * @brief Resets the Buffer's max size, clearing it in the process
     *
     * @param n New max size
     */
    void resize(size_type n) noexcept;

    // Operator overloads

    Buffer& operator=(Buffer const& rhs) noexcept;
    Buffer& operator=(Buffer&& rhs) noexcept;
    Buffer& operator=(std::initializer_list<T> const& il) noexcept;

    bool operator!=(const Buffer& lhs) noexcept;

    T&       operator[](size_type index) noexcept;
    T const& operator[](size_type index) const noexcept;

    // Friend to all specialized Buffer overloads of operator== and operator!=
    template<typename ValueType, typename AllocatorTypeArg>
    friend bool operator==(Buffer<ValueType, AllocatorTypeArg> const& lhs,
                           Buffer<ValueType, AllocatorTypeArg> const& rhs) noexcept;
    template<typename ValueType, typename AllocatorTypeArg>
    friend bool operator!=(Buffer<ValueType, AllocatorTypeArg> const& lhs,
                           Buffer<ValueType, AllocatorTypeArg> const& rhs) noexcept;

private:
    /**!
     * @brief Fill-inserts a zone within the buffer.
     *
     * @tparam ValueType T& or T&&
     * @param position Start of insert
     * @param n Size of insert zone
     * @param value Value to fill-in
     * @return iterator First inserted position
     */
    template<typename ValueType>
    iterator FillInsert(iterator position, size_type n, ValueType&& value) noexcept;

    /**!
     * @brief Inserts the contents of a range to a zone within the buffer
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param position Start of insert
     * @param i Range start
     * @param j Range end (not included in contents)
     * @return iterator First inserted position
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    iterator RangeInsert(iterator position, IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Fill-inserts a zone within the Buffer, starting at the front. If the insert wraps around, the back will be
     * truncated by the difference.
     *
     * @tparam ValueType T& or T&&
     * @param n Size of insert zone
     * @param value Value to fill-in
     * @return iterator First inserted position
     */
    template<typename ValueType>
    iterator FillPushFront(size_type n, ValueType&& value) noexcept;

    /**!
     * @brief Fill-inserts a zone within the Buffer, starting at the back. If the insert wraps around, the front will be
     * truncated by the difference.
     *
     * @tparam ValueType T& or T&&
     * @param n Size of insert zone
     * @param value Value to fill-in
     * @return iterator First inserted position
     */
    template<typename ValueType>
    iterator FillPushBack(size_type n, ValueType&& value) noexcept;

    /**!
     * @brief Inserts the contents of a range to a zone within the buffer, starting at the front. If the insert wraps
     * around, the back will be truncated by the difference.
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param i Range start
     * @param j Range end (not included in contents)
     * @return iterator First inserted position
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    iterator RangePushFront(IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Inserts the contents of a range to a zone within the buffer, starting at the back. If the insert wraps
     * around, the front will be truncated by the difference.
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param i Range start
     * @param j Range end (not included in contents)
     * @return iterator First inserted position
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    iterator RangePushBack(IteratorType i, IteratorType j) noexcept;

    /**!
     * @brief Assigns a value to elements in the Buffer
     *
     * @tparam ValueType T& or T&&
     * @param position First element to be assigned
     * @param n Number of elements to assign
     * @param value To assign
     * @return detail::Buffer::Iterator Position past the last assigned element
     */
    template<typename ValueType>
    detail::Buffer::Iterator FillInForward(iterator position, size_type n, ValueType&& value) noexcept;

    /**!
     * @brief Assigns the contents of a range to elements in the Buffer
     *
     * @tparam IteratorType Range iterator type
     * @tparam typename (anonymous) Requires IteratorType to meet the "Legacy Input Iterator" named requirements
     * @param position First element to be assigned
     * @param i Range start
     * @param j Range end (not included in contents)
     * @return detail::Buffer::Iterator Position past the last assigned element
     */
    template<typename IteratorType, typename = std::_RequireInputIter<IteratorType>>
    detail::Buffer::Iterator FillRangeForward(iterator position, IteratorType i, IteratorType j) noexcept;

    /**
     * @brief Erases a section starting from the front of the Buffer, destroying the elements
     *
     * @param n Size of the erasure zone
     * @return detail::Buffer::Iterator First element behind the erasure zone
     */
    iterator EraseFront(size_type n);

    /**
     * @brief Erases a section starting from the back of the Buffer (moving forwards), destroying the elements
     *
     * @param n Size of the erasure zone
     */
    void EraseBack(size_type n);

    /**!
     * @brief Erases a section from the Buffer, destroying the elements
     *
     * @param start Start of the erasure zone
     * @param n Size of the erasure zone
     * @return detail::Buffer::Iterator Element that was immediately following 'start' prior to erasure
     */
    iterator EraseSection(iterator start, size_type n);

    // Helper methods for allocating, copying, and moving Buffer contents.
    // I don't feel like I need to add any more green to this page, would you agree?
    T*   AllocateBuffer(size_type n) noexcept;
    void DeallocateBuffer() noexcept;

    void CopyBuffer(Buffer const& rhs) noexcept;
    void CopyAssignWithAllocatorPropogation(const Buffer& rhs, std::true_type t) noexcept;
    void CopyAssignWithAllocatorPropogation(const Buffer& rhs, std::false_type f) noexcept;

    void MoveAssignWithAllocatorPropogation(Buffer&& rhs, std::true_type t) noexcept;
    void MoveAssignWithAllocatorPropogation(Buffer&& rhs, std::false_type f) noexcept;

    AllocatorType m_allocator; //!< Each Buffer gets its own individual Allocator!
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer constructor definitions          ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer() noexcept : detail::Buffer(sizeof(T))
{
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(AllocatorType const& allocator) noexcept
    : detail::Buffer(sizeof(T)), m_allocator {allocator}
{
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(Buffer<T, AllocatorType>::size_type buffer_size) noexcept : Buffer()
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(size_type buffer_size, AllocatorType const& allocator) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(Buffer<T, AllocatorType>::size_type buffer_size, T const& init) noexcept
    : Buffer(buffer_size)
{
    if (m_start_address)
    {
        // Set back of buffer to the end of the available space
        m_back = IncrementBoundless(m_front, m_max_element_count - 1);

        // Construct elements in the buffer
        // Need to typecast 'm_start_address' from void* before performing pointer arithmetic
        for (T& e : *this)
            alloc_traits::construct(m_allocator, (T*)&e, init);

        // Buffer is full
        AtCapacity();
    }
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(Buffer<T, AllocatorType>::size_type buffer_size, T const& init,
                                 AllocatorType const& allocator) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name) noexcept : detail::Buffer(name, sizeof(T))
{
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, AllocatorType const& allocator) noexcept
    : detail::Buffer(name, sizeof(T)), m_allocator {allocator}
{
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, Buffer<T, AllocatorType>::size_type buffer_size) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, Buffer<T, AllocatorType>::size_type buffer_size,
                                 T const& init) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, Buffer<T, AllocatorType>::size_type buffer_size, T const& init,
                                 AllocatorType const& allocator) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(std::initializer_list<T> il) noexcept : Buffer(il.size())
{
    if (m_start_address)
        assign(il);
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(std::initializer_list<T> il, AllocatorType const& allocator) noexcept
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, std::initializer_list<T> il) noexcept : Buffer(name, il.size())
{
    if (m_start_address)
        assign(il);
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(char const* name, std::initializer_list<T> il, AllocatorType const& allocator) noexcept
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

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
Buffer<T, AllocatorType>::Buffer(IteratorType i, IteratorType j) noexcept : Buffer(to_unsigned(j - i))
{
    if (m_start_address)
        assign(i, j);
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
Buffer<T, AllocatorType>::Buffer(char const* name, IteratorType i, IteratorType j) noexcept
    : Buffer(name, std::min((j - i), 0u))
{
    if (m_start_address)
        assign(i, j);
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
Buffer<T, AllocatorType>::Buffer(char const* name, IteratorType i, IteratorType j,
                                 AllocatorType const& allocator) noexcept
    : detail::Buffer(name, sizeof(T)), m_allocator {allocator}
{
    // Only continue if the range size is > 0
    size_type range_size = std::min((j - i), 0u);
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

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(Buffer<T, AllocatorType> const& rhs) noexcept
    : detail::Buffer(rhs.GetName(), sizeof(T)),
      m_allocator(alloc_traits::select_on_container_copy_construction(rhs.m_allocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
    CopyBuffer(rhs);
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::Buffer(Buffer<T, AllocatorType>&& rhs) noexcept
    : detail::Buffer(rhs.GetName(), sizeof(T)), m_allocator(std::move(rhs.m_allocator))
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
    MoveMemoryContents(std::forward<detail::Buffer&&>(rhs));
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>::~Buffer() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(destructing)
    DeallocateBuffer();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer method definitions in alphabetical order     ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::assign(Buffer<T, AllocatorType>::size_type n, T const& value) noexcept
{
    // Guard against a count of 0
    if (!n)
        return;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(assign, shmit::log::diagnostics::Print("Count:%u", n));

    // Guard against overfill, then fill in the Buffer
    n                                       = OverfillGuard(n);
    detail::Buffer::Iterator assignment_end = FillInForward(m_front, n, value);

    if (assignment_end == m_front)
    {
        m_back = DecrementBoundless(assignment_end, 1);
        AtCapacity();
    }
    else if (assignment_end > m_back)
        m_back = assignment_end;
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::assign(std::initializer_list<T> il) noexcept
{
    assign(il.begin(), il.end());
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
void Buffer<T, AllocatorType>::assign(IteratorType i, IteratorType j) noexcept
{
    size_type range_size = to_unsigned(j - i);

    // Guard against empty range
    if (!range_size)
        return;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(assign, shmit::log::diagnostics::Print("Count:%u", range_size));

    // Guard against overfills
    size_type corrected_size      = OverfillGuard(range_size);
    size_type list_reduction_bias = range_size - corrected_size;
    j -= list_reduction_bias;

    // Fill in range, starting at front
    detail::Buffer::Iterator assignment_end = FillRangeForward(m_front, i, j);

    if (assignment_end == m_front)
    {
        m_back = DecrementBoundless(assignment_end, 1);
        AtCapacity();
    }
    else if (assignment_end > m_back)
        m_back = assignment_end;
}

template<typename T, class AllocatorType>
T& Buffer<T, AllocatorType>::at(Buffer<T, AllocatorType>::size_type index) noexcept
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

template<typename T, class AllocatorType>
T const& Buffer<T, AllocatorType>::at(Buffer<T, AllocatorType>::size_type index) const noexcept
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

template<typename T, class AllocatorType>
T& Buffer<T, AllocatorType>::back() noexcept
{
    iterator back = end() - 1;
    return *back;
}

template<typename T, class AllocatorType>
T const& Buffer<T, AllocatorType>::back() const noexcept
{
    const_iterator back = cend() - 1;
    return *back;
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::begin() noexcept
{
    return iterator(m_front);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_iterator Buffer<T, AllocatorType>::begin() const noexcept
{
    // If the Buffer is empty, return end() instead
    if (empty())
        return end();

    return const_iterator(m_front);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_iterator Buffer<T, AllocatorType>::cbegin() const noexcept
{
    // If the Buffer is empty, return cend() instead
    if (empty())
        return cend();

    return const_iterator(m_front);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_iterator Buffer<T, AllocatorType>::cend() const noexcept
{
    detail::Buffer::Iterator end = m_back;
    IncrementBounded(end, 1);
    return const_iterator(end);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_reverse_iterator Buffer<T, AllocatorType>::crbegin() const noexcept
{
    // If the Buffer is empty, return crend() instead
    if (empty())
        return crend();

    return const_reverse_iterator(m_front);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_reverse_iterator Buffer<T, AllocatorType>::crend() const noexcept
{
    detail::Buffer::Iterator end = m_back;
    IncrementBounded(end, 1);
    return const_reverse_iterator(end);
}

template<typename T, class AllocatorType>
template<typename... ValueTypes>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::emplace(
    typename Buffer<T, AllocatorType>::iterator position, ValueTypes&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_random)
    PrepareForRandomPlacement(position, 1);
    alloc_traits::construct(m_allocator, &(*position), std::forward<ValueTypes>(args)...);

    return position;
}

template<typename T, class AllocatorType>
template<typename... ValueTypes>
void Buffer<T, AllocatorType>::emplace_back(ValueTypes&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_back)

    // Emplace position is the back of the Buffer, unless the buffer is full
    iterator emplace_position = m_back;
    if (m_is_full)
        emplace_position = IncrementBoundless(emplace_position, 1);

    // Emplace element
    alloc_traits::construct(m_allocator, &(*emplace_position), std::forward<ValueTypes>(args)...);

    // Detect overflow and capacity
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

template<typename T, class AllocatorType>
template<typename... ValueTypes>
void Buffer<T, AllocatorType>::emplace_front(ValueTypes&&... args) noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(emplace_front)

    // Emplace position is 1 before the front of the Buffer, always
    iterator emplace_position = DecrementBoundless(m_front, 1);

    // Emplace element
    alloc_traits::construct(m_allocator, &(*emplace_position), std::forward<ValueTypes>(args)...);

    // Detect overflow and capacity
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

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::end() noexcept
{
    detail::Buffer::Iterator end = m_back;
    IncrementBounded(end, 1);
    return iterator(end);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::const_iterator Buffer<T, AllocatorType>::end() const noexcept
{
    detail::Buffer::Iterator end = m_back;
    IncrementBounded(end, 1);
    return const_iterator(end);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::erase(typename Buffer<T, AllocatorType>::iterator position) noexcept
{
    // Call the appropriate erase subroutine based on position's relative location to the front and back
    if (position == m_front)
        return EraseFront(1);
    else if (position < m_back)
        return EraseSection(position, 1);
    else if (position == m_back)
        EraseBack(1);

    // Otherwise (or if the back element was just erased) return the end of the Buffer
    return end();
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::erase(
    typename Buffer<T, AllocatorType>::iterator i, typename Buffer<T, AllocatorType>::iterator j) noexcept
{
    size_type range_size = to_unsigned(j - i);
    if (i == m_front)
        return EraseFront(range_size);
    else if (j < m_back)
        return EraseSection(i, range_size);
    else if (j == m_back)
        EraseBack(range_size);

    // Otherwise (or if the back was just erased) return the end of the Buffer
    return end();
}

template<typename T, class AllocatorType>
T& Buffer<T, AllocatorType>::front() noexcept
{
    iterator front = begin();
    return *front;
}

template<typename T, class AllocatorType>
T const& Buffer<T, AllocatorType>::front() const noexcept
{
    const_iterator front = cbegin();
    return *front;
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::insert(typename Buffer<T, AllocatorType>::iterator position, T const& value) noexcept
{
    // Make a copy of the reference value, it could exist in the buffer already and be affected by the
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == m_front)
        return FillPushFront(1, copyOfValue);
    else if (position >= m_back)
        return FillPushBack(1, copyOfValue);

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, copyOfValue);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::insert(typename Buffer<T, AllocatorType>::iterator position, T&& value) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == m_front)
        return FillPushFront(1, std::forward<T>(value));
    else if (position >= m_back)
        return FillPushBack(1, std::forward<T>(value));

    // Otherwise call the random-access insert routine
    return FillInsert(position, 1, std::forward<T>(value));
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::insert(typename Buffer<T, AllocatorType>::iterator position,
                                     Buffer<T, AllocatorType>::size_type n, T const& value) noexcept
{
    // Guard against a count of 0
    if (!n)
        return position;

    // Make a copy of the reference value, it could exist in the buffer already and be affected by the
    // moving/truncating of elements
    T copyOfValue = value;

    // If the position is the back or beginning of the buffer, perform a push
    if (position == m_front)
        return FillPushFront(n, copyOfValue);
    else if (position >= m_back)
        return FillPushBack(n, copyOfValue);

    // Otherwise call the random-access insert routine
    return FillInsert(position, n, copyOfValue);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::insert(
    typename Buffer<T, AllocatorType>::iterator position, std::initializer_list<T> il) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == m_front)
        return RangePushFront(il.begin(), il.end());
    else if (position >= m_back)
        return RangePushBack(il.begin(), il.end());

    // Otherwise call the random-access insert routine
    return RangeInsert(position, il.begin(), il.end());
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::insert(
    typename Buffer<T, AllocatorType>::iterator position, IteratorType i, IteratorType j) noexcept
{
    // If the position is the back or beginning of the buffer, perform a push
    if (position == m_front)
        return RangePushFront(i, j);
    else if (position >= m_back)
        return RangePushBack(i, j);

    // Otherwise call the random-access insert routine
    return RangeInsert(position, i, j);
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::pop_back() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(pop_back)

    // Check for empty condition
    if (!empty())
    {
        // Move back of buffer backwards 1 step unless full and clear full flag
        if (!full())
            m_back = DecrementBoundless(m_back, 1);
        m_is_full = false;

        // Destroy popped element
        iterator popped = m_back;
        alloc_traits::destroy(m_allocator, (T*)&(*popped));
    }
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::pop_front() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(pop_front)

    // Check for empty condition
    if (!empty())
    {
        iterator popped = m_front;

        // Move front of buffer fowards 1 step and clear full flag
        m_front   = IncrementBoundless(m_front, 1);
        m_is_full = false;

        // Destroy popped element
        alloc_traits::destroy(m_allocator, (T*)&(*popped));
    }
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::push_back(T const& value) noexcept
{
    FillPushBack(1, value);
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::push_back(T&& value) noexcept
{
    FillPushBack(1, std::forward<T>(value));
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::push_front(T const& value) noexcept
{
    FillPushFront(1, value);
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::push_front(T&& value) noexcept
{
    FillPushFront(1, std::forward<T>(value));
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::reverse_iterator Buffer<T, AllocatorType>::rbegin() noexcept
{
    // If the Buffer is empty, return rend() instead
    if (empty())
        return rend();

    return reverse_iterator(m_front);
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::reverse_iterator Buffer<T, AllocatorType>::rend() noexcept
{
    detail::Buffer::Iterator end = m_back;
    IncrementBounded(end, 1);
    return reverse_iterator(end);
}

// TODO refactor
template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::resize(Buffer<T, AllocatorType>::size_type n) noexcept
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

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::swap(Buffer& rhs) noexcept
{
    detail::Buffer::Swap(rhs);

    // If AllocatorType requires that it be propogated on container swap, do so now
    if (alloc_traits::propagate_on_container_swap::value == true)
    {
        AllocatorType tempAllocator = m_allocator;
        m_allocator                 = rhs.m_allocator;
        rhs.m_allocator             = tempAllocator;
    }
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>& Buffer<T, AllocatorType>::operator=(Buffer<T, AllocatorType> const& rhs) noexcept
{
    // Tag-dispatch the appropriate routine, selected at compile time
    CopyAssignWithAllocatorPropogation(rhs, typename alloc_traits::propagate_on_container_copy_assignment {});

    return *this;
}

template<typename T, class AllocatorType>
Buffer<T, AllocatorType>& Buffer<T, AllocatorType>::operator=(Buffer<T, AllocatorType>&& rhs) noexcept
{
    // Tag-dispatch the appropriate routine, selected at compile time
    MoveAssignWithAllocatorPropogation(std::forward<Buffer<T, AllocatorType>>(rhs),
                                       typename alloc_traits::propagate_on_container_move_assignment {});

    return *this;
}

// TODO refactor
template<typename T, class AllocatorType>
Buffer<T, AllocatorType>& Buffer<T, AllocatorType>::operator=(std::initializer_list<T> const& il) noexcept
{
    size_type il_size = il.size();
    if (m_max_element_count != il_size)
        resize(il_size);
    else
        clear();

    assign(il);

    return *this;
}

template<typename T, class AllocatorType>
T& Buffer<T, AllocatorType>::operator[](Buffer<T, AllocatorType>::size_type index) noexcept
{
    iterator it = begin();
    std::advance(it, index);
    return *it;
}

template<typename T, class AllocatorType>
T const& Buffer<T, AllocatorType>::operator[](Buffer<T, AllocatorType>::size_type index) const noexcept
{
    const_iterator it = cbegin();
    std::advance(it, index);
    return *it;
}

//  Private ============================================================================================================

template<typename T, class AllocatorType>
T* Buffer<T, AllocatorType>::AllocateBuffer(Buffer<T, AllocatorType>::size_type n) noexcept
{
    // Guard against Buffer sizes larger than the allowable limit
    if (n > kMaxAllowableSize)
        n = kMaxAllowableSize;

    T* bufferPtr = alloc_traits::allocate(m_allocator, n);
    if (bufferPtr == nullptr)
        LOG_MEMBER_DIAGNOSTIC_POSIT(bad_alloc)

    return bufferPtr;
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::CopyAssignWithAllocatorPropogation(Buffer<T, AllocatorType> const& rhs,
                                                                  std::true_type                  t) noexcept
{
    // Deallocate current buffer, the lhs allocator may not be able to later
    DeallocateBuffer();

    // Assign the right-hand-side's allocator to this instance, then copy-assign buffer
    m_allocator = rhs.m_allocator;
    CopyBuffer(rhs);
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::CopyAssignWithAllocatorPropogation(Buffer<T, AllocatorType> const& rhs,
                                                                  std::false_type                 f) noexcept
{
    if (m_max_element_count != rhs.m_max_element_count)
        resize(rhs.m_max_element_count);
    else
        clear(); // Just clear the current buffer

    // Copy Buffer contents
    assign(rhs.begin(), rhs.end());
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::CopyBuffer(Buffer const& rhs) noexcept
{
    if (rhs.m_start_address && rhs.m_max_element_count)
    {
        // Allocate new buffer with right-hand-side size, then assign contents
        m_start_address = AllocateBuffer(rhs.m_max_element_count);
        if (m_start_address)
        {
            m_max_element_count = rhs.m_max_element_count;
            assign(rhs.begin(), rhs.end());
        }
    }
    // Failure to allocate here will post a diagnostic log posit
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::DeallocateBuffer() noexcept
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

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::EraseFront(Buffer<T, AllocatorType>::size_type n)
{
    iterator* it = static_cast<iterator*>(&m_front); // Cast to typed iterator for access to operator*

    if (n)
    {
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(erase_front, shmit::log::diagnostics::Print("Count:%u", n))

        // Guard against overfill, even though it's technically the opposite
        size_type current_size = size();
        if (n > current_size)
            n = current_size;

        for (size_t i = 0; i < n; i++)
        {
            // Destroy the element then increment
            alloc_traits::destroy(m_allocator, (T*)&(*(*it)));
            *it = IncrementBoundless(*it, 1);
        }

        if (m_is_full)
            m_back = IncrementBoundless(m_back, 1);
        m_is_full = false;
    }

    return *it;
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::EraseBack(Buffer<T, AllocatorType>::size_type n)
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(erase_back, shmit::log::diagnostics::Print("Count:%u", n))

    if (n)
    {
        // Guard against overfill, even though it's technically the opposite
        size_type current_size = size();
        if (n > current_size)
            n = current_size;

        iterator* it = static_cast<iterator*>(&m_back); // Cast to typed iterator for access to operator*
        if (m_is_full)
        {
            // If Buffer is full, delete the backmost element before looping through the rest
            alloc_traits::destroy(m_allocator, (T*)&(*(*it)));
            n--;
        }

        for (size_t i = 0; i < n; i++)
        {
            // Decrement, then destroy element
            *it = DecrementBoundless(*it, 1);
            alloc_traits::destroy(m_allocator, (T*)&(*(*it)));
        }

        // Clear full flag
        m_is_full = false;
    }
}

template<typename T, class AllocatorType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::EraseSection(
    typename Buffer<T, AllocatorType>::iterator start, Buffer<T, AllocatorType>::size_type n)
{
    if (n)
    {
        size_type start_wrapped      = (unsigned)(start - begin());
        size_type distance_from_back = size() - start_wrapped;

        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(erase_random,
                                                shmit::log::diagnostics::Print("Count:%u,At:%u", n, start_wrapped))

        // Determine if erasure will extend to the back of the buffer
        if (n >= distance_from_back)
        {
            // Sneaky call out to EraseBack() for the heavy lifting
            // Using 'distance_from_back' so that we are not erasing more elements than the Buffer currently contains
            EraseBack(distance_from_back);
            return end();
        }

        // Else, there are elements behind the erasure zone which need to be shifted up in order to be preserved

        // Destroy elements in erasure zone
        iterator erase_curr = start;
        iterator erase_end  = IncrementBoundless(start, n);
        while (erase_curr < erase_end)
        {
            alloc_traits::destroy(m_allocator, (T*)&(*erase_curr));
            erase_curr++;
        }

        // From the end of the erasure zone to the back of the Buffer is the source zone for shifting
        // The destination is the zone that was just erased
        // ShiftContents() will return an iterator equivalent to 'erase_end', so we ignore it
        size_type        num_preserved = distance_from_back - n;
        Buffer::Iterator dest_end      = ShiftContents(erase_end, start, num_preserved);

        // Update the back of the Buffer to track the end of the shifted content and clear full flag
        m_back    = dest_end;
        m_is_full = false;
    }

    // Return the first element to get shifted, which is now conveniently located at 'start'
    // Or, if n == 0, we're returning the first element to NOT get erased
    return start;
}

template<typename T, class AllocatorType>
template<typename ValueType>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::FillInsert(
    Buffer<T, AllocatorType>::iterator position, Buffer<T, AllocatorType>::size_type n, ValueType&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_random, shmit::log::diagnostics::Print("Count:%u", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Prepare for placement at random position, this adjusts the front and back of the Buffer internally
    PrepareForRandomPlacement(position, n);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillInForward(position, n, std::forward<ValueType>(value));

    return position;
}

template<typename T, class AllocatorType>
template<typename ValueType> // T& or T&&
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::FillPushBack(Buffer<T, AllocatorType>::size_type n, ValueType&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_back, shmit::log::diagnostics::Print("Count:%u", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Insert start position is the back of the Buffer, unless the buffer is full
    detail::Buffer::Iterator insert_start = m_back;
    if (m_is_full)
        insert_start = IncrementBoundless(insert_start, 1);

    // Perform insert
    detail::Buffer::Iterator insert_end = FillInForward(insert_start, n, std::forward<ValueType>(value));
    PostInsertBack(insert_end);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class AllocatorType>
template<typename ValueType> // T& or T&&
typename Buffer<T, AllocatorType>::iterator
    Buffer<T, AllocatorType>::FillPushFront(Buffer<T, AllocatorType>::size_type n, ValueType&& value) noexcept
{
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_front, shmit::log::diagnostics::Print("Count:%u", n))

    // Guard against overfill
    n = OverfillGuard(n);

    // Insert start is n places before the front of the Buffer
    detail::Buffer::Iterator insert_start = DecrementBoundless(m_front, n);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillInForward(insert_start, n, std::forward<ValueType>(value));
    PostInsertFront(insert_start);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class AllocatorType>
template<typename ValueType> // T& or T&&
detail::Buffer::Iterator Buffer<T, AllocatorType>::FillInForward(typename Buffer<T, AllocatorType>::iterator position,
                                                                 Buffer<T, AllocatorType>::size_type         n,
                                                                 ValueType&& value) noexcept
{
    for (size_t count = 0; count < n; count++)
    {
        *position = value;
        position  = IncrementBoundless(position, 1);
    }

    // Upcast to base class on return
    return position;
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
detail::Buffer::Iterator Buffer<T, AllocatorType>::FillRangeForward(
    typename Buffer<T, AllocatorType>::iterator position, IteratorType i, IteratorType j) noexcept
{
    size_type range_size = to_unsigned(j - i);
    for (size_t count = 0; count < range_size; count++)
    {
        *position = *i++;
        position  = IncrementBoundless(position, 1);
    }

    // Upcast to base class on return
    return position;
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::MoveAssignWithAllocatorPropogation(Buffer<T, AllocatorType>&& rhs,
                                                                  std::true_type             t) noexcept
{
    // Deallocate the current buffer, move the allocator from the right hand side to here, and move memory contents
    DeallocateBuffer();
    m_allocator = std::move(rhs.m_allocator);
    MoveMemoryContents(std::forward<detail::Buffer>(rhs));
}

template<typename T, class AllocatorType>
void Buffer<T, AllocatorType>::MoveAssignWithAllocatorPropogation(Buffer<T, AllocatorType>&& rhs,
                                                                  std::false_type            f) noexcept
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

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::RangeInsert(
    typename Buffer<T, AllocatorType>::iterator position, IteratorType i, IteratorType j) noexcept
{
    // Guard against overfill
    size_type range_size          = to_unsigned(j - i);
    size_type corrected_size      = OverfillGuard(range_size);
    size_type list_reduction_bias = range_size - corrected_size;
    j -= list_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_random, shmit::log::diagnostics::Print("Count:%u", corrected_size))

    // Prepare for placement at random position, this adjusts the front and back of the Buffer internally
    PrepareForRandomPlacement(position, corrected_size);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillRangeForward(position, i, j);

    return position;
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::RangePushBack(IteratorType i,
                                                                                    IteratorType j) noexcept
{
    // Guard against overfill
    size_type range_size           = to_unsigned(j - i);
    size_type corrected_size       = OverfillGuard(range_size);
    size_type range_reduction_bias = range_size - corrected_size;
    j -= range_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_back, shmit::log::diagnostics::Print("Count:%u", corrected_size))

    // Insert start position is the back of the Buffer, unless the buffer is full
    detail::Buffer::Iterator insert_start = m_back;
    if (m_is_full)
        insert_start = IncrementBoundless(insert_start, 1);

    // Perform insert
    detail::Buffer::Iterator insert_end = FillRangeForward(insert_start, i, j);
    PostInsertBack(insert_end);

    // Construct typed iterator and return
    return iterator(insert_start);
}

template<typename T, class AllocatorType>
template<typename IteratorType, typename>
typename Buffer<T, AllocatorType>::iterator Buffer<T, AllocatorType>::RangePushFront(IteratorType i,
                                                                                     IteratorType j) noexcept
{
    // Guard against overfill
    size_type range_size           = to_unsigned(j - i);
    size_type corrected_size       = OverfillGuard(range_size);
    size_type range_reduction_bias = range_size - corrected_size;
    j -= range_reduction_bias;

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(insert_front, shmit::log::diagnostics::Print("Count:%u", corrected_size))

    // Insert start is n places before the front of the Buffer
    detail::Buffer::Iterator insert_start = DecrementBoundless(m_front, corrected_size);

    // Perform insert
    // End of insert is not needed, void return
    (void)FillRangeForward(insert_start, i, j);
    PostInsertFront(insert_start);

    // Construct typed iterator and return
    return iterator(insert_start);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  External method definitions in alphabetical order       ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ValueType, typename AllocatorTypeArg = std::allocator<ValueType>>
bool operator==(Buffer<ValueType, AllocatorTypeArg> const& lhs, Buffer<ValueType, AllocatorTypeArg> const& rhs) noexcept
{
    using const_iterator = typename Buffer<ValueType, AllocatorTypeArg>::const_iterator;

    // Compare sizes first
    if (lhs.size() == rhs.size())
    {
        // Compare buffer contents
        const_iterator rhsCurr = rhs.cbegin();
        for (ValueType const& lhsVal : lhs)
        {
            if (*rhsCurr++ != lhsVal)
                return false;
        }

        // If here, the contents of the buffers are equivalent
        return true;
    }

    return false;
}

template<typename ValueType, typename AllocatorTypeArg = std::allocator<ValueType>>
bool operator!=(Buffer<ValueType, AllocatorTypeArg> const& lhs, Buffer<ValueType, AllocatorTypeArg> const& rhs) noexcept
{
    return !(lhs == rhs);
}

} // End namespace shmit

namespace std
{

template<typename T, class AllocatorType>
void swap(shmit::Buffer<T, AllocatorType>& rhs, shmit::Buffer<T, AllocatorType>& lhs) noexcept
{
    rhs.swap(lhs);
}

} // End namespace std
