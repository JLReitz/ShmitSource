#include <ShmitCore/Types/Containers/Detail/Buffer.hpp>

#include <cstring>

namespace shmit
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer Constructor definitions      ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Protected   ========================================================================================================

// clang-format off
Buffer::Buffer(Buffer::size_type element_size_bytes) noexcept : m_element_size_bytes {element_size_bytes}
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
}

// clang-format off
Buffer::Buffer(char const* name, Buffer::size_type element_size_bytes) noexcept
    : Named(name), m_element_size_bytes {element_size_bytes}
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer methods in alphabetical order        ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

void Buffer::clear() noexcept
{
    m_is_full = false;
    m_back = m_front = Buffer::BufferIterator(*this);
}

bool Buffer::empty() const noexcept
{
    return m_front == m_back;
}

bool Buffer::full() const noexcept
{
    return m_is_full;
}

Buffer::size_type Buffer::max_size() const noexcept
{
    return m_max_element_count;
}

Buffer::size_type Buffer::size() const noexcept
{
    if (m_is_full)
        return m_max_element_count;

    return WrapIndex(m_back.m_offset);
}

//  Protected   ========================================================================================================

void Buffer::AtCapacity() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(at_capacity)
    m_is_full = true;
}

Buffer::BufferIterator Buffer::DecrementBounded(Buffer::BufferIterator iterator, size_type steps) const
{
    size_type offset      = iterator.m_offset;
    size_type back_offset = m_back.m_offset;

    // If offset points at the end of buffer, the first step back will be to the last valid element in the buffer
    // Guard against steps == 0
    if ((offset == m_max_element_count) && (steps > 0))
    {
        // Count back from the end
        if (full() || empty())
        {
            // The last element is stored at the back of buffer index
            offset = RollIndexBackward(back_offset, steps - 1);
        }
        else
        {
            // The last element is one position before the back
            offset = RollIndexBackward(back_offset, steps);
        }
    }
    // Otherwise, decrement normally while accounting for wrap-around
    else
        offset = RollIndexBackward(offset, steps);

    iterator.m_offset = offset;
    return iterator;
}

Buffer::ConstBufferIterator Buffer::DecrementBounded(Buffer::ConstBufferIterator iterator, size_type steps) const
{
    size_type offset      = iterator.m_offset;
    size_type back_offset = m_back.m_offset;

    // If offset points at the end of buffer, the first step back will be to the last valid element in the buffer
    // Guard against steps == 0
    if ((offset == m_max_element_count) && (steps > 0))
    {
        // Count back from the end
        if (full() || empty())
        {
            // The last element is stored at the back of buffer index
            offset = RollIndexBackward(back_offset, steps - 1);
        }
        else
        {
            // The last element is one position before the back
            offset = RollIndexBackward(back_offset, steps);
        }
    }
    // Otherwise, decrement normally while accounting for wrap-around
    else
        offset = RollIndexBackward(offset, steps);

    iterator.m_offset = offset;
    return iterator;
}

Buffer::BufferIterator Buffer::DecrementBoundless(Buffer::BufferIterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = RollIndexBackward(offset, steps);
    iterator.m_offset    = new_offset;
    return iterator;
}

Buffer::ConstBufferIterator Buffer::DecrementBoundless(Buffer::ConstBufferIterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = RollIndexBackward(offset, steps);
    iterator.m_offset    = new_offset;
    return iterator;
}

Buffer::BufferIterator Buffer::IncrementBounded(Buffer::BufferIterator iterator, size_type steps) const
{
    size_type offset      = iterator.m_offset;
    size_type back_offset = m_back.m_offset;

    // Guard against steps == 0
    if (steps > 0)
    {
        // If index points at the end of buffer already, do not increment.
        // If it points to the back of the buffer, increment to end of buffer.
        // If the new index points to the back of the buffer but the buffer is not full, increment to end of buffer.
        // If the new index would point past the back of the buffer, increment to end of buffer.
        // Otherwise increment normally while accounting for wrap-around.
        size_type new_offset = RollIndexForward(offset, steps);
        if ((offset == m_max_element_count) ||             // Index points at the end of buffer already
            (offset == back_offset) ||                     // Index points to the back of the buffer
            (!m_is_full && (new_offset == back_offset)) || // New index points to the back of the buffer but the buffer
                                                           // is not full
            ((WrapIndex(back_offset) - WrapIndex(offset)) < steps)) // New index would point past the back of
                                                                    // the buffer
        {
            iterator.m_offset = m_max_element_count;
        }
        else
            iterator.m_offset = new_offset;
    }

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment_iterator, shmit::log::diagnostics::Print("Count:%d,From:%d,To:%d", steps, offset, iterator.m_offset))

    return iterator;
}

