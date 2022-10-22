#include <ShmitCore/Logging/Loggers/VoidLogger.hpp>

namespace shmit
{
namespace log
{

void VoidLogger::PublishEntry(const char* type, const char* level, const char* id, const char* context, const char* msg)
{
    // Do nothing
    (void)type;    // Unused
    (void)level;   // Unused
    (void)id;      // Unused
    (void)context; // Unused
    (void)msg;     // Unused
}

} // namespace log
} // namespace shmit
