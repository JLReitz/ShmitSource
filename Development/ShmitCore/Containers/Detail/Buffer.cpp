#include <ShmitCore/Containers/Detail/Buffer.hpp>

#include <cstring>

namespace shmit
{
namespace detail
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer constructor definitions      ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Protected   ========================================================================================================

// clang-format off
Buffer::Buffer(Buffer::size_type element_size_bytes) noexcept : m_element_size_bytes {element_size_bytes}
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
}

Buffer::Buffer(char const* name, Buffer::size_type element_size_bytes) noexcept
    : Named(name), m_element_size_bytes {element_size_bytes}
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(constructing)
}
// clang-format on

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer method definitions in alphabetical order         ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

void Buffer::clear() noexcept
{
    m_is_full = false;
    m_back = m_front = Buffer::Iterator(*this);
}

bool Buffer::empty() const noexcept
{
    // Can't perform Buffer::Iterator::operator==() overload here because it calls this function
    // Manually check front and back offsets
    return m_front.m_offset == m_back.m_offset;
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

Buffer::size_type Buffer::WrapIndex(size_type index) const
{
    // If the index is the end of buffer, return that
    // Else return the index's position in reference to the front of the buffer
    if (index == m_max_element_count)
        return m_max_element_count;
    else
        return (index < m_front.m_offset) ? m_max_element_count - (m_front.m_offset - index) : index - m_front.m_offset;
}

//  Protected   ========================================================================================================

void Buffer::AtCapacity() noexcept
{
    LOG_MEMBER_DIAGNOSTIC_POSIT(at_capacity)
    m_is_full = true;
}

void Buffer::ClearFullState() noexcept
{
}

void Buffer::DecrementBounded(Buffer::Iterator& iterator, size_type steps) const
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
}

void Buffer::DecrementBounded(Buffer::ConstIterator& iterator, size_type steps) const
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
}

Buffer::Iterator Buffer::DecrementBoundless(Buffer::Iterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = RollIndexBackward(offset, steps);
    iterator.m_offset    = new_offset;
    return iterator;
}

Buffer::ConstIterator Buffer::DecrementBoundless(Buffer::ConstIterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = RollIndexBackward(offset, steps);
    iterator.m_offset    = new_offset;
    return iterator;
}

void Buffer::IncrementBounded(Buffer::Iterator& iterator, size_type steps) const
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
}

void Buffer::IncrementBounded(Buffer::ConstIterator& iterator, size_type steps) const
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
}

Buffer::Iterator Buffer::IncrementBoundless(Buffer::Iterator iterator, size_type steps) const
{
    size_type offset     = iterator.m_offset;
    size_type new_offset = (offset + steps) % m_max_element_count;
    iterator.m_offset    = new_offset;
    return iterator;
}

Buffer::ConstIterator Buffer::IncrementBoundless(Buffer::ConstIterator iterator, size_type steps) const
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
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(overfill, shmit::log::diagnostics::Print("Count:%u", diff))
    }

    return count;
}

void Buffer::PostInsertFront(Buffer::Iterator insert_start)
{
    if (insert_start <= m_back)
    {
        // Insert has filled buffer
        // Check for back overflow
        size_type back_wrapped         = WrapIndex(m_back.m_offset);
        size_type insert_start_wrapped = WrapIndex(insert_start.m_offset);
        size_type diff                 = back_wrapped - insert_start_wrapped;
        if (diff)
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(front_overflow, shmit::log::diagnostics::Print("Count:%u", diff))

        m_back = DecrementBoundless(insert_start, 1);
        AtCapacity();
    }

    m_front = insert_start;
}

void Buffer::PostInsertBack(Buffer::Iterator insert_end)
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
        size_type front_wrapped      = WrapIndex(m_front.m_offset);
        size_type insert_end_wrapped = WrapIndex(insert_end.m_offset);
        size_type diff               = insert_end_wrapped - front_wrapped;
        LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("Count:%u", diff))

        m_front = IncrementBoundless(insert_end, 1);
        AtCapacity();
    }
    else
        m_back = insert_end;
}

