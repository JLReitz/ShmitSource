#pragma once

#include "../Logger.hpp"

#include <ShmitCore/Platform/Time.hpp>

#include <cstdarg>

namespace shmit
{
namespace log
{
namespace diagnostics
{

/**!
 * @brief Static diagnostics logging interface
 *
 */
class Logging
{
public:
    /**!
     * @brief Submits a diagnostic log
     *
     * @tparam Context Logging context
     * @param[in] level Diagnostic log severity level
     * @param[in] id Diagnostic log ID
     * @param[in] context_instance Identifies and correlates an instance of Context::type with this log. If the
     * instance's class is a derivative of shmit::Named, and it has a valid name assigned at runtime, the log
     * will reference it by that name and otherwise by its address. If the log should not reference a specific
     * object instance, nullptr may be passed instead.
     * @param[in] message Diagnostic message. Supports standard printf format. Pass nullptr if no message is
     * desired, in this case the format arguments (following) will be ignored.
     * @param[in] ... Diagnostic message format arguments
     */
    template<typename Context>
    static void Log(Level level, char const* id, typename Context::type const* context_instance, char const* message,
                    ...) noexcept;

    /**!
     * @brief Replaces the current diagnostic logger with a new one
     *
     * @param[in] logger Logger instance
     */
    static void LoadLogger(Logger& logger);

    /**!
     * @brief Set the severity threshold for all diagnostic logs. Any that are submitted with a level below this
     * threshold will not be posted to the logger.
     *
     * @param[in] level Severity threshold
     */
    static void SetThreshold(Level level);

private:
    static Level   m_threshold; /*! Diagnostics severity threshold */
    static Logger& m_logger;    /*! Diagnostics logger */
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename Context>
void Logging::Log(Level level, char const* id, typename Context::type const* context_instance, char const* message,
                  ...) noexcept // Static method
{
    using ContextType = typename Context::type;

    static_assert(std::is_void_v<ContextType> || std::is_class_v<ContextType>, "Context::type must be void or a class "
                                                                               "implementation");

    constexpr size_t kDiagnosticStringSize = 256;
    char             diagnostic_c_str[kDiagnosticStringSize] {};
    size_t           diagnostic_str_length = 0;

    // Print the name of the logged instance to the diagnostic string
    diagnostic_str_length += print_name_of_instance(context_instance, diagnostic_c_str);

    // Print diagnostic message, if it exists
    if (message)
    {
        std::va_list message_args;
        va_start(message_args, message);
        diagnostic_c_str[diagnostic_str_length++] = ',';
        diagnostic_str_length += protected_vsnprintf((diagnostic_c_str + diagnostic_str_length),
                                                     (kDiagnosticStringSize - diagnostic_str_length), message,
                                                     message_args);
    }

    // If the submission's level passes the filter, post it
    shmit::log::String diagnostic_str {diagnostic_c_str, diagnostic_str_length};
    if (level >= m_threshold)
        m_logger.Post(Type::eDiagnostics, level, Context::kName, id, diagnostic_str);
}

} // namespace diagnostics
} // namespace log
} // namespace shmit