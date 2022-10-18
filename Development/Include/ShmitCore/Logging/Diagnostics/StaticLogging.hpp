#pragma once

#include "Detail/Posit.hpp"

#include <ShmitCore/Logging/Loggers/LoggerInterface.hpp>
#include <ShmitCore/Types/Generic/Named.hpp>

#include <string.h>
#include <cstdio>

namespace shmit
{
namespace log
{
namespace diagnostics
{

class StaticLogging
{
public:
    static void LoadLogger(Logger& logger);

    template<typename T>
    static void Log(diagnostics::Posit const& posit, uint8_t* data, char const* context, T const* ref);

private:
    static void LogEntry(Level level, char const* tag, char const* context, char const* diagnostic_str);

    static Logger* m_logger;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void StaticLogging::Log(diagnostics::Posit const& posit, uint8_t* data, char const* context,
                        T const* ref) // Static method
{
    constexpr size_t kDiagnosticStringSize = 100; // TODO determine good length for this
    char             diagnostic_str[kDiagnosticStringSize];

    size_t diagnostic_str_length = 0;

    // TODO cache timestamp

    // Log context name together with reference's name
    char   context_reference_str[Named::kMaxSize] = {0};
    size_t context_name_length                    = strnlen(context, Named::kMaxSize);
    (void)strncpy(context_reference_str, context, context_name_length);
    if constexpr (IsNamed<T>::value)
    {
        // If the reference type is named, log that name if it is valid, otherwise substitute the address
        if (ref.NameLength() > 0)
            (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%s", ref->GetName());
        else
        {
            (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%p", (void*)ref);
        }
    }
    else
    {
        // Else, substitute the name for the address
        (void)snprintf(&context_reference_str[context_name_length], Named::kMaxSize, "-%p", (void*)ref);
    }

    // Run through data points for the posit, executing their functions and appending their results to 'diagnostic_str'
    size_t                                variable_data_index = 0;
    size_t                                num_data_points     = posit.DataPointCount();
    diagnostics::detail::DataPoint const* data_points         = posit.DataPoints();
    for (size_t i = 0; i < num_data_points; i++)
    {
        // Functions return the amount of data written to the diagnostic string
        diagnostics::detail::DataPoint const& data_point = data_points[i];
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