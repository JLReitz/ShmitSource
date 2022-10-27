#pragma once

#include <ShmitCore/Logging/Logger.hpp>

#include <cstdio>
#include <sstream>

namespace shmit
{
namespace log
{
namespace data
{

/// @brief Static data logging interface
class StaticLogging
{
public:
    /// @brief Public entry point for logging data to the system
    /// @tparam ...ARGV Data arguments type pack
    /// @param level Data log level
    /// @param id Data ID
    /// @param context Data context name
    /// @param ...args Data arguments
    template<typename... ARGV>
    static void Log(Level level, char const* id, char const* context, ARGV... args);

    /// @brief Loads a logger instance in to the static interface. This replaces the previously loaded Logger.
    /// @param logger Logger instance
    static void LoadLogger(Logger& logger);

    /// @brief Configures the level threshold for the static interface
    /// @param level Log entries below this level will not be published
    static void SetThreshold(Level level);

private:
    /// @brief Forwards compiled entry fields to the currently loaded Logger
    /// @param level Data log level
    /// @param id Data ID string
    /// @param context Data context name
    /// @param data_str Data string
    static void LogEntry(Level level, char const* id, char const* context, char const* data_str);

    /// @brief Starts printing diagnostic data arguments to an output stream
    /// @tparam T First argument type
    /// @tparam ...ARGV Data arguments type pack for the rest
    /// @param ostream Output stream
    /// @param first First data argument
    /// @param ...rest The rest of the data arguments
    template<typename T, typename... ARGV>
    static void StartPrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest);

    /// @brief Recursive printing of diagnostic data arguments to an output stream
    /// @tparam T First argument type
    /// @tparam ...ARGV Data arguments type pack for the rest
    /// @param ostream Output stream
    /// @param first First data argument
    /// @param ...rest The rest of the data arguments
    template<typename T, typename... ARGV>
    static void PrintArgs(std::ostream& ostream, T const& first, ARGV const&... rest);

    /// @brief Recursive print base case
    /// @param ostream Output stream
    static void PrintArgs(std::ostream& ostream); // Base case

    /// @brief References the currently loaded Logger. By default this is an StdoutLogger on hosted systems and a
    /// VoidLogger when running on bare metal.
    static Logger& m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename... ARGV>
void StaticLogging::Log(Level level, char const* id, char const* context, ARGV... args)
{
    std::stringstream data_str;

    // TODO cache timestamp

    StartPrintArgs(data_str, args...);
    LogEntry(level, id, context, data_str.str().c_str());
}

//  Private ============================================================================================================

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