Buffer::ConstBufferIterator Buffer::IncrementBounded(Buffer::ConstBufferIterator iterator, size_type steps) const
{
    size_type offset      = iterator.m_offset;
    size_type back_offset = m_back.m_offset;

    // Guard against steps == 0
    if (steps > 0)
    {
        // If index points at the end of buffer already, do not increment.
        // If it points to the back of the buffer, increment to end of buffer.
        // If the new index points to the back of the buffer but the buffer is not full, increment to end of buffer.
        // If the new index would point past the back of the buffer, increment to end of buffer.
        // Otherwise increment normally while accounting for wrap-around.
        size_type new_offset = RollIndexForward(offset, steps);
        if ((offset == m_max_element_count) ||             // Index points at the end of buffer already
            (offset == back_offset) ||                     // Index points to the back of the buffer
            (!m_is_full && (new_offset == back_offset)) || // New index points to the back of the buffer but the buffer
                                                           // is not full
            ((WrapIndex(back_offset) - WrapIndex(offset)) < steps)) // New index would point past the back of
                                                                    // the buffer
        {
            offset = m_max_element_count;
        }
        else
            offset = new_offset;

        // Update iterator with offset
        iterator.m_offset = offset;
    }

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment_iterator, shmit::log::diagnostics::Print("Count:%d,From:%d,To:%d", steps, offset, iterator.m_offset))

    return iterator;
}

Buffer::BufferIterator Buffer::IncrementBoundless(Buffer::BufferIterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = (offset + steps) % m_max_element_count;
    iterator.m_offset    = new_offset;
    return iterator;
}

Buffer::ConstBufferIterator Buffer::IncrementBoundless(Buffer::ConstBufferIterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = (offset + steps) % m_max_element_count;
    iterator.m_offset    = new_offset;
    return iterator;
}

void Buffer::MoveMemoryContents(Buffer&& rhs)
{
    // Copy start address and other members from the right hand side to this
    m_front             = rhs.m_front;
    m_back              = rhs.m_back;
    m_is_full           = rhs.m_is_full;
    m_start_address     = rhs.m_start_address;
    m_max_element_count = rhs.m_max_element_count;

    // Set right hand side start address to nullptr and clear
    rhs.m_start_address = nullptr;
    rhs.clear();
}

Buffer::size_type Buffer::OverfillGuard(size_type count) const noexcept
{
    if (count > m_max_element_count)
    {
        count = m_max_element_count;

        // Log overfill
        size_type diff = count - m_max_element_count;
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(overfill, shmit::log::diagnostics::Print("Count:%d", diff))
    }

    return count;
}

void Buffer::PostInsertFront(Buffer::BufferIterator insert_start)
{
    if (insert_start <= m_back)
    {
        // Insert has filled buffer
        // Check for back overflow
        difference_type diff = m_back - insert_start;
        if (diff)
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(front_overflow, shmit::log::diagnostics::Print("Count:%d", diff))

        m_back = DecrementBoundless(insert_start, 1);
        AtCapacity();
    }

    m_front = insert_start;
}

void Buffer::PostInsertBack(Buffer::BufferIterator insert_end)
{
    if (insert_end == m_front)
    {
        // Insert has filled buffer
        m_back = DecrementBoundless(insert_end, 1);
        AtCapacity();
    }
    else if (insert_end < m_back)
    {
        // Insert has overflowed front of buffer
        difference_type diff = insert_end - m_front;
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("Count:%d", diff))

        m_front = IncrementBoundless(insert_end, 1);
        AtCapacity();
    }
    else
        m_back = insert_end;
}

