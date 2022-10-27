#include <ShmitCore/Logging/StdoutLogger.hpp>

#include <iostream>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StdoutLogger method definitions in alphabetical order   ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

StdoutLogger::StdoutLogger(Level filter) : OStreamLogger(::std::cout, filter)
{
}

void StdoutLogger::Load(::std::ostream& oStream)
{
    // Do nothing
    // Member pointer to std::cout shall not get reassigned, this method override exists to ensure that does not happen
}

} // namespace log
} // namespace shmit
