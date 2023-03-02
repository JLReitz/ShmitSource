#include <ShmitCore/Logging/Events/StaticLogging.hpp>

#ifdef BARE_METAL_SHMIT

extern Logger& gDefaultEventLogger; /*! The default event logger for bare metal systems is declared in the target
                                       platform's library */

#else

#include <ShmitCore/Logging/StdoutLogger.hpp>

shmit::log::StdoutLogger gDefaultEventLogger; /*! The default event logger for OS hosted systems */

#endif

namespace shmit
{
namespace log
{
namespace events
{

Level   Logging::m_threshold = Level::eTrace;
Logger& Logging::m_logger    = gDefaultEventLogger;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

void Logging::LoadLogger(Logger& logger) // Static method
{
    // Update logger reference and open its threshold to accept all
    // The static interface will now filter for unnecessary messages
    m_logger = logger;
    m_logger.SetThreshold(Level::eTrace);
}

void Logging::SetThreshold(Level threshold) // Static method
{
    m_threshold = threshold;
}

} // namespace events
} // namespace log
} // namespace shmit