void Buffer::PrepareForRandomPlacement(Buffer::BufferIterator const& start, size_type n) noexcept
{
    Buffer::BufferIterator  end_of_buffer(*this, m_max_element_count);
    difference_type distance_from_end = end_of_buffer - start;

    // Determine if the placement will fill the remainder of the buffer
    if (n >= distance_from_end)
    {
        // Placement will fill in the remainder of the buffer and, if n > num_vacancies, will truncate the front
        // The most extreme case will wrap the back of the buffer around and place the front at 'start'
        // Check for overflow and log if present
        if (n > distance_from_end)
        {
            size_type diff = n - distance_from_end;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("Count:%d", diff))
        }

        m_back = IncrementBoundless(start, n - 1); // Back of buffer is now the end of the placement
                                                   // zone
        m_front = IncrementBoundless(m_back, 1);   // Keep front of buffer ahead of the back
        AtCapacity();
    }
    else
    {
        // Else, there is room after the placement zone for some (maybe not all) pre-existing data
        // Copy as much from behind 'start_index' as possible without truncating anything ahead
        difference_type num_pre_existing = m_back - start; // This won't count the last element if the buffer is
                                                           // full, but it would be truncated anyways so who gives a
                                                           // hoot

        // Check for overflow and negate it if present
        size_type total_element_count = size() + n;
        if (total_element_count > m_max_element_count)
        {
            // Reduce recorded number of pre-existing elements to truncate those at the back that will not fit
            size_type diff = total_element_count - m_max_element_count;
            num_pre_existing -= diff;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(truncation, shmit::log::diagnostics::Print("Count:%d", diff))
        }

        // Establish destination zone for pre-exsiting data
        Buffer::BufferIterator dest_start = IncrementBoundless(start, n); // 1 past the end of placement zone
        Buffer::BufferIterator dest_end   = IncrementBoundless(dest_start, num_pre_existing);

        // Copy data in placement zone to destination zone
        uint8_t* src  = static_cast<uint8_t*>(m_start_address) + (start.m_offset * m_element_size_bytes);
        uint8_t* dest = static_cast<uint8_t*>(m_start_address) + (dest_start.m_offset * m_element_size_bytes);

        // If the end of the copy or destination zones wraps around, the copy will need to occur across two seperate
        // segments of the buffer
        // It is one or the other situation, or neither, cannot be both since the zones are adjacent
        Buffer::BufferIterator copy_end = IncrementBoundless(start, num_pre_existing);
        if (copy_end < start)
        {
            // Placement zone wraparound
            // Determine size of the first placement zone segment and copy that first
            difference_type first_segment_size = end_of_buffer - start;
            std::memmove(dest, src, first_segment_size * m_element_size_bytes);

            // Reduce, reuse, recycle :thumbsup:
            num_pre_existing -= first_segment_size;
            dest += first_segment_size * m_element_size_bytes;
            src = static_cast<uint8_t*>(m_start_address) +
                  ((copy_end.m_offset - num_pre_existing) * m_element_size_bytes);
        }
        else if (dest_end < dest_start)
        {
            // Destination zone wraparound
            // Determine size of the first destination zone segment and copy to that first
            difference_type first_segment_size = end_of_buffer - dest_start;
            std::memmove(dest, src, first_segment_size * m_element_size_bytes);

            // :recycle:
            num_pre_existing -= first_segment_size;
            dest = static_cast<uint8_t*>(m_start_address) +
                   ((dest_end.m_offset - num_pre_existing) * m_element_size_bytes);
            src += first_segment_size * m_element_size_bytes;
        }
        // Else, no zone wraparound occured. Carry on with simple copy from one to the other.

        // If wraparound did occur, this will copy the second and final segment
        std::memmove(dest, src, num_pre_existing * m_element_size_bytes);

        // Update back of buffer
        if (dest_end == m_front)
        {
            // Buffer is full
            m_back = DecrementBoundless(m_front, 1);
            AtCapacity();
        }
        else
            m_back = dest_end;
    }
}

void Buffer::Swap(Buffer& rhs) noexcept
{
    // Cache this buffer's memory contents
    bool                 temp_is_full           = m_is_full;
    Buffer::BufferIterator       temp_front             = m_front;
    Buffer::BufferIterator       temp_back              = m_back;
    size_t               temp_max_element_count = m_max_element_count;
    shmit::MemoryAddress temp_start_address     = m_start_address;

    // Copy memory contents from right hand side to here
    m_is_full           = rhs.m_is_full;
    m_front             = rhs.m_front;
    m_back              = rhs.m_back;
    m_max_element_count = rhs.m_max_element_count;
    m_start_address     = rhs.m_start_address;

    // Copy cached memory contents to right hand side
    rhs.m_is_full           = temp_is_full;
    rhs.m_front             = temp_front;
    rhs.m_back              = temp_back;
    rhs.m_max_element_count = temp_max_element_count;
    rhs.m_start_address     = temp_start_address;
}

