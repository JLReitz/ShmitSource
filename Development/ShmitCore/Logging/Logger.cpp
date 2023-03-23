#include <ShmitCore/Help/String.hpp>
#include <ShmitCore/Logging/Logger.hpp>
#include <ShmitCore/Platform/Time.hpp>

namespace shmit
{
namespace log
{

constexpr size_t kTimeStampMaxLength {shmit::max_digits_for_type<typename time::Milliseconds::Rep>::value};
// constexpr String kEndLogLine {"\n\r"};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Logger method definitions in alphabetical order ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public ============================================================================================================

Logger::Logger(Level threshold) : m_threshold(threshold)
{
}

void Logger::Post(Type type, Level level, String const& context, String const& id, String const& msg)
{
    char               timestamp_c_str[kTimeStampMaxLength] {0};
    time::Microseconds runtime_us       = platform::Clock::now().DurationSinceEpoch<time::Microsecond>();
    size_t             timestamp_length = shmit::print(timestamp_c_str, kTimeStampMaxLength, runtime_us.Count());
    String             log_str {timestamp_c_str, timestamp_length};

    String type_str {TypeToString(type)};
    String level_str {LevelToString(level)};
    String context_str {context};
    String id_str {id};
    String msg_str {msg};

    log_str.Append(type_str);
    log_str.Append(level_str);
    log_str.Append(context_str);
    log_str.Append(id_str);
    log_str.Append(msg_str);

    if (level >= m_threshold)
    {
        Publish(log_str);
    }
}

void Logger::SetThreshold(Level threshold)
{
    m_threshold = threshold;
}

Level Logger::Threshold() const
{
    return m_threshold;
}

} // namespace log
} // namespace shmit