#pragma once

#include <ostream>
#include <string_view>
#include <type_traits>

namespace shmit
{
namespace log
{

class String
{
public:
    class iterator
    {
    public:
        iterator() = delete;

        constexpr iterator(iterator const& rhs) noexcept;
        constexpr iterator(iterator&& rhs) noexcept;

        constexpr char operator*() const noexcept;

        constexpr bool operator==(iterator const& rhs) const noexcept;
        constexpr bool operator!=(iterator const& rhs) const noexcept;

        constexpr bool operator>(iterator const& rhs) const noexcept;
        constexpr bool operator>=(iterator const& rhs) const noexcept;
        constexpr bool operator<(iterator const& rhs) const noexcept;
        constexpr bool operator<=(iterator const& rhs) const noexcept;

        constexpr iterator  operator++() noexcept;
        constexpr iterator& operator++(int) noexcept;

        constexpr iterator& operator=(iterator const& rhs) noexcept;

        friend String;

    private:
        constexpr iterator(String const* str, bool end = false) noexcept;

        String const*              m_current_string {nullptr};
        std::string_view::iterator m_pos {nullptr};
    };

    class const_iterator
    {
    public:
        const_iterator() = delete;

        constexpr const_iterator(iterator const& rhs) noexcept;

        constexpr const_iterator(const_iterator const& rhs) noexcept;
        constexpr const_iterator(const_iterator&& rhs) noexcept;

        constexpr char const operator*() const noexcept;

        constexpr bool operator==(const_iterator const& rhs) const noexcept;
        constexpr bool operator!=(const_iterator const& rhs) const noexcept;

        constexpr bool operator>(const_iterator const& rhs) const noexcept;
        constexpr bool operator>=(const_iterator const& rhs) const noexcept;
        constexpr bool operator<(const_iterator const& rhs) const noexcept;
        constexpr bool operator<=(const_iterator const& rhs) const noexcept;

        constexpr const_iterator  operator++() noexcept;
        constexpr const_iterator& operator++(int) noexcept;

        constexpr const_iterator& operator=(const_iterator const& rhs) noexcept;

        friend String;

    private:
        constexpr const_iterator(String const* str, bool end = false) noexcept;

        String const*              m_current_string;
        std::string_view::iterator m_pos {nullptr};
    };

    using value_type      = char;
    using pointer         = value_type*;
    using const_pointer   = const pointer;
    using reference       = char&;
    using const_reference = const reference;

    constexpr String() = default;

    constexpr String(char const* str) noexcept;
    constexpr String(char const* str, size_t str_length) noexcept;
    constexpr String(std::string_view const& str) noexcept;

    constexpr String(String const& rhs) noexcept;
    constexpr String(String&& rhs) noexcept;

    constexpr iterator       begin() const noexcept;
    constexpr const_iterator cbegin() const noexcept;

    constexpr iterator       end() const noexcept;
    constexpr const_iterator cend() const noexcept;

    constexpr size_t size() const noexcept;

    constexpr void Append(String& str) noexcept;

    constexpr String& operator=(String const& rhs) noexcept;

    friend iterator;
    friend const_iterator;

    friend std::ostream& operator<<(std::ostream& os, String const& str);

private:
    constexpr String* FinalSuffix() const noexcept;

    static constexpr char kDelimiter = ',';
    static constexpr char kNewLine   = '\n';

    static constexpr size_t kEndOfStringOffset = 1;

    std::string_view m_str {};
    String const*    m_suffix {nullptr};
};

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
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}
{
}

constexpr String::iterator::iterator(iterator&& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}
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

    if ((++m_pos == (m_current_string->m_str.end() + String::kEndOfStringOffset)) && m_current_string->m_suffix)
    {
        m_current_string = m_current_string->m_suffix;
        m_pos            = m_current_string->m_str.begin();
    }

    return tmp;
}

constexpr String::iterator& String::iterator::operator++(int) noexcept
{
    if ((++m_pos == m_current_string->m_str.end() + String::kEndOfStringOffset) && m_current_string->m_suffix)
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
//  String::const_iterator constructor definitions ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

constexpr String::const_iterator::const_iterator(iterator const& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}
{
}

constexpr String::const_iterator::const_iterator(const_iterator const& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}
{
}

constexpr String::const_iterator::const_iterator(const_iterator&& rhs) noexcept
    : m_current_string {rhs.m_current_string}, m_pos {rhs.m_pos}
{
}

//  Private     ========================================================================================================

constexpr String::const_iterator::const_iterator(String const* str, bool end) noexcept
    : m_current_string {str}, m_pos {(end) ? str->m_str.end() + String::kEndOfStringOffset : str->m_str.begin()}
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  String::const_iterator method definitions in alphabetical order ////////////////////////////////////////////
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

inline std::ostream& operator<<(std::ostream& ostream, shmit::log::String const& str)
{
    for (char c : str)
        ostream.put(c);
    return ostream;
}

} // namespace log
} // namespace shmit
