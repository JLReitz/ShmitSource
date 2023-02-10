#include <algorithm>
#include <array>
#include <cctype>
#include <charconv>
#include <climits>
#include <cstdio>
#include <limits>
#include <string_view>
#include <type_traits>

#include <iostream>

template<typename T, typename = void>
struct const_ref
{
    using type = std::add_const_t<std::add_lvalue_reference_t<T>>;
};

template<typename T>
using const_ref_t = typename const_ref<T>::type;

namespace detail
{
template<typename T>
struct max_digits_for_int
{
    static constexpr size_t binary {
        std::numeric_limits<T>::digits +
        ((std::numeric_limits<T>::is_signed &&
          std::numeric_limits<T>::digits == std::numeric_limits<unsigned char>::digits * sizeof(T) - 1) ?
             1 :
             0)};
    static constexpr size_t octal {(binary + 2) / 3}; // Binary digits divided by 3, rounded up
    static constexpr size_t decimal {std::numeric_limits<T>::digits10};
    static constexpr size_t hexadecimal {(binary + 3) / 4};

    static constexpr size_t value = decimal;
};

template<typename T>
struct max_digits_for_float
{
private:
    static constexpr size_t kNumFloatFormatDigits {2}; // '.' and 'e'

public:
    using type = max_digits_for_float<T>;
    static constexpr size_t value {std::numeric_limits<T>::max_digits10 + kNumFloatFormatDigits};
};

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct max_digits_for_type
{
    static constexpr size_t value {
        std::conditional_t<std::is_floating_point_v<T>, max_digits_for_float<T>, max_digits_for_int<T>>::value};
};

template<typename T>
struct max_digits_for_type<T*, std::enable_if_t<std::is_pointer_v<T>>>
{
    static constexpr size_t value = 2 + sizeof(T*) * CHAR_BIT / 4;
};

template<>
struct max_digits_for_type<bool, void>
{
    static constexpr size_t value = 5; // "false" is longer than "true"
};

template<>
struct max_digits_for_type<char, void>
{
    static constexpr size_t value = 1;
};

template<typename T>
struct max_digits_for_signed_type
{
private:
    static constexpr size_t kNumSignDigits = 1; // '-' or '+'

public:
    static constexpr size_t value {max_digits_for_type<T>::value + kNumSignDigits};
};

} // namespace detail

template<typename T, typename = void>
struct max_digits_for_type
{
    static constexpr size_t value {(std::is_signed_v<T>) ? detail::max_digits_for_signed_type<T>::value :
                                                           detail::max_digits_for_type<T>::value};
};

template<typename T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline size_t Print(char* const str, size_t str_length, T value)
{
    constexpr size_t     kMaxPrintLength {max_digits_for_type<T>::value};
    std::to_chars_result result = std::to_chars(str, str + kMaxPrintLength, value, 10);
}

namespace base
{

template<typename T>
struct Binary
{
    static_assert(std::is_integral_v<T>, "T must be integer type");

    using type = T;

    static constexpr size_t           value {2};
    static constexpr std::string_view format_prefix {"0b"};
    static constexpr size_t           max_print_digits {detail::max_digits_for_int<T>::binary + format_prefix.length()};
};

template<typename T>
struct Octal
{
public:
    static_assert(std::is_integral_v<T>, "T must be integer type");

    using type = T;

    static constexpr size_t           value {8};
    static constexpr std::string_view format_prefix {"0o"};
    static constexpr size_t           max_print_digits {detail::max_digits_for_int<T>::octal + format_prefix.length()};
};

template<typename T>
struct Decimal
{
    static_assert(std::is_arithmetic_v<T>, "T must be fundamental arithmetic type");

    using type = T;

    static constexpr size_t           value {10};
    static constexpr std::string_view format_prefix {""};
    static constexpr size_t max_print_digits {detail::max_digits_for_int<T>::decimal + format_prefix.length()};
};

template<typename T>
struct Hexadecimal
{
    static_assert(std::is_integral_v<T>, "T must be integer type");

    using type = T;

    static constexpr size_t           value {16};
    static constexpr std::string_view format_prefix {"0x"};
    static constexpr size_t max_print_digits {detail::max_digits_for_int<T>::hexadecimal + format_prefix.length()};
};

template<typename T>
struct is_base : public std::false_type
{
    using type = is_base<T>;
};

template<typename T>
struct is_base<Binary<T>> : std::true_type
{
    using type = is_base<Binary<T>>;
};

template<typename T>
struct is_base<Octal<T>> : std::true_type
{
    using type = is_base<Octal<T>>;
};

template<typename T>
struct is_base<Decimal<T>> : std::true_type
{
    using type = is_base<Decimal<T>>;
};

template<typename T>
struct is_base<Hexadecimal<T>> : std::true_type
{
    using type = is_base<Hexadecimal<T>>;
};

template<typename T>
inline constexpr bool is_base_v {is_base<T>::value};

} // namespace base

template<typename T>
struct const_ref<T, std::enable_if<base::is_base_v<T>>>
{
    using type = std::add_const_t<std::add_lvalue_reference_t<typename T::type>>;
};

template<typename T>
struct max_digits_for_type<T, std::enable_if_t<base::is_base_v<T>>>
{
    static constexpr size_t value {T::max_print_digits};
};

// template<typename T>
// struct max_digits_for_type<base::Octal<T>>
// {
//     static_assert(std::is_integral_v<T>, "base::Octal<T> requires T to be an integer type");
//     static constexpr size_t value {detail::max_digits_for_int<T>::octal + base::Octal<T>::format_prefix.length()};
// };

// template<typename T>
// struct max_digits_for_type<base::Decimal<T>>
// {
//     static constexpr size_t value {detail::max_digits_for_type<T>::value + base::Decimal<T>::format_prefix.length()};
// };