Buffer::size_type Buffer::UnwrapPosition(size_type position) const
{
    // If the position is the end of buffer, return that
    // Else convert position to absolute index
    if (position == m_max_element_count)
        return m_max_element_count;

    return (m_front.m_offset + position) % m_max_element_count;
}

Buffer::size_type Buffer::WrapIndex(size_type index) const
{
    // If the index is the end of buffer, return that
    // Else return the index's position in reference to the front of the buffer
    if (index == m_max_element_count)
        return m_max_element_count;
    else
        return (index < m_front.m_offset) ? m_max_element_count - (m_front.m_offset - index) :
                                               index - m_front.m_offset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::BufferIterator Constructor definitions      ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::BufferIterator::BufferIterator(Buffer::BufferIterator const& rhs) noexcept : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

//  Protected   ========================================================================================================

Buffer::BufferIterator::BufferIterator(Buffer& buffer) noexcept : m_buffer {&buffer}
{
}

Buffer::BufferIterator::BufferIterator(Buffer& buffer, size_type offset) noexcept : m_offset {offset}, m_buffer {&buffer}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::BufferIterator methods in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::BufferIterator Buffer::BufferIterator::operator++() noexcept
{
    size_type old_offset = m_offset;
    *this = m_buffer->IncrementBounded(*this, 1);

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment, shmit::log::diagnostics::Print("Count:1,From:%d,To:%d", old_offset, m_offset))

    return *this;
}

Buffer::BufferIterator Buffer::BufferIterator::operator++(int) noexcept
{
    Buffer::BufferIterator tmp = *this;
    *this = m_buffer->IncrementBounded(*this, 1);

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment, shmit::log::diagnostics::Print("Count:1,From:%d,To:%d", m_offset, tmp.m_offset))

    return tmp;
}

Buffer::BufferIterator Buffer::BufferIterator::operator--() noexcept
{
    *this = m_buffer->DecrementBounded(*this, 1);
    return *this;
}

Buffer::BufferIterator Buffer::BufferIterator::operator--(int) noexcept
{
    Buffer::BufferIterator tmp = *this;
    *this = m_buffer->DecrementBounded(*this, 1);
    return tmp;
}

Buffer::BufferIterator Buffer::BufferIterator::operator+=(difference_type steps) noexcept
{
    size_type old_offset = m_offset;
    *this = m_buffer->IncrementBounded(*this, steps);

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment, shmit::log::diagnostics::Print("Count:%d,From:%d,To:%d", steps, old_offset, m_offset))

    return *this;
}

Buffer::BufferIterator Buffer::BufferIterator::operator-=(difference_type steps) noexcept
{
    *this = m_buffer->DecrementBounded(*this, steps);
    return *this;
}

Buffer::BufferIterator Buffer::BufferIterator::operator=(Buffer::BufferIterator const& rhs) noexcept
{
    m_offset = rhs.m_offset;
    m_buffer = rhs.m_buffer;
    return *this;
}

