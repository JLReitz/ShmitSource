#include <ShmitCore/Types/Containers/Detail/Buffer.hpp>

#include <cstring>

namespace shmit
{
namespace detail
{

//  Public      ========================================================================================================

void Buffer::clear() noexcept
{
    m_is_full    = false;
    m_back_index = m_front_index = 0;
}

bool Buffer::empty() const noexcept
{
    return (size() == 0);
}

bool Buffer::full() const noexcept
{
    return m_is_full;
}

size_t Buffer::max_size() const noexcept
{
    return m_max_element_count;
}

size_t Buffer::size() const noexcept
{
    if (m_is_full)
        return m_max_element_count;

    return WrapIndex(m_back_index);
}

//  Protected   ========================================================================================================

size_t Buffer::DecrementIteratorBounded(BufferIterator& iterator, size_t steps) const
{
    size_t offset = iterator.m_offset;

    // Guard against steps == 0
    if (steps > 0)
    {
        // If index points at the end of buffer already, do not increment.
        // If it points to the back of the buffer, increment to end of buffer.
        // If the new index points to the back of the buffer but the buffer is not full, increment to end of buffer.
        // If the new index would point past the back of the buffer, increment to end of buffer.
        // Otherwise increment normally while accounting for wrap-around.
        size_t new_offset = RollIndexForward(offset, steps);
        if ((offset == m_max_element_count) ||              // Index points at the end of buffer already
            (offset == m_back_index) ||                     // Index points to the back of the buffer
            (!m_is_full && (new_offset == m_back_index)) || // New index points to the back of the buffer but the buffer
                                                            // is not full
            ((WrapIndex(m_back_index) - WrapIndex(offset)) < steps)) // New index would point past the back of
                                                                     // the buffer
        {
            offset = m_max_element_count;
        }
        else
            offset = new_offset;

        // Update iterator with offset
        iterator.m_offset = offset;
    }

    return offset;
}

size_t Buffer::DecrementIteratorBoundless(BufferIterator& iterator, size_t steps) const
{
    size_t new_offset = (iterator.m_offset < steps) ? m_max_element_count - (steps - iterator.m_offset) :
                                                      iterator.m_offset - steps;
    iterator.m_offset = new_offset;
    return new_offset;
}

size_t Buffer::IncrementIteratorBounded(BufferIterator& iterator, size_t steps) const
{
    size_t offset = iterator.m_offset;

    // If offset points at the end of buffer, the first step back will be to the last valid element in the buffer
    if ((offset == m_max_element_count) && (steps > 0))
    {
        // Count back from the end
        if (m_is_full)
        {
            // Tthe last element is stored at the back of buffer
            return RollIndexBackward(m_back_index, steps - 1);
        }
        else
        {
            // The last element is one position before the back
            return RollIndexBackward(m_back_index, steps);
        }
    }

    // Otherwise, decrement normally while accounting for wrap-around
    return _M_roll_over_backward(offset, steps);
}

size_t Buffer::IncrementIteratorBoundless(BufferIterator& iterator, size_t steps) const
{
    size_t new_offset = (iterator.m_offset + steps) % m_max_element_count;
    iterator.m_offset = new_offset;
    return new_offset;
}

size_t Buffer::PrepareForRandomPlacement(size_t start_index, size_t n) noexcept
{
    size_t distance_from_end = m_max_element_count - WrapIndex(start_index);

    // Determine if the placement will fill the remainder of the buffer
    if (n >= distance_from_end)
    {
        // Placement will fill in the remainder of the buffer and, if n > num_vacancies, will truncate the front
        // The most extreme case will wrap the back of the buffer around and place the front at 'start_index'
        // Check for overflow and log if present
        if (n > distance_from_end)
        {
            size_t diff = n - distance_from_end;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(back_overflow, shmit::log::diagnostics::Print("Count:%d", diff))
        }

        m_back_index  = RollIndexForward(start_index, n - 1); // Back of buffer is now the end of the placement zone
        m_front_index = RollIndexForward(m_back_index, 1);    // Keep front of buffer ahead of the back
        m_is_full     = true;                                 // buffer is at capacity
        LOG_MEMBER_DIAGNOSTIC_POSIT(at_capacity)
    }
    else
    {
        // Else, there is room after the placement zone for some (maybe not all) pre-existing data
        // Copy as much from behind 'start_index' as possible without truncating anything ahead
        size_t num_pre_existing = m_back_index - start_index; // This won't count the last element if the buffer is
                                                              // full, but it would be truncated anyways

        // Check for overflow and negate it if present
        size_t total_element_count = size() + n;
        if (total_element_count > m_max_element_count)
        {
            // Reduce recorded number of pre-existing elements to truncate those at the back that will not fit
            size_t diff = total_element_count - m_max_element_count;
            num_pre_existing -= diff;
            LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(truncation, shmit::log::diagnostics::Print("Count:%d", diff))
        }

        // Establish destination zone for pre-exsiting data
        size_t dest_start_index = RollIndexForward(start_index, n); // 1 past the end of placement zone
        size_t dest_end_index   = RollIndexForward(dest_start_index, num_pre_existing);

        // Copy data in placement zone to destination zone
        shmit::MemoryAddress src  = m_start_address + (start_index * m_element_size);
        shmit::MemoryAddress dest = m_start_address + (dest_start_index * m_element_size);

        // If the end of the copy or destination zones wraps around, the copy will need to occur across two seperate
        // segments of the buffer
        // It is one or the other situation, or neither, cannot be both since the zones occupy seperate spaces
        size_t copy_end_index = RollIndexForward(start_index, num_pre_existing);
        if (copy_end_index < start_index)
        {
            // Placement zone wraparound
            // Determine size of the first placement zone segment and copy that first
            size_t first_segment_size = m_max_element_count - start_index;
            std::memcpy(dest, src, first_segment_size);

            // Reduce, reuse, recycle :thumbsup:
            num_pre_existing -= first_segment_size;
            dest += first_segment_size * m_element_size;
            src = m_start_address + ((copy_end_index - num_pre_existing) * m_element_size);
        }
        else if (dest_end_index < dest_start_index)
        {
            // Destination zone wraparound
            // Determine size of the first destination zone segment and copy to that first
            size_t first_segment_size = m_max_element_count - dest_start_index;
            std::memcpy(dest, src, first_segment_size);

            // :recycle:
            num_pre_existing -= first_segment_size;
            dest = m_start_address + ((dest_end_index - num_pre_existing) * m_element_size);
            src += first_segment_size * m_element_size;
        }
        // Else, no zone wraparound occured. Carry on with simple copy from one to the other.

        // If wraparound did occur, this will copy the second and final segment
        std::memcpy(dest, src, num_pre_existing);

        // Update back of buffer
        if (dest_end_index == m_front_index)
        {
            // Buffer is full
            m_back_index = RollIndexBackward(m_front_index, 1);
            m_is_full    = true;
            LOG_MEMBER_DIAGNOSTIC_POSIT(at_capacity)
        }
        else
            m_back_index = dest_end_index;
    }
}

size_t Buffer::UnwrapPosition(size_t position) const
{
    // If the position is the end of buffer, return that
    // Else convert position to absolute index
    if (position == m_max_element_count)
        return m_max_element_count;

    return (m_front_index + position) % m_max_element_count;
}

size_t Buffer::WrapIndex(size_t index) const
{
    // If the index is the end of buffer, return that
    // Else return the index's position in reference to the front of the buffer
    if (index == m_max_element_count)
        return m_max_element_count;
    else
        return (index < m_max_element_count) ? m_max_element_count - (m_front_index - index) : index - m_front_index;
}

} // namespace detail
} // namespace shmit