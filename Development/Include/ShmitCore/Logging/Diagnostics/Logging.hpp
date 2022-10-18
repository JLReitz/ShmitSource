#pragma once

#include "DataPoints.hpp"
#include "StaticLogging.hpp"

#define DIAGNOSTIC_CONTEXT(context) static constexpr char const* kDiagnosticContextName = #context;

#define DIAGNOSTIC_POSIT(tag, level, ...) \
    static constexpr size_t kPosit_##tag##_NumDataPoints = \
        shmit::log::diagnostics::detail::CountDataPoints({__VA_ARGS__}); \
    static constexpr size_t kPosit_##tag##_ConstDataSize = \
        shmit::log::diagnostics::detail::ConstDatabaseSize({__VA_ARGS__}); \
    static constexpr size_t kPosit_##tag##_VariableDataSize = \
        shmit::log::diagnostics::detail::VariableDatabaseSize({__VA_ARGS__}); \
    static constexpr shmit::log::diagnostics::detail::Posit<kPosit_##tag##_NumDataPoints, \
                                                            kPosit_##tag##_ConstDataSize> \
        _posit_##tag = \
            shmit::log::diagnostics::detail::MakePosit<kPosit_##tag##_NumDataPoints, kPosit_##tag##_ConstDataSize>( \
                #tag, level, {__VA_ARGS__}); \
    uint8_t _posit_##tag##_variable_data[kPosit_##tag##_VariableDataSize] = {0};

#define LOG_MEMBER_DIAGNOSTIC_POSIT(tag) \
    shmit::log::diagnostics::StaticLogging::Log(_posit_##tag, _posit_##tag##_variable_data, kDiagnosticContextName, \
                                                this);

#define LOG_GLOBAL_DIAGNOSTIC_POSIT(tag) \
    shmit::log::diagnostics::StaticLogging::Log<void>(_posit_##tag, _posit_##tag##_variable_data, \
                                                      kDiagnosticContextName, nullptr);
