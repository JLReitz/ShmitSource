#include <ShmitCore/Help/String.hpp>
#include <ShmitCore/Logging/Logger.hpp>
#include <ShmitCore/Platform/Time.hpp>

namespace shmit
{
namespace log
{

constexpr size_t kTimeStampMaxLength {shmit::max_digits_for_type<typename time::Milliseconds::Rep>::value};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Logger method definitions in alphabetical order ///////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public ============================================================================================================

Logger::Logger(Level threshold) : m_threshold(threshold)
{
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