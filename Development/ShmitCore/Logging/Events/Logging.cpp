#include <ShmitCore/Logging/Events/Logging.hpp>

namespace shmit
{
namespace log
{
namespace events
{

#ifdef BARE_METAL_SHMIT

extern Logger& gDefaultEventLogger; /*! The default event logger for bare metal systems is declared in the target
                                       platform's library */

#else

#include <ShmitCore/Logging/StdoutLogger.hpp>

shmit::log::StdoutLogger gDefaultEventLogger; /*! The default event logger for OS hosted systems */

#endif

detail::LoggingInterface<EventsType> interface(&gDefaultEventLogger);

} // namespace events
} // namespace log
} // namespace shmit