void Buffer::PrepareForRandomPlacement(Buffer::Iterator const& start, size_type n) noexcept
{
    size_type start_wrapped     = WrapIndex(start.m_offset);
    size_type distance_from_end = m_max_element_count - start_wrapped;

    // Determine if the placement will fill the remainder of the buffer
    if (n >= distance_from_end)
    {
        // Placement will fill in the remainder of the buffer and, if n > distance_from_end, will truncate the front
        // The most extreme case will wrap the back of the buffer around and place the new front at 'start'
        // Check for overflow and log if present
        if (n > distance_from_end)
        {
            size_type diff = n - distance_from_end;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("Count:%u", diff))
        }

        m_back = IncrementBoundless(start, n - 1); // Back of buffer is now the end of the placement
                                                   // zone
        m_front = IncrementBoundless(m_back, 1);   // Keep front of buffer ahead of the back
        AtCapacity();
    }
    else
    {
        // Else, there is room after the placement zone for some (maybe not all) pre-existing data
        // Shift as much from behind 'start' as possible without truncating anything ahead
        size_type num_pre_existing = size() - start_wrapped;

        // Check for overflow and negate it if present
        size_type total_element_count = size() + n;
        if (total_element_count > m_max_element_count)
        {
            // Reduce recorded number of pre-existing elements to truncate those at the back that will not fit
            size_type diff = total_element_count - m_max_element_count;
            num_pre_existing -= diff;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(truncation, shmit::log::diagnostics::Print("Count:%u", diff))
        }

        // Establish start of destination zone for content shift
        Iterator dest_start = IncrementBoundless(start, n); // 1 past the end of placement zone
        Iterator dest_end   = ShiftContents(start, dest_start, num_pre_existing);

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

Buffer::Iterator Buffer::ShiftContents(Buffer::Iterator const& src, Buffer::Iterator const& dest, Buffer::size_type n)
{
    uint8_t* src_ptr  = static_cast<uint8_t*>(m_start_address) + (src.m_offset * m_element_size_bytes);
    uint8_t* dest_ptr = static_cast<uint8_t*>(m_start_address) + (dest.m_offset * m_element_size_bytes);

    // If the end of the source or destination zones wraps around, the copy will need to occur across two seperate
    // segments of the buffer
    // It is one or the other situation, or neither, cannot be both since the zones start adjacent to each other
    Iterator src_end  = IncrementBoundless(src, n);
    Iterator dest_end = IncrementBoundless(dest, n);
    if (src_end < src)
    {
        // Source zone wraparound
        // Determine size of the source zone segment before the wrap and copy that first
        size_type src_wrapped           = WrapIndex(src.m_offset);
        size_type src_distance_from_end = m_max_element_count - src_wrapped;
        std::memmove(dest_ptr, src_ptr, src_distance_from_end * m_element_size_bytes);

        // Reduce, reuse, recycle :thumbsup:
        n -= src_distance_from_end;
        dest_ptr += src_distance_from_end * m_element_size_bytes;
        src_ptr = static_cast<uint8_t*>(m_start_address) + ((src_end.m_offset - n) * m_element_size_bytes);
    }
    else if (dest_end < dest)
    {
        // Destination zone wraparound
        // Determine size of the destination zone segment before the wrap and copy to that first
        size_type dest_wrapped           = WrapIndex(dest.m_offset);
        size_type dest_distance_from_end = m_max_element_count - dest_wrapped;
        std::memmove(dest_ptr, src_ptr, dest_distance_from_end * m_element_size_bytes);

        // :recycle:
        n -= dest_distance_from_end;
        dest_ptr = static_cast<uint8_t*>(m_start_address) + ((dest_end.m_offset - n) * m_element_size_bytes);
        src_ptr += dest_distance_from_end * m_element_size_bytes;
    }
    // Else, no zone wraparound occured. Carry on with simple copy from one to the other.

    // If wraparound did occur, this will copy the second and final segment
    std::memmove(dest_ptr, src_ptr, n * m_element_size_bytes);

    // Return the end of the destination zone
    return dest_end;
}

void Buffer::Swap(Buffer& rhs) noexcept
{
    // Cache this buffer's memory contents
    bool                 temp_is_full           = m_is_full;
    Iterator             temp_front             = m_front;
    Iterator             temp_back              = m_back;
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::Iterator constructor definitions            ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::Iterator::Iterator(Buffer::Iterator const& rhs) noexcept : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

//  Protected   ========================================================================================================

Buffer::Iterator::Iterator(Buffer& buffer) noexcept : m_buffer {&buffer}
{
}

Buffer::Iterator::Iterator(Buffer& buffer, size_type offset) noexcept : m_offset {offset}, m_buffer {&buffer}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::Iterator method definitions in alphabetical order           ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

bool Buffer::Iterator::operator>(Buffer::Iterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) > rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::Iterator::operator>=(Buffer::Iterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) >= rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::Iterator::operator<(Buffer::Iterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

bool Buffer::Iterator::operator<=(Buffer::Iterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

bool Buffer::Iterator::operator==(Buffer::Iterator const& rhs) const noexcept
{
    // If buffer references do not match, iterators are not equivalent
    if (m_buffer == rhs.m_buffer)
    {
        // Check wrapped iterator positions
        if (m_buffer->WrapIndex(m_offset) == rhs.m_buffer->WrapIndex(rhs.m_offset))
            return true;
    }

    // Else, iterators are not equal, return false
    return false;
}

bool Buffer::Iterator::operator!=(Buffer::Iterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

//  Protected   ========================================================================================================

void Buffer::Iterator::Copy(Buffer::Iterator const& rhs)
{
    m_buffer = rhs.m_buffer;
    m_offset = rhs.m_offset;
}

void Buffer::Iterator::Increment(Buffer::size_type steps)
{
    size_type old_offset = m_offset;
    m_buffer->IncrementBounded(*this, steps);

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment, shmit::log::diagnostics::Print("Count:%u,From:%u,To:%u", steps,
                                                                                      old_offset, m_offset))
}

void Buffer::Iterator::Decrement(Buffer::size_type steps)
{
    size_type old_offset = m_offset;
    m_buffer->DecrementBounded(*this, steps);

    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(decrement, shmit::log::diagnostics::Print("Count:%u,From:%u,To:%u", steps,
                                                                                      old_offset, m_offset))
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::ConstIterator constructor definitions           ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

Buffer::ConstIterator::ConstIterator(Buffer::Iterator const& rhs) noexcept
    : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

Buffer::ConstIterator::ConstIterator(Buffer::ConstIterator const& rhs) noexcept
    : m_offset {rhs.m_offset}, m_buffer {rhs.m_buffer}
{
}

//  Protected   ========================================================================================================

Buffer::ConstIterator::ConstIterator(Buffer const& buffer) noexcept : m_buffer {&buffer}
{
}

Buffer::ConstIterator::ConstIterator(Buffer const& buffer, size_type offset) noexcept
    : m_offset {offset}, m_buffer {&buffer}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Buffer::ConstIterator method definitions in alphabetical order          ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

bool Buffer::ConstIterator::operator>(Buffer::ConstIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) > rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::ConstIterator::operator>=(Buffer::ConstIterator const& rhs) const noexcept
{
    return m_buffer->WrapIndex(m_offset) >= rhs.m_buffer->WrapIndex(rhs.m_offset);
}

bool Buffer::ConstIterator::operator<(Buffer::ConstIterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

bool Buffer::ConstIterator::operator<=(Buffer::ConstIterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

bool Buffer::ConstIterator::operator==(Buffer::ConstIterator const& rhs) const noexcept
{
    return (m_buffer->m_start_address == rhs.m_buffer->m_start_address) &&
           (m_buffer->WrapIndex(m_offset) == rhs.m_buffer->WrapIndex(rhs.m_offset));
}

bool Buffer::ConstIterator::operator!=(Buffer::ConstIterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

//  Protected   ========================================================================================================

void Buffer::ConstIterator::Copy(Buffer::Iterator const& rhs)
{
    m_buffer = rhs.m_buffer;
    m_offset = rhs.m_offset;
}

void Buffer::ConstIterator::Increment(Buffer::size_type steps)
{
    size_type old_offset = m_offset;
    m_buffer->IncrementBounded(*this, steps);
    LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(increment, shmit::log::diagnostics::Print("Count:%u,From:%d,To:%d", steps,
                                                                                      old_offset, m_offset))
}

void Buffer::ConstIterator::Decrement(Buffer::size_type steps)
{
    m_buffer->DecrementBounded(*this, steps);
}

} // namespace detail
} // namespace shmit