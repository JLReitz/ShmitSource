#pragma once

#include "Detail/Posit.hpp"

#include <ShmitCore/Logging/Loggers/Logger.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <string.h>
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
    /// @brief Public entry point for logging a diagnostic posit to the system
    /// @tparam T Context type that owns the posit, void if the context is global
    /// @param posit Diagnostic posit
    /// @param data Address of the variable database used by the posit's data points
    /// @param context_name Name of the context that owns the Posit
    /// @param context_ref Address of the context instance logging the posit, nullptr if the context is global
    template<typename T>
    static void Log(Posit const& posit, uint8_t* data, char const* context_name, T const* context_ref);

    /// @brief Loads a logger instance in to the static interface. This replaces the previously loaded Logger.
    /// @param logger Logger instance
    static void LoadLogger(Logger& logger);

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
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void StaticLogging::Log(Posit const& posit, uint8_t* data, char const* context_name,
                        T const* context_ref) // Static method
{
    constexpr size_t kDiagnosticStringSize = 100; // TODO determine good length for this
    char             diagnostic_str[kDiagnosticStringSize];

    size_t diagnostic_str_length = 0;

    // TODO cache timestamp

    // Log context name together with reference's name
    char   context_reference_str[Named::kMaxSize] = {0};
    size_t context_name_length                    = strnlen(context_name, Named::kMaxSize);
    (void)strncpy(context_reference_str, context_name, context_name_length);
    if constexpr (IsNamed<T>::value)
    {
        // If the reference type is named, log that name if it is valid, otherwise substitute the address
        if (context_ref->NameLength() > 0)
            (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%s", context_ref->GetName());
        else
        {
            (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%p", (void*)context_ref);
        }
    }
    else
    {
        // Else, substitute the name for the address
        (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%p", (void*)context_ref);
    }

    // Run through data points for the posit, executing their functions and appending their results to 'diagnostic_str'
    size_t           variable_data_index = 0;
    size_t           num_data_points     = posit.DataPointCount();
    DataPoint const* data_points         = posit.DataPoints();
    for (size_t i = 0; i < num_data_points; i++)
    {
        // Functions return the amount of data written to the diagnostic string
        DataPoint const& data_point = data_points[i];
        diagnostic_str_length += (*data_point.function)(data_point.const_data, (data + variable_data_index),
                                                        &diagnostic_str[diagnostic_str_length],
                                                        (kDiagnosticStringSize - diagnostic_str_length));
        variable_data_index += data_point.variable_data_size;
    }

    LogEntry(posit.GetLevel(), posit.GetTag(), context_reference_str, diagnostic_str);
}

} // namespace diagnostics
} // namespace log
} // namespace shmit