#pragma once

#include <ShmitCore/Logging/Loggers/LoggerInterface.hpp>

#include <cstdio>
#include <sstream>

namespace shmit
{
namespace log
{
namespace data
{

class StaticLogging
{
public:
    template<typename... ARGV>
    static void Log(Level level, char const* id, char const* context, ARGV... args);

    static void LoadLogger(Logger& logger);

private:
    static void LogEntry(Level level, char const* id, char const* context, char const* data_str);

    template<typename T, typename... ARGV>
    static void StartPrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest);

    template<typename T, typename... ARGV>
    static void PrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest);

    static void PrintArgs(std::ostream& ostream); // Base case

    static Logger* m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename... ARGV>
void StaticLogging::Log(Level level, char const* id, char const* context, ARGV... args)
{
    std::stringstream data_str;

    // TODO cache timestamp

    StartPrintArgs(data_str, args...);

    LogEntry(level, id, context, data_str.str().c_str());
}

void StaticLogging::PrintArgs(std::ostream& ostream)
{
    // Do nothing
    (void)ostream; // Unused
    return;
}

template<typename T, typename... ARGV>
void StaticLogging::PrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest)
{
    ostream << ',' << first;
    PrintArgs(ostream, rest...);
}

template<typename T, typename... ARGV>
void StaticLogging::StartPrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest)
{
    ostream << first;
    PrintArgs(ostream, rest...);
}

} // namespace data
} // namespace log
} // namespace shmit
