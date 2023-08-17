#pragma once

#include "Category.hpp"
#include "Detail/Interface.hpp"
#include "Element.hpp"
#include "Logger.hpp"

namespace shmit
{
namespace log
{

extern Logger& default_logger;

template<typename Category>
class StaticInterface : public detail::Interface
{
    static_assert(detail::is_category_v<Category>, "shmit::log::Context Category parameter must be specialization of "
                                                   "shmit::log::Category");

public:
    template<typename EntryType>
    static void Log(Level severity, EntryType const& msg);

    template<typename ContextType, typename EntryType>
    static void Log(Level severity, ContextType const& context, EntryType const& entry);

    static void SetThreshold(Level threshold);

    static void SetLogger(Logger& logger);

private:
    static Level   m_severity_threshold {Level::eInfo};
    static Logger* m_logger {&default_logger};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  LoggingInterface method definitions in alphabetical order           ////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public  ============================================================================================================

template<typename Category>
void StaticInterface<Category>::SetThreshold(Level threshold)
{
    m_severity_threshold = threshold;
}

template<typename Category>
void StaticInterface<Category>::SetLogger(Logger& logger)
{
    m_logger = &logger;
}

template<typename Category>
template<typename EntryType>
void StaticInterface<Category>::Log(Level severity, EntryType const& msg)
{
    Log(severity, NullContext(), msg);
}

template<typename Category>
template<typename ContextType, typename EntryType>
void StaticInterface<Category>::Log(Level severity, ContextType const& context, EntryType const& entry)
{
    constexpr static size_t kContextSize {Posit::kElementHeaderSize + ContextType::kDataSize};
    constexpr static size_t kEntrySize {Posit::kElementHeaderSize + EntryType::kDataSize};
    constexpr static size_t kPositSize {Posit::kPrefixStr.size() + Posit::kHeaderSize + kContextSize + kEntrySize +
                                        Posit::kPostfixStr.size()};

    // Check against severity threshold, only continue if it passes
    if (severity >= m_severity_threshold)
    {
        uint8_t posit_payload[kPositSize] {};
        Posit   posit {Category::kLabelId, severity, posit_payload};

        // Populate posit with context header and data
        Payload::char_type* context_start {posit.ElementStart()};
        ElementHeader*      context_header {reinterpret_cast<ElementHeader*>(context_start)};
        context_header->tag_id    = ContextType::kTagId;
        context_header->string_id = ContextType::kStringId;
        context.EncodeData(context_start + kElementHeaderSize);

        // Populate posit with entry header and data
        // Entry header begins after context data ends
        Payload::char_type* entry_start {context_start + kElementHeaderSize + ContextType::kDataSize};
        ElementHeader*      entry_header {reinterpret_cast<ElementHeader*>(entry_start)};
        context_header->tag_id  = EntryType::kTagId;
        entry_header->string_id = EntryType::kStringId;
        entry.EncodeData(entry_start + kElementHeaderSize);

        // Post posit to logger
        m_logger->Post(posit);
    }
}

} // namespace log
} // namespace shmit