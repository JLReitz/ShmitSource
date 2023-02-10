#include "StaticLogging.hpp"

//  Standard Logging    -----------------------------------------------------------------------------------------------

/// Submits a trace event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_TRACE_EVENT("example", "trace-event", "x = %d", x)
#define LOG_TRACE_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eTrace, id, context, message, __VA_ARGS__);

/// Submits a debug event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_DEBUG_EVENT("example", "debug-event", "x = %d", x)
#define LOG_DEBUG_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eDebug, id, context, message, __VA_ARGS__);

/// Submits an info event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_INFO_EVENT("example", "info-event", "x = %d", x)
#define LOG_INFO_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eInfo, id, context, message, __VA_ARGS__);

/// Submits a warning event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_WARNING_EVENT("example", "warning-event", "x = %d", x)
#define LOG_WARNING_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eWarning, id, context, message, __VA_ARGS__);

/// Submits an error event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_ERROR_EVENT("example", "error-event", "x = %d", x)
#define LOG_ERROR_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eError, id, context, message, __VA_ARGS__);

/// Submits a fatal event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     message - Standard printf format string
///     ... = Format arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_FATAL_EVENT("example", "fatal-event", "x = %d", x)
#define LOG_FATAL_EVENT(context, id, message, ...) \
    shmit::log::events::Logging::Log(shmit::log::Level::eFatal, id, context, message, __VA_ARGS__);

//  String Logging  ---------------------------------------------------------------------------------------------------

/// Submits a trace event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
///     LOG_TRACE_EVENT_STRING("example", "trace-event", "This is a trace event")
#define LOG_TRACE_EVENT_STRING(context, id, str) LOG_TRACE_EVENT(context, id, "%s", str)

/// Submits a debug event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
///     LOG_DEBUG_EVENT_STRING("example", "debug-event", "This is a debug event")
#define LOG_DEBUG_EVENT_STRING(context, id, str) LOG_DEBUG_EVENT(context, id, "%s", str)

/// Submits an info event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
///     LOG_INFO_EVENT_STRING("example", "info-event", "This is an info event")
#define LOG_INFO_EVENT_STRING(context, id, str) LOG_INFO_EVENT(context, id, "%s", str)

/// Submits a warning event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
///     LOG_WARNING_EVENT_STRING("example", "warning-event", "This is a warning event")
#define LOG_WARNING_EVENT_STRING(context, id, str) LOG_WARNING_EVENT(context, id, "%s", str)

/// Submits an error event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
///     LOG_ERROR_EVENT_STRING("example", "error-event", "This is an error event")
#define LOG_ERROR_EVENT_STRING(context, id, str) LOG_ERROR_EVENT(context, id, "%s", str)

/// Submits a fatal event to the static logging interface
/// Parameters:
///     context - Event context
///     id - Event ID
///     str - Message string literal
///
/// Usage example:
///
//      LOG_FATAL_EVENT_STRING("example", "fatal-event", "This is a fatal event")
#define LOG_FATAL_EVENT_STRING(context, id, str) LOG_FATAL_EVENT(context, id, "%s", str)
