#include <ShmitCore/Logging/String.hpp>

namespace shmit
{
namespace log
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String constructor definitions          ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr String::String(char const* str) noexcept : m_str {str}
{
}

constexpr String::String(char const* str, size_t str_length) noexcept : m_str {str, str_length}
{
}

constexpr String::String(std::string_view const& str) noexcept : m_str {str}
{
}

constexpr String::String(String const& rhs) noexcept : m_str {rhs.m_str}, m_suffix {rhs.m_suffix}
{
}

constexpr String::String(String&& rhs) noexcept : m_str {rhs.m_str}, m_suffix {rhs.m_suffix}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String method definitions in alphabetical order               //////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr String::iterator String::begin() const noexcept
{
    return iterator {this};
}

constexpr String::const_iterator String::cbegin() const noexcept
{
    return const_iterator {this};
}

constexpr String::iterator String::end() const noexcept
{
    String* final_suffix = FinalSuffix();
    return iterator {final_suffix, true};
}

constexpr String::const_iterator String::cend() const noexcept
{
    String* final_suffix = FinalSuffix();
    return const_iterator {final_suffix, true};
}

constexpr size_t String::size() const noexcept
{
    size_t        string_length = m_str.size();
    String const* curr          = m_suffix;
    while (curr)
        string_length += curr->m_str.size();
    return string_length;
}

constexpr void String::Append(String& str) noexcept
{
    String* final_suffix   = FinalSuffix();
    final_suffix->m_suffix = &str;
}

constexpr String& String::operator=(String const& rhs) noexcept
{
    m_str    = rhs.m_str;
    m_suffix = rhs.m_suffix;
    return *this;
}

//  Private     ========================================================================================================

constexpr String* String::FinalSuffix() const noexcept
{
    String const* string = this;
    while (string->m_suffix)
        string = string->m_suffix;
    return const_cast<String*>(string);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String::iterator constructor definitions                ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr String::iterator::iterator(iterator const& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}, m_sub_count {rhs.m_sub_count}, m_sub_pos {rhs.m_sub_pos}, m_sub {rhs.m_sub}
{
}

constexpr String::iterator::iterator(iterator&& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}, m_sub_count {rhs.m_sub_count}, m_sub_pos {rhs.m_sub_pos}, m_sub {rhs.m_sub}
{
}

//  Private     ========================================================================================================

constexpr String::iterator::iterator(String const* str, bool end) noexcept
    : m_current_string {str}, m_pos {(end) ? str->m_str.end() + String::kEndOfStringOffset : str->m_str.begin()}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String::iterator method definitions in alphabetical order               ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr char String::iterator::operator*() const noexcept
{
    // Check if currently on substitute
    if (m_sub_pos)
    {
        return *m_sub_pos;
    }
    // Else, check current position for substitute injection token
    // Token is valid if it is found at the beginning of the string or is otherwise not preceded by an escape token
    else if ((*m_pos == String::kSubstituteInjectStartToken) &&
             ((m_pos == m_current_string->m_str.begin()) || (*(m_pos - 1) != String::kEscapeToken)))
    {
        // Get current substitute, if available
        SubstituteEntry* sub = m_current_string->GetSubstitute(m_sub_count);
        if (sub)
        {
            switch (sub->type)
            {
                case String::Substitution::Type::eArithmetic:
                    // Intended fall-through
                case String::Substitution::Type::ePointer:
                {
                    m_sub = std::string_view(static_cast<char*>(sub->start), sub->length);
                }
                case String::Substitution::Type::eCstring:

                    break;
            }
        }

        // If sub is invalid, continue on printing the format tokens
    }

    // Else, check for the current string's end
    // If there is a suffix appended, end with a delimiter, otherwise go to a new line
    if (m_pos == m_current_string->m_str.end())
    {
        if (m_current_string->m_suffix)
            return String::kDelimiter;
        else
            return String::kNewLine;
    }
    else
        return *m_pos;
}

constexpr bool String::iterator::operator==(iterator const& rhs) const noexcept
{
    return (m_pos == rhs.m_pos);
}

constexpr bool String::iterator::operator!=(iterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

constexpr bool String::iterator::operator>(iterator const& rhs) const noexcept
{
    return ((m_current_string == rhs.m_current_string) && (m_pos > rhs.m_pos));
}

constexpr bool String::iterator::operator>=(iterator const& rhs) const noexcept
{
    return ((m_current_string == rhs.m_current_string) && (m_pos >= rhs.m_pos));
}

constexpr bool String::iterator::operator<(iterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool String::iterator::operator<=(iterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr String::iterator String::iterator::operator++() noexcept
{
    iterator tmp = *this;

    // Check if currently traversing a substitute
    if (m_sub_pos)
    {
        // Increment current position
        m_sub_pos++;

        if (m_sub_pos == m_sub.end())
        {
            // Clear the current substitute position, increment the substitute count, and move the string's current
            // position past the substitute injection end token
            m_sub_pos = nullptr;
            m_sub_count++;
            m_pos += 2;
        }
    }
    else
    {
        // Increment current position
        m_pos++;

        // Check current position for substitute injection start token
        // Token is valid if:
        //  1. It is found at the beginning of the string or is otherwise not preceeded by an escape token
        //  2. It is followed by the substitute injection end token
        if ((*m_pos == String::kSubstituteInjectStartToken) &&
            ((m_pos == m_current_string->m_str.begin()) || (*(m_pos - 1) != String::kEscapeToken)) &&
            (((m_pos + 1) < m_current_string->m_str.end()) && (*(m_pos + 1) == String::kSubstituteInjectEndToken)))
        {
            // Get current substitute, if available
            SubstituteEntry* sub = m_current_string->GetSubstitute(m_sub_count);
            if (sub)
            {
                switch (sub->type)
                {
                    case String::Substitution::Type::eArithmetic:
                        // Intended fall-through
                    case String::Substitution::Type::ePointer:
                    {
                        m_sub     = std::string_view(reinterpret_cast<char*>(sub->start), sub->length);
                        m_sub_pos = m_sub.begin();
                        break;
                    }
                    case String::Substitution::Type::eCstring:
                    {
                        char* c_str = reinterpret_cast<char*>(*sub->start);
                        m_sub       = std::string_view(c_str);
                        break;
                    }
                }
            }

            // If sub is invalid, continue on printing the format tokens
        }
    }

    // Check if current position is at the end, including the delimiter/newline offset
    // If it is, and there is a suffix to begin traversing, switch to that
    if ((m_pos == (m_current_string->m_str.end() + String::kEndOfStringOffset)) && m_current_string->m_suffix)
    {
        m_current_string = m_current_string->m_suffix;
        m_pos            = m_current_string->m_str.begin();
    }

    return tmp;
}

constexpr String::iterator& String::iterator::operator++(int) noexcept
{
    // Check if currently traversing a substitute
    if (m_sub_pos)
    {
        // Increment current position
        m_sub_pos++;

        if (m_sub_pos == m_sub.end())
        {
            // Clear the current substitute position, increment the substitute count, and move the string's current
            // position past the substitute injection end token
            m_sub_pos = nullptr;
            m_sub_count++;
            m_pos += 2;
        }
    }
    else
    {
        // Increment current position
        m_pos++;

        // Check current position for substitute injection start token
        // Token is valid if:
        //  1. It is found at the beginning of the string or is otherwise not preceeded by an escape token
        //  2. It is followed by the substitute injection end token
        if ((*m_pos == String::kSubstituteInjectStartToken) &&
            ((m_pos == m_current_string->m_str.begin()) || (*(m_pos - 1) != String::kEscapeToken)) &&
            (((m_pos + 1) < m_current_string->m_str.end()) && (*(m_pos + 1) == String::kSubstituteInjectEndToken)))
        {
            // Get current substitute, if available
            SubstituteEntry* sub = m_current_string->GetSubstitute(m_sub_count);
            if (sub)
            {
                switch (sub->type)
                {
                    case String::Substitution::Type::eArithmetic:
                        // Intended fall-through
                    case String::Substitution::Type::ePointer:
                    {
                        m_sub     = std::string_view(reinterpret_cast<char*>(sub->start), sub->length);
                        m_sub_pos = m_sub.begin();
                        break;
                    }
                    case String::Substitution::Type::eCstring:
                    {
                        char* c_str = reinterpret_cast<char*>(*sub->start);
                        m_sub       = std::string_view(c_str);
                        break;
                    }
                }
            }

            // If sub is invalid, continue on printing the format tokens
        }
    }

    // Check if current position is at the end, including the delimiter/newline offset
    // If it is, and there is a suffix to begin traversing, switch to that
    if ((m_pos == (m_current_string->m_str.end() + String::kEndOfStringOffset)) && m_current_string->m_suffix)
    {
        m_current_string = m_current_string->m_suffix;
        m_pos            = m_current_string->m_str.begin();
    }

    return *this;
}

constexpr String::iterator& String::iterator::operator=(iterator const& rhs) noexcept
{
    m_current_string = rhs.m_current_string;
    m_pos            = rhs.m_pos;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String::const_iterator constructor definitions          ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr String::const_iterator::const_iterator(iterator const& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}, m_sub_count {rhs.m_sub_count}, m_sub_pos {rhs.m_sub_pos}, m_sub {rhs.m_sub}
{
}

constexpr String::const_iterator::const_iterator(const_iterator const& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}, m_sub_count {rhs.m_sub_count}, m_sub_pos {rhs.m_sub_pos}, m_sub {rhs.m_sub}
{
}

constexpr String::const_iterator::const_iterator(const_iterator&& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}, m_sub_count {rhs.m_sub_count}, m_sub_pos {rhs.m_sub_pos}, m_sub {rhs.m_sub}
{
}

//  Private     ========================================================================================================

constexpr String::const_iterator::const_iterator(String const* str, bool end) noexcept
    : m_current_string {str}, m_pos {(end) ? str->m_str.end() + String::kEndOfStringOffset : str->m_str.begin()}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String::const_iterator method definitions in alphabetical order         ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr char const String::const_iterator::operator*() const noexcept
{
    if (m_pos == m_current_string->m_str.end())
    {
        if (m_current_string->m_suffix)
            return String::kDelimiter;
        else
            return String::kNewLine;
    }
    else
        return *m_pos;
}

constexpr bool String::const_iterator::operator==(const_iterator const& rhs) const noexcept
{
    return (m_pos == rhs.m_pos);
}

constexpr bool String::const_iterator::operator!=(const_iterator const& rhs) const noexcept
{
    return !(*this == rhs);
}

constexpr bool String::const_iterator::operator>(const_iterator const& rhs) const noexcept
{
    return ((m_current_string == rhs.m_current_string) && (m_pos > rhs.m_pos));
}

constexpr bool String::const_iterator::operator>=(const_iterator const& rhs) const noexcept
{
    return ((m_current_string == rhs.m_current_string) && (m_pos >= rhs.m_pos));
}

constexpr bool String::const_iterator::operator<(const_iterator const& rhs) const noexcept
{
    return !(*this >= rhs);
}

constexpr bool String::const_iterator::operator<=(const_iterator const& rhs) const noexcept
{
    return !(*this > rhs);
}

constexpr String::const_iterator String::const_iterator::operator++() noexcept
{
    const_iterator tmp = *this;

    if ((++m_pos == (m_current_string->m_str.end() + String::kEndOfStringOffset)) && m_current_string->m_suffix)
    {
        m_current_string = m_current_string->m_suffix;
        m_pos            = m_current_string->m_str.begin();
    }

    return tmp;
}

constexpr String::const_iterator& String::const_iterator::operator++(int) noexcept
{
    if ((++m_pos == (m_current_string->m_str.end() + String::kEndOfStringOffset)) && m_current_string->m_suffix)
    {
        m_current_string = m_current_string->m_suffix;
        m_pos            = m_current_string->m_str.begin();
    }

    return *this;
}

constexpr String::const_iterator& String::const_iterator::operator=(const_iterator const& rhs) noexcept
{
    m_current_string = rhs.m_current_string;
    m_pos            = rhs.m_pos;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

inline std::ostream& operator<<(std::ostream& ostream, shmit::log::String const& str)
{
    for (char c : str)
        ostream.put(c);
    return ostream;
}

} // namespace log
} // namespace shmit