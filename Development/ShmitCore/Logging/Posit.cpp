#include <ShmitCore/Logging/Posit.hpp>
#include <ShmitCore/Platform/Time.hpp>

namespace shmit
{
namespace log
{

Posit::Posit(StringConstantId type_id, Level severity, std::basic_string_view<Payload::char_type> payload_buffer)
    : m_payload_buffer {payload_buffer}
{
    // Header starts behind the prefix string
    Header* header {reinterpret_cast<Header*>(m_payload_buffer.data() + Payload::kPrefixStr.size())};

    // Populate timestamp
    time::Microseconds now_us {platform::Clock::Now().DurationSinceEpoch(time::Microsecond())};
    header->timestamp = now_us.Count();

    // Populate payload size
    // Cap the encoded value at the maximum, it is okay that the original buffer remains its original size
    size_t payload_size {m_payload_buffer.size()};
    if (payload_size > Payload::kMaxSize)
        payload_size = Payload::kMaxSize;
    header->packet_size = payload_size;

    // Populate type ID and severity
    header->type_id  = type_id;
    header->severity = severity;
}

} // namespace log
} // namespace shmit