#include <ShmitCore/Logging/Diagnostics/Logging.hpp>

#ifdef BARE_METAL_SHMIT

extern Logger& gDefaultDiagnosticsLogger; /*! The default diagnostics logger for bare metal systems is declared in the
                                          target platform's library*/

#else

#include <ShmitCore/Logging/StdoutLogger.hpp>

shmit::log::StdoutLogger gDefaultDiagnosticsLogger; /*! The default diagnostics logger for OS hosted systems */

#endif

namespace shmit
{
namespace log
{
namespace diagnostics
{

Level   Logging::m_threshold = Level::eTrace;
Logger& Logging::m_logger    = gDefaultDiagnosticsLogger;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public ============================================================================================================

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

} // namespace diagnostics
} // namespace log
} // namespace shmit