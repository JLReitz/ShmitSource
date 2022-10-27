#pragma once

#include "DataPoints.hpp"
#include "StaticLogging.hpp"

/// Establishes a diagnostic context within the current scope
/// The context's name is represented by a constexpr c-style string named 'kDiagnosticContextName'
#define DIAGNOSTIC_CONTEXT(name) static constexpr char const* kDiagnosticContextName = #name;

/// Declares a basic Posit within the current diagnostic context.
/// Parameters:
///     tag - Means for identifying the posit. Provided as an expression, not a string.
///     level - shmit::log::Level
///
/// The Posit is a static const instance of 'shmit::log::diagnostic::Posit', named after the tag provided in this
/// argument list (format: _posit_<tag>). This instance should not be accessed or manipulated manually unless absolutely
/// necessary, developers should rely on the macros within this file to do the heavy lifting as much as possible.
///
/// Usage example:
///
///     Given the following diagnostic context is declared:
///         DIAGNOSTIC_CONTEXT(ExampleContext)
///
///     Then:
///         DIAGNOSTIC_POSIT(example, shmit::log::Level::eDebug)
///
///     Will generate a static const Posit instance named '_posit_example' and variable data set named
///     '_example_variable_data'. This posit does not hold any data points but the variable database of size 0 is still
///     declared for compatability with the static logging interface.
#define DIAGNOSTIC_POSIT(tag, level) \
    static constexpr shmit::log::diagnostics::detail::Posit<0, 0> _posit_##tag = \
        shmit::log::diagnostics::detail::MakePosit<0, 0>(#tag, level, {}); \
    std::array<uint8_t, 0> _posit_##tag##_variable_data {};

/// Declares a Posit with listed DataPoints and complimentary variable database within the current diagnostic context.
/// Parameters:
///     tag - Means for identifying the posit. Provided as an expression, not a string.
///     level - shmit::log::Level
///     ... - Data points to be held by the posit. Core set of available definitions are listed in "DataPoints.hpp".
///
/// The Posit is a static const instance of 'shmit::log::diagnostic::Posit', named after the tag provided in this
/// argument list (format: _posit_<tag>). The variable database is a mutable 'uint8_t' std::array sized appropriately
/// for the accumulated resource needs of the data points held by the posit and is also named after the tag (format:
/// _<tag>_variable_data).
///
/// Neither of these instances should be accessed or manipulated manually unless absolutely
/// necessary, developers should rely on the macros within this file to do the heavy lifting as much as possible.
///
/// Usage example:
///
///     Given the following diagnostic context is declared:
///         DIAGNOSTIC_CONTEXT(ExampleContext)
///
///     Then:
///         DIAGNOSTIC_DATA_POSIT(example, shmit::log::Level::eDebug, shmit::log::diagnostics::Count())
///
///     Will generate a static const Posit instance named '_posit_example' and variable data set named
///     '_example_variable_data'. This posit holds one DataPoint -- Count -- which reports the number of times the Posit
///     has been submitted.
#define DIAGNOSTIC_DATA_POSIT(tag, level, ...) \
    static constexpr size_t kPosit_##tag##_NumDataPoints = \
        shmit::log::diagnostics::detail::CountDataPoints({__VA_ARGS__}); \
    static constexpr size_t kPosit_##tag##_ConstDataSize = \
        shmit::log::diagnostics::detail::ConstDatabaseSize({__VA_ARGS__}); \
    static constexpr size_t kPosit_##tag##_VariableDataSize = \
        shmit::log::diagnostics::detail::VariableDatabaseSize({__VA_ARGS__}); \
    static constexpr shmit::log::diagnostics::detail::Posit<kPosit_##tag##_NumDataPoints, kPosit_##tag##_ConstDataSize> \
        _posit_##tag = \
            shmit::log::diagnostics::detail::MakePosit<kPosit_##tag##_NumDataPoints, kPosit_##tag##_ConstDataSize>( \
                #tag, level, {__VA_ARGS__}); \
    std::array<uint8_t, kPosit_##tag##_VariableDataSize> _posit_##tag##_variable_data {};

/// Submits a posit that is globally defined to the static logging interface
///
/// Usage example:
///
///     Given the following context and posit is declared in the global scope:
///         DIAGNOSTIC_CONTEXT(GlobalContext)
///         DIAGNOSTIC_POSIT(Example, shmit::log::Level::eDebug, shmit::log::diagnostics::NoData())
///
///     That posit can be logged from anywhere:
///         LOG_GLOBAL_DIAGNOSTIC_POSIT(Example)
#define LOG_GLOBAL_DIAGNOSTIC_POSIT(tag) \
    shmit::log::diagnostics::StaticLogging::Log<void>(::_posit_##tag##, ::_posit_##tag##_variable_data.data(), \
                                                      ::kDiagnosticContextName, nullptr);

/// Submits a posit that is a member of the provided namespace to the static logging interface.
///
/// Usage example:
///
///     Given the following context and posit is declared within a namespace 'scoped::example':
///         DIAGNOSTIC_CONTEXT(scoped::example)
///         DIAGNOSTIC_POSIT(Example, shmit::log::Level::eDebug, shmit::log::diagnostics::NoData())
///
///     That posit can be logged from anywhere:
///         LOG_SCOPED_DIAGNOSTIC_POSIT(Example, scoped::example)
#define LOG_SCOPED_DIAGNOSTIC_POSIT(tag, namespace) \
    shmit::log::diagnostics::StaticLogging::Log<void>(namespace## ::_posit_##tag, \
                                                      namespace## ::_posit_##tag##_variable_data.data(), \
                                                      namespace## ::kDiagnosticContextName, nullptr);

/// Submits a posit that is a member of the encompassing class to the static logging interface.
/// Use only when within a class context which holds diagnostic posit that matches the tag.
///
/// Usage example:
///
///     Given the following context and posit is declared within a class 'Impl':
///         DIAGNOSTIC_CONTEXT(Impl)
///         DIAGNOSTIC_POSIT(Example, shmit::log::Level::eDebug, shmit::log::diagnostics::NoData())
///
///     That posit can be logged from within a member method of 'Impl':
///         LOG_MEMBER_DIAGNOSTIC_POSIT(Example)
#define LOG_MEMBER_DIAGNOSTIC_POSIT(tag) \
    shmit::log::diagnostics::StaticLogging::Log(_posit_##tag, _posit_##tag##_variable_data.data(), \
                                                kDiagnosticContextName, this);

#define LOG_INTERACTIVE_MEMBER_DIAGNOSTIC_POSIT(tag, ...) \
    shmit::log::diagnostics::StaticLogging::Log(_posit_##tag, _posit_##tag##_variable_data.data(), \
                                                kDiagnosticContextName, this, {__VA_ARGS__});
