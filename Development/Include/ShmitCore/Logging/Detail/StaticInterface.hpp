#pragma once

#include "../Category.hpp"
#include "../Element.hpp"
#include "../Logger.hpp"

namespace shmit
{
namespace log
{

extern Logger& default_logger;

namespace detail
{

template<typename CategoryT>
class StaticInterface
{
    static_assert(detail::is_CategoryT_v<CategoryT>, "shmit::log::Context CategoryT parameter must be specialization "
                                                     "of shmit::log::Category");

public:
    using type     = StaticInterface<CategoryT>;
    using Category = typename CategoryT::type;

    template<typename EntryT>
    static void Log(Severity severity, EntryT const& msg);

    template<typename ContextT, typename EntryT>
    static void Log(Severity severity, ContextT const& context, EntryT const& entry);

    static void SetThreshold(Severity threshold);

    static void SetLogger(Logger& logger);

private:
    static Severity m_severity_threshold {Severity::eInfo};
    static Logger*  m_logger {&default_logger};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LoggingInterface method definitions in alphabetical order           ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename CategoryT>
void StaticInterface<CategoryT>::SetThreshold(Severity threshold)
{
    m_severity_threshold = threshold;
}

template<typename CategoryT>
void StaticInterface<CategoryT>::SetLogger(Logger& logger)
{
    m_logger = &logger;
}

template<typename CategoryT>
template<typename EntryT>
void StaticInterface<CategoryT>::Log(Severity severity, EntryT const& msg)
{
    Log(severity, NullContext {}, msg);
}

template<typename CategoryT>
template<typename ContextT, typename EntryT>
void StaticInterface<CategoryT>::Log(Severity severity, ContextT const& context, EntryT const& entry)
{
    constexpr static size_t kContextSize {Posit::kElementHeaderSize + ContextT::kDataSize};
    constexpr static size_t kEntrySize {Posit::kElementHeaderSize + EntryT::kDataSize};
    constexpr static size_t kPositSize {Posit::kPrefixStr.size() + Posit::kHeaderSize + kContextSize + kEntrySize +
                                        Posit::kPostfixStr.size()};

    // Check against severity threshold, only continue if it passes
    if (severity >= m_severity_threshold)
    {
        uint8_t posit_payload[kPositSize] {};
        Posit   posit {CategoryT::kLabelId, severity, posit_payload};

        // Populate posit with context header and data
        Posit::Packet::char_type* context_start {posit.GetStartOfData()};
        size_t                    context_length {context.Encode(context_start)};

        // Populate posit with entry header and data
        // Entry header begins after context data ends
        Posit::Packet::char_type* entry_start {context_start + context_length};
        (void)entry.Encode(entry_start);

        // Post posit to logger
        m_logger->Post(posit);
    }
}

} // namespace detail
} // namespace log
} // namespace shmit