// template<typename T>
// struct max_digits_for_type<base::Hexadecimal<T>>
// {
//     static_assert(std::is_integral_v<T>, "base::Hexadecimal<T> requires T to be an integer type");
//     static constexpr size_t value {detail::max_digits_for_int<T>::hexadecimal +
//                                    base::Hexadecimal<T>::format_prefix.length()};
// };

template<typename... Argv>
struct sum_max_digits_for_types
{
    static constexpr size_t value = (0 + ... + max_digits_for_type<Argv>::value);
};

inline bool is_alpha(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isalpha(c); };
    return std::all_of(str.begin(), str.end(), check);
}

inline bool is_alphanumeric(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isalnum(c); };
    return std::all_of(str.begin(), str.end(), check);
}

inline bool is_digit(std::string_view str)
{
    auto check = [](char c) -> bool { return std::isdigit(c); };
    return std::all_of(str.begin(), str.end(), check);
}

struct Entry
{
    enum class Type : uint8_t
    {
        eInvalid = 0,

        eArithmetic,
        ePointer,
        eCstring,
        eRawString
    };

    Type const   type {Type::eInvalid};
    size_t const max_size {0};
    size_t const integer_base {10};
};

class RawString
{
public:
    class Iterator
    {
    public:
        using difference_type = int;
        using size_type       = std::string_view::size_type;

    private:
        size_type m_offset;
    };

    using difference_type = Iterator::difference_type;
    using size_type       = Iterator::size_type;

    RawString(std::string_view str) noexcept : m_raw {str}
    {
    }

protected:
    RawString(char* replacement_buffer) noexcept : m_replacements {replacement_buffer}
    {
    }

    RawString(std::string_view str, char* replacement_buffer) noexcept
        : m_raw {str}, m_replacements {replacement_buffer}
    {
    }

    std::string_view m_raw {};
    char*            m_replacements {nullptr};
};

template<typename T>
constexpr Entry::Type get_entry_type()
{
    static_assert((base::is_base_v<T> || std::is_arithmetic_v<T> || std::is_pointer_v<T> || std::is_same_v<T, RawString>),
                  "T must be a fundamental arithmetic type, base representation of an arithmetic type, pointer, or "
                  "RawString");

    if constexpr (base::is_base_v<T> || std::is_arithmetic_v<T>)
        return Entry::Type::eArithmetic;
    else if constexpr (std::is_pointer_v<T>)
    {
        // Need to determine whether the argument is a c-string or regular pointer
        if constexpr (std::is_same_v<std::remove_cv_t<T>, char*>)
            return Entry::Type::eCstring;
        else
            return Entry::Type::ePointer;
    }
    else if constexpr (std::is_same_v<T, RawString>)
        return Entry::Type::eRawString;

    // Should not get here, static_assert will be encountered above
    return Entry::Type::eInvalid;
}

template<typename T, typename = void>
struct generate_entry
{
    static constexpr Entry value {get_entry_type<T>(), max_digits_for_type<T>::value};
};

template<typename T>
struct generate_entry<T, std::enable_if_t<base::is_base_v<T>>>
{
    static constexpr Entry value {get_entry_type<T>(), max_digits_for_type<T>::value, T::value};
};

template<typename... Argv>
class FormatString : public RawString
{
public:
    static constexpr size_t kMaxSubstitutionTokenLength {3}; // "{<argument index>}"
    static constexpr size_t kNumArguments {sizeof...(Argv)};
    static constexpr size_t kMaxSubstitutionBufferLength {sum_max_digits_for_types<Argv...>::value};

    static constexpr std::array<Entry, kNumArguments> kEntries {generate_entry<Argv>::value...};

    std::array<char, kMaxSubstitutionBufferLength> m_substitute_buffer {};

public:
    using difference_type = RawString::difference_type;
    using size_type       = RawString::size_type;

    static constexpr char kSubstituteStartToken {'{'};
    static constexpr char kSubstituteEndToken {'}'};

    FormatString() noexcept : RawString(m_substitute_buffer.data())
    {
    }

    FormatString(std::string_view str) noexcept : RawString(str, m_substitute_buffer.data())
    {
    }

    bool Process(const_ref_t<Argv>... args) noexcept
    {
        (size_t n = 0, ..., (n += Print(m_substitute_buffer.data() + n, m_substitute_buffer.length() - n, args)));
        return (n == kMaxSubstitutionBufferLength);
    }
};

using CountString = FormatString<int, double, char>;
using CString     = FormatString<>;
using ErrorString = FormatString<unsigned int, volatile double>;

int main()
{
    // static_assert(std::is_integral_v<float>, "Sanity check");
    // using hex = typename base::Hexadecimal<double>::type;

    char const* test = "12k}jdbsiub{}ashj";
    int         val  = std::atoi(test);

    std::cout << val << std::endl;

    bool is_signed = std::is_signed_v<bool>;
    std::cout << is_signed << std::endl;

    size_t signed_digits   = std::numeric_limits<int>::digits10;
    size_t unsigned_digits = std::numeric_limits<unsigned int>::digits10;
    size_t hex_digits      = max_digits_for_type<base::Hexadecimal<int>>::value;

    std::cout << signed_digits << " " << unsigned_digits << " " << hex_digits << std::endl;

    bool char_ptr_signed = std::is_signed_v<char*>;
    bool int_ptr_signed  = std::is_signed_v<int*>;

    std::cout << char_ptr_signed << " " << int_ptr_signed << std::endl;

    ErrorString err;
    for (auto entry : err.kEntries)
        std::cout << entry.max_size << " ";
    std::cout << std::endl;

    return 0;
}