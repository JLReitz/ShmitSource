#include <ShmitCore/Logging/VoidLogger.hpp>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  VoidLogger method definitions in alphabetical order     ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

void VoidLogger::Publish(char const* type, char const* level, char const* context, char const* id, char const* msg)
{
    // Do nothing
    (void)type;    // Unused
    (void)level;   // Unused
    (void)context; // Unused
    (void)id;      // Unused
    (void)msg;     // Unused
}

} // namespace log
} // namespace shmit
