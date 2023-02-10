#pragma once

#include <ShmitCore/Types/StdTypes.hpp>
#include <ShmitCore/Types/Traits/Named.hpp>

#include <type_traits>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Logging context declaration             ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define ESTABLISH_LOGGING_CONTEXT(name) \
    struct LoggingContext \
    { \
        using type                         = std::conditional_t<std::is_class_v<name>, name, void>; \
        static constexpr char const* kName = #name; \
    }; \
    using LoggingContext_t = typename LoggingContext::type; \
    static constexpr LoggingContext kLoggingContext {};

#define INHERIT_LOGGING_CONTEXT() using LoggingContext = decltype(kLoggingContext);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Commonly used log IDs           ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace ids
{

static constexpr char const* kConstructing = "constructing";
static constexpr char const* kDestructing  = "destructing";

static constexpr char const* kAlllocated   = "allocated";
static constexpr char const* kDeallocating = "deallocating";

static constexpr char const* kAtCapacity = "at-capacity";

static constexpr char const* kBadAllocation = "bad-allocation";
static constexpr char const* kBadMove       = "bad-move";

} // namespace ids

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Metafunction definitions            ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Default (failed) check for if a type contains a logging context
 *
 * @tparam T Any type
 * @tparam typename (anonymous) Defaults to void
 */
template<class T, typename = void>
struct has_logging_context : public std::false_type
{
};

/**!
 * @brief Successful check for if a type contains a logging context
 *
 * @tparam T Any type which has established a logging context within
 */
template<class T>
struct has_logging_context<T, typename T::LoggingContext> : public std::true_type
{
};

/**!
 * @brief Accesses the returned type from has_logging_context
 *
 * @tparam T Any type
 */
template<class T>
using has_logging_context_t = typename has_logging_context<T>::type;

/**!
 * @brief Accesses the returned value from has_logging_context
 *
 * @tparam T Any type
 */
template<class T>
inline constexpr bool has_logging_context_v = has_logging_context<T>::value;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Logging type declarations           ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Log type
 *
 */
enum class Type : uint8_t
{
    eData        = 0,
    eDiagnostics = 1,
    eEvent       = 2
};

/**!
 * @brief Converts a log type to its string representation
 *
 * @param type Log type
 * @return C-string of the tyoe
 */
constexpr const char* TypeToString(Type type);

/**!
 * @brief Log severity level
 *
 */
enum class Level : uint8_t
{
    eTrace   = 0,
    eDebug   = 1,
    eInfo    = 2,
    eWarning = 3,
    eError   = 4,
    eFatal   = 5
};

/**!
 * @brief Converts a log severity level to its string representation
 *
 * @param level Severity level
 * @return C-string of the level
 */
constexpr const char* LevelToString(Level level);

class RawString
{
    // TODO iterators

private:
    std::string_view m_raw;
    char*            m_format;
};

template<typename... Argv>
struct FormatTable
{
    template<typename First, typename... Argv2>
    void populate_element_length(size_t* element_start)
    {
        if constexpr (sizeof...(ARGV) == 0)
            *element_start = sizeof(First);
        else
            populate_element_length<Argv2...>((element_start + 1));
    }

    constexpr FormatTable()
    {
        populate_element_length<Argv...>(element_length);
    }

    static constexpr size_t kNumElements = sizeof...(Argv);
    size_t                  element_length[kNumElements];
};

template<typename... Argv>
class FormatString
{
private:
    static constexpr FormatTable<Argv...> kFormatTable;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Implementation details          ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{

// Constant c-style strings that are returned by the converter functions below

constexpr char const* typeDataStr        = "DATA";
constexpr char const* typeDiagnosticsStr = "DIAG";
constexpr char const* typeEventStr       = "EVENT";

constexpr char const* levelTraceStr    = "TRACE";
constexpr char const* levelDebugStr    = "DEBUG";
constexpr char const* levelInfoStr     = "INFO";
constexpr char const* levelWarningeStr = "WARNING";
constexpr char const* levelErrorStr    = "ERROR";
constexpr char const* levelFatalStr    = "FATAL";

constexpr char const* unknownValueStr = "UNKNOWN";

} // namespace detail

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Function definitions in alphabetical order          ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline constexpr const char* LevelToString(Level level)
{
    if (level == Level::eTrace)
        return detail::levelTraceStr;
    else if (level == Level::eDebug)
        return detail::levelDebugStr;
    else if (level == Level::eInfo)
        return detail::levelInfoStr;
    else if (level == Level::eWarning)
        return detail::levelWarningeStr;
    else if (level == Level::eError)
        return detail::levelErrorStr;
    else if (level == Level::eFatal)
        return detail::levelFatalStr;

    return detail::unknownValueStr;
}

template<typename T>
inline size_t print_name_of_instance(T const* instance, char* out_str)
{
    constexpr char const* kNullStr       = "NULL";
    constexpr size_t      kNullStrLength = strlen(kNullStr);

    // Print the instance's name to the output string
    size_t char_count = 0;
    if (instance)
        char_count = get_name_of_instance(*instance, out_str);
    else
        char_count = protected_snprintf(out_str, kNullStrLength, "%s", kNullStr);

    return char_count;
}

inline constexpr const char* TypeToString(Type type)
{
    if (type == Type::eData)
        return detail::typeDataStr;
    else if (type == Type::eDiagnostics)
        return detail::typeDiagnosticsStr;
    else if (type == Type::eEvent)
        return detail::typeEventStr;

    return detail::unknownValueStr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Base logging interface                  ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Logger; // Forward declaration

namespace detail
{

class LoggingInterface
{
public:
    /**!
     * @brief Replaces the current event logger with a new one
     *
     * @param[in] logger Logger instance
     */
    static void LoadLogger(Logger& logger);

    /**!
     * @brief Set the severity threshold for event logs
     *
     * @param[in] level Severity threshold
     */
    static void SetThreshold(Level level);

private:
    static Level   m_threshold; /*! Diagnostics severity threshold */
    static Logger& m_logger;    /*! Event logger */
};

} // namespace detail
} // namespace log
} // namespace shmit
