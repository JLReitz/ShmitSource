#include <string_view>
#include <type_traits>

namespace shmit
{

using StringConstantId = uint16_t;

template<typename CharT, CharT... Chars>
extern StringConstantId string_constant_id();

template<typename CharT, CharT... Chars>
struct StringConstant
{
private:
    constexpr static size_t kDataSize {sizeof...(Chars)};
    constexpr static CharT  kData[kDataSize] {Chars...};

public:
    using type = StringConstant<CharT, Chars...>;
    using rep  = std::basic_string_view<CharT>;

    constexpr static rep value {kData, kDataSize};

    static StringConstantId const kId {string_constant_id<CharT, Chars...>()};

    constexpr operator rep() const noexcept
    {
        return value;
    }

    constexpr rep operator()() const noexcept
    {
        return value;
    }
};

template<typename CharT, CharT... Chars>
constexpr StringConstant<CharT, Chars...> operator""_ssc()
{
    return {};
}

template<typename T>
struct is_string_constant : public std::false_type
{
    using type = typename std::false_type::type;
};

template<typename CharT, CharT... Chars>
struct is_string_constant<StringConstant<CharT, Chars...>> : public std::true_type
{
    using type = typename std::true_type::type;
};

template<typename T>
using is_string_constant_t = typename is_string_constant<T>::type;

template<typename T>
constexpr inline bool is_string_constant_v = is_string_constant<T>::value;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Commonly used StringConstants               ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

using NullStringConstant = StringConstant<char, '\0'>;

} // namespace shmit