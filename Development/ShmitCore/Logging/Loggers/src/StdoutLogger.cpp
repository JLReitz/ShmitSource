#include <ShmitCore/Logging/Loggers/StdoutLogger.hpp>

#include <iostream>

namespace shmit
{
namespace log
{

StdoutLogger::StdoutLogger() : OStreamLogger(::std::cout)
{
}

void StdoutLogger::Load(::std::ostream& oStream)
{
    // Do nothing
    // Member pointer to std::cout shall not get reassigned, this method override exists to ensure that does not happen
}

} // namespace log
} // namespace shmit
