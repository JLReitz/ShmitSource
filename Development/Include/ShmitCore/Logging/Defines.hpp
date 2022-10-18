#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{
namespace log
{

namespace detail
{

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

enum class Type : uint8_t
{
    eData        = 0,
    eDiagnostics = 1,
    eEvent       = 2
};

enum class Level : uint8_t
{
    eTrace   = 0,
    eDebug   = 1,
    eInfo    = 2,
    eWarning = 3,
    eError   = 4,
    eFatal   = 5
};

constexpr const char* TypeToString(Type type)
{
    if (type == Type::eData)
        return detail::typeDataStr;
    else if (type == Type::eDiagnostics)
        return detail::typeDiagnosticsStr;
    else if (type == Type::eEvent)
        return detail::typeEventStr;

    return detail::unknownValueStr;
}

constexpr const char* LevelToString(Level level)
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

} // namespace log
} // namespace shmit