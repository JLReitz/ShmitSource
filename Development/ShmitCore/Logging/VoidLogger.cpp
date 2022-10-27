#include <ShmitCore/Logging/VoidLogger.hpp>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VoidLogger method definitions in alphabetical order     ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

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
