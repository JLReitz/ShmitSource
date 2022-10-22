#pragma once

#include "StaticLogging.hpp"

/// Submits trace level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_TRACE_DATA("Example", "trace-data", x)
#define LOG_TRACE_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eTrace, id, context, __VA_ARGS__);

/// Submits debug level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_DEBUG_DATA("Example", "debug-data", x)
#define LOG_DEBUG_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eDebug, id, context, __VA_ARGS__);

/// Submits info level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_INFO_DATA("Example", "info-data", x)
#define LOG_INFO_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eInfo, id, context, __VA_ARGS__);

/// Submits warning level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_WARNING_DATA("Example", "warning-data", x)
#define LOG_WARNING_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eWarning, id, context, __VA_ARGS__);

/// Submits error level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_ERROR_DATA("Example", "error-data", x)
#define LOG_ERROR_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eError, id, context, __VA_ARGS__);

/// Submits fatal level data to the static logging interface
/// Parameters:
///     context - Data context
///     id - Data ID
///     ... = Data arguments
///
/// Usage example:
///
///     int x = 69;
///     LOG_FATAL_DATA("Example", "fatal-data", x)
#define LOG_FATAL_DATA(context, id, ...) \
    shmit::log::events::data::StaticLogging::Log(shmit::log::Level::eFatal, id, context, __VA_ARGS__);