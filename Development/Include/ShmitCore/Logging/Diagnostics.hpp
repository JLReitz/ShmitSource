#pragma once

#include "Diagnostics/Logging.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Standard logging                ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Posts a diagnostic log
 *
 * @param[in] level Diagnostic log severity level
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_DIAGNOSTICS(level, id, message, ...) \
    if constexpr (std::is_class_v<LoggingContext_t>) \
        shmit::log::diagnostics::Logging::Log<LoggingContext>(level, id, this, message, __VA_ARGS__); \
    else \
        shmit::log::diagnostics::Logging::Log<LoggingContext>(level, id, (LoggingContext_t*)nullptr, message, __VA_ARGS__);

/**!
 * @brief Posts a trace diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_TRACE_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eTrace, id, nullptr, (void*)nullptr)

/**!
 * @brief Posts a debug diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_DEBUG_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eDebug, id, nullptr, (void*)nullptr)

/**!
 * @brief Posts an info diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_INFO_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eInfo, id, nullptr, (void*)nullptr)

/**!
 * @brief Posts a warning diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_WARNING_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eWarning, id, nullptr, (void*)nullptr)

/**!
 * @brief Posts an error diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_ERROR_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eError, id, nullptr, (void*)nullptr)

/**!
 * @brief Posts a fatal diagnostic log with no message
 *
 * @param[in] id Diagnostic log ID
 */
#define LOG_FATAL_DIAGNOSTICS(id) LOG_DIAGNOSTICS(shmit::log::Level::eFatal, id, nullptr, (void*)nullptr)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Message logging                 ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Posts a trace diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_TRACE_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eTrace, id, message, __VA_ARGS__)

/**!
 * @brief Posts a debug diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_DEBUG_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eDebug, id, message, __VA_ARGS__)

/**!
 * @brief Posts an info diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_INFO_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eInfo, id, message, __VA_ARGS__)

/**!
 * @brief Posts a warning diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_WARNING_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eWarning, id, message, __VA_ARGS__)

/**!
 * @brief Posts an error diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_ERROR_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eError, id, message, __VA_ARGS__)

/**!
 * @brief Posts a fatal diagnostic log with encoded message
 *
 * @param[in] id Diagnostic log ID
 * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is desired, in
 * this case the format arguments (following) will be ignored.
 * @param[in] ... Diagnostic message format arguments
 */
#define LOG_FATAL_DIAGNOSTICS_MESSAGE(id, message, ...) \
    LOG_DIAGNOSTICS(shmit::log::Level::eFatal, id, message, __VA_ARGS__)

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String logging                  ////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Posts a trace diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_TRACE_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eTrace, id, "%s", str)

/**
 * @brief Posts a debug diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_DEBUG_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eDebug, id, "%s", str)

/**
 * @brief Posts an info diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_INFO_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eInfo, id, "%s", str)

/**
 * @brief Posts a warning diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_WARNING_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eWarning, id, "%s", str)

/**
 * @brief Posts an error diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_ERROR_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eError, id, "%s", str)

/**
 * @brief Posts a fatal diagnostic log with encoded c-string
 *
 * @param[in] id Diagnostic log ID
 * @param[in] str String literal or char const*
 */
#define LOG_FATAL_DIAGNOSTICS_STRING(id, str) LOG_DIAGNOSTICS(shmit::log::Level::eFatal, id, "%s", str)
