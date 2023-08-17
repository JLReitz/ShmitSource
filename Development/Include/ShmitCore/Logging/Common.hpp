#pragma once

#include "Element.hpp"

namespace shmit
{
namespace log
{

template<typename Context, typename Posit>
inline void Make

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

} // namespace log
} // namespace shmit
