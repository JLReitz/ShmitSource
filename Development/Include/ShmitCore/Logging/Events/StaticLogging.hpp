#pragma once

#include <ShmitCore/Logging/Logger.hpp>

#include <cstdarg>
#include <cstdio>

namespace shmit
{
namespace log
{
namespace events
{

/**!
 * @brief Static event logging interface
 *
 */
class Logging
{
public:
    /**!
     * @brief Submits an event log to be posted by the loaded logger
     *
     * @tparam Context Log context
     * @param[in] level Event log severity level
     * @param[in] id Event log ID
     * @param[in] context_instance Identifies and correlates an instance of Context::type with this log. If the
     * instance's class is a derivative of shmit::Named, and it has a valid name assigned at runtime, the log
     * will reference it by that name and otherwise by its address. If the log should not reference a specific
     * object instance, nullptr may be passed instead.
     * @param[in] message Event message. Supports standard printf format. Pass nullptr if no message is
     * desired, in this case the format arguments (following) will be ignored.
     * @param[in] ... Event message format arguments
     */
    template<typename Context>
    static void Log(Level level, char const* id, typename Context::type const* context_instance, char const* message, ...);

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename Context>
void Logging::Log(Level level, char const* id, typename Context::type const* context_instance, char const* message,
                  ...) // Static method
{
    using ContextType = typename Context::type;

    static_assert(std::is_void_v<ContextType> || std::is_class_v<ContextType>, "Context::type must be void or a class "
                                                                               "implementation");

    constexpr size_t kEventStringSize = 256;
    char             event_c_str[kEventStringSize] {};
    size_t           event_str_length = 0;

    // TODO cache timestamp

    // Print the name of the logged instance to the diagnostic string
    event_str_length += print_name_of_instance(context_instance, event_c_str);

    // Print event message, if it exists
    if (message)
    {
        std::va_list message_args;
        va_start(message_args, message);
        event_c_str[event_str_length++] = ',';
        event_str_length += protected_vsnprintf((event_c_str + event_str_length), (kEventStringSize - event_str_length),
                                                message, message_args);
    }

    // If the submission's level passes the filter, post it
    String event_str {event_c_str, event_str_length};
    if (level >= m_threshold)
        m_logger.Post(Type::eEvent, level, Context::kName, id, event_str);
}

} // namespace events
} // namespace log
} // namespace shmit