bool Buffer::BufferIterator::operator>(Buffer::BufferIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) > rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::BufferIterator::operator>=(Buffer::BufferIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) >= rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::BufferIterator::operator<(Buffer::BufferIterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

bool Buffer::BufferIterator::operator<=(Buffer::BufferIterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

bool Buffer::BufferIterator::operator==(Buffer::BufferIterator const& rhs) const noexcept
{
    return (m_buffer->m_start_address == rhs.m_buffer->m_start_address) &&
           (m_buffer->WrapIndex(m_offset) == rhs.m_buffer->WrapIndex(rhs.m_offset));
}

bool Buffer::BufferIterator::operator!=(Buffer::BufferIterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::ConstBufferIterator Constructor definitions         ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::ConstBufferIterator::ConstBufferIterator(Buffer::BufferIterator const& rhs) noexcept
    : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

Buffer::ConstBufferIterator::ConstBufferIterator(Buffer::ConstBufferIterator const& rhs) noexcept
    : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

//  Protected   ========================================================================================================

Buffer::ConstBufferIterator::ConstBufferIterator(Buffer const& buffer) noexcept : m_buffer {&buffer}
{
}

Buffer::ConstBufferIterator::ConstBufferIterator(Buffer const& buffer, size_type offset) noexcept
    : m_offset {offset}, m_buffer {&buffer}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::ConstBufferIterator methods in alphabetical order       ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator++() noexcept
{
    *this = m_buffer->IncrementBounded(*this, 1);
    return *this;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator++(int) noexcept
{
    Buffer::ConstBufferIterator tmp = *this;
    *this = m_buffer->IncrementBounded(*this, 1);
    return tmp;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator--() noexcept
{
    *this = m_buffer->DecrementBounded(*this, 1);
    return *this;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator--(int) noexcept
{
    Buffer::ConstBufferIterator tmp = *this;
    *this = m_buffer->DecrementBounded(*this, 1);
    return tmp;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator+=(difference_type steps) noexcept
{
    *this = m_buffer->IncrementBounded(*this, steps);
    return *this;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator-=(difference_type steps) noexcept
{
    *this = m_buffer->DecrementBounded(*this, steps);
    return *this;
}

Buffer::ConstBufferIterator Buffer::ConstBufferIterator::operator=(Buffer::ConstBufferIterator const& rhs) noexcept
{
    m_offset = rhs.m_offset;
    m_buffer = rhs.m_buffer;
    return *this;
}

bool Buffer::ConstBufferIterator::operator>(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) > rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::ConstBufferIterator::operator>=(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) >= rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::ConstBufferIterator::operator<(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

bool Buffer::ConstBufferIterator::operator<=(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

bool Buffer::ConstBufferIterator::operator==(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return (m_buffer->m_start_address == rhs.m_buffer->m_start_address) &&
           (m_buffer->WrapIndex(m_offset) == rhs.m_buffer->WrapIndex(rhs.m_offset));
}

bool Buffer::ConstBufferIterator::operator!=(Buffer::ConstBufferIterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  External operator overloads         ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Buffer::BufferIterator operator+(Buffer::BufferIterator const& lhs, Buffer::difference_type rhs) noexcept
{
    Buffer::BufferIterator tmp = lhs;
    tmp += rhs;
    return tmp;
}

Buffer::BufferIterator operator+(const Buffer::difference_type lhs, Buffer::BufferIterator const& rhs) noexcept
{
    Buffer::BufferIterator tmp = rhs;
    tmp += lhs;
    return tmp;
}

Buffer::difference_type operator-(Buffer::BufferIterator const& lhs, Buffer::BufferIterator const& rhs) noexcept
{
    if (lhs < rhs)
        return 0;

    Buffer::difference_type diff = lhs.m_buffer->WrapIndex(lhs.m_offset) - rhs.m_buffer->WrapIndex(rhs.m_offset);
    return diff;
}

Buffer::BufferIterator operator-(Buffer::BufferIterator const& lhs, const Buffer::difference_type rhs) noexcept
{
    Buffer::BufferIterator tmp = lhs;
    tmp -= rhs;
    return tmp;
}

Buffer::ConstBufferIterator operator+(Buffer::ConstBufferIterator const& lhs, Buffer::difference_type rhs) noexcept
{
    Buffer::ConstBufferIterator tmp = lhs;
    tmp += rhs;
    return tmp;
}

Buffer::ConstBufferIterator operator+(const Buffer::difference_type lhs, Buffer::ConstBufferIterator const& rhs) noexcept
{
    Buffer::ConstBufferIterator tmp = rhs;
    tmp += lhs;
    return tmp;
}

Buffer::difference_type operator-(Buffer::ConstBufferIterator const& lhs, Buffer::ConstBufferIterator const& rhs) noexcept
{
    if (lhs < rhs)
        return 0;

    Buffer::difference_type diff = lhs.m_buffer->WrapIndex(lhs.m_offset) - rhs.m_buffer->WrapIndex(rhs.m_offset);
    return diff;
}

Buffer::ConstBufferIterator operator-(Buffer::ConstBufferIterator const& lhs, const Buffer::difference_type rhs) noexcept
{
    Buffer::ConstBufferIterator tmp = lhs;
    tmp -= rhs;
    return tmp;
}

} // namespace detail
} // namespace shmit