#pragma once

#include "Detail/Posit.hpp"
#include "Interactions.hpp"

#include <ShmitCore/Help/CString.hpp>
#include <ShmitCore/Logging/Logger.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <cstdio>

namespace shmit
{
namespace log
{
namespace diagnostics
{

/// @brief Static diagnostics logging interface
class StaticLogging
{
public:
    /// @brief Public entry point for logging a diagnostic Posit to the system
    /// @tparam T Context type that owns the posit, void if the context is global
    /// @param[in] posit Diagnostic posit
    /// @param[in] data Address of the variable database used by the posit's data points
    /// @param[in] context_name Name of the context that owns the Posit
    /// @param[in] context_ref Address of the context instance logging the posit if it is within a class scope, nullptr
    /// if the context is namespace-scoped or global
    /// @param[in] interactions List of interactions to perform with the Posit
    template<typename T>
    static void Log(Posit const& posit, uint8_t* data, char const* context_name, T const* context_ref,
                    std::initializer_list<Interaction> interactions);

    /// @brief Loads a logger instance in to the static interface. This replaces the previously loaded Logger.
    /// @param logger Logger instance
    static void LoadLogger(Logger& logger);

    /// @brief Configures the level threshold for the static interface
    /// @param level Log entries below this level will not be published
    static void SetThreshold(Level level);

private:
    /// @brief Forwards compiled entry fields to the currently loaded Logger
    /// @param level Diagnostic log level
    /// @param tag Diagnostic posit tag string
    /// @param context Diagnostic context name
    /// @param diagnostic_str Diagnostic message string
    static void LogEntry(Level level, char const* tag, char const* context, char const* diagnostic_str);

    /// @brief References the currently loaded Logger. By default this is an StdoutLogger on hosted systems and a
    /// VoidLogger when running on bare metal.
    static Logger& m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  StaticLogging method definitions in alphabetical order      ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename T>
void StaticLogging::Log(Posit const& posit, uint8_t* data, char const* context_name, T const* context_ref,
                        std::initializer_list<Interaction> interactions) // Static method
{
    constexpr size_t kDiagnosticStringSize = 256;
    char             diagnostic_str[kDiagnosticStringSize] {};
    size_t           diagnostic_str_length = 0;

    // TODO cache timestamp

    // If the reference type is named, log that name if it is valid, otherwise substitute the address
    if constexpr (IsNamed<T>::value)
    {
        if (context_ref->NameLength() > 0)
            diagnostic_str_length += protected_snprintf(diagnostic_str, Named::kMaxSize, "%s", context_ref->GetName());
        else
            diagnostic_str_length += protected_snprintf(diagnostic_str, Named::kMaxSize, "%p", (void*)context_ref);
    }
    // Else, substitute the name for the address
    else
        diagnostic_str_length += protected_snprintf(diagnostic_str, Named::kMaxSize, "%p", (void*)context_ref);

    // Iterate through interactions, executing them in order and appending their results to 'diagnostic_str'
    for (const Interaction& interaction : interactions)
    {
        diagnostic_str[diagnostic_str_length++] = ','; // Prefix comma before interaction writes its output
        diagnostic_str_length +=
            interaction(&diagnostic_str[diagnostic_str_length], (kDiagnosticStringSize - diagnostic_str_length));
    }

    // Iterate through data points for the posit, executing their processing function and appending their results to
    // 'diagnostic_str'
    size_t           variable_data_index = 0;
    size_t           num_data_points     = posit.DataPointCount();
    DataPoint const* data_points         = posit.DataPoints();
    for (size_t i = 0; i < num_data_points; i++)
    {
        DataPoint const& data_point             = data_points[i];
        diagnostic_str[diagnostic_str_length++] = ','; // Prefix comma before DataPoint writes its output
        diagnostic_str_length += (*data_point.function)(data_point.const_data, (data + variable_data_index),
                                                        &diagnostic_str[diagnostic_str_length],
                                                        (kDiagnosticStringSize - diagnostic_str_length));
        variable_data_index += data_point.variable_data_size;
    }

    // Log the entry
    LogEntry(posit.GetLevel(), posit.GetTag(), context_name, diagnostic_str);
}

} // namespace diagnostics
} // namespace log
} // namespace shmit