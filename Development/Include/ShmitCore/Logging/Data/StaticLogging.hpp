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
    template<typename LoggingContext, typename... ARGV>
    static void Log(Level level, char const* id, typename LoggingContext::type const* context_ref, ARGV... args);

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
    static void LogEntry(Level level, char const* context, char const* id, char const* data_str);

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

template<typename LoggingContext, typename... ARGV>
void StaticLogging::Log(Level level, char const* id, typename LoggingContext::type const* context_ref, ARGV... args) // Static method
{
    using ContextType = typename LoggingContext::type;

    static_assert(std::is_void_v<ContextType> || std::is_class_v<ContextType>, "LoggingContext::type must be void or a "
                                                                               "class implementation");

    constexpr size_t kDataStringSize {256};
    char             data_str[kDataStringSize] {};
    size_t           data_str_length {0};

    std::stringstream data_strstr;

    // TODO cache timestamp

    // Print the name of the logged instance to the data string
    data_str_length += print_name_of_instance(context_ref, data_str);
    data_strstr << data_str;

    PrintArgs(data_strstr, args...);
    LogEntry(level, LoggingContext::kName, id, data_strstr.str().c_str());
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

} // namespace data
} // namespace log
} // namespace shmit
