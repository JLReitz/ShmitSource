#include <ShmitCore/Logging/Loggers/VoidLogger.hpp>

namespace shmit
{
namespace log
{

void VoidLogger::LogEntry(Type type, Level level, const char* id, const char* msg)
{
    // Do nothing
}

} // namespace log
} // namespace shmit
