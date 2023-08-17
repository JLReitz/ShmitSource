#pragma once

#include <ShmitCore/Help/Data.hpp>
#include <ShmitCore/StringConstant.hpp>

#include <tuple>

namespace shmit
{
namespace log
{

using ElementId = uint16_t;

namespace detail
{

template<typename Tag, typename String, typename... Args>
struct Element
{
    using Data = std::tuple<Args...>;

    static StringConstantId const kTagId {Tag::kId};
    static StringConstantId const kStringId {String::kId};

    constexpr static size_t kNumArgs {sizeof...(Args)};
    constexpr static size_t kDataSize {help::DataSize_v<Args...>};
    Data                    data;

    Element(Args... args);

    void EncodeData(void* data_start);

private:
    template<size_t... Indices>
    void EncodeData(void* data_start, std::index_sequence<Indices...>);
};

template<typename T>
struct is_element : public std::false_type
{
    using type = std::false_type;
};

template<typename Tag, typename String, typename... Args>
struct is_element<Element<Tag, String, Args...>> : public std::true_type
{
    using type = std::true_type;
};

template<typename T>
using is_element_t = typename is_element<T>::type;

template<typename T>
constexpr inline bool is_element_v {is_element<T>::value};

template<typename T, typename Enable = void>
struct arg_type;

template<typename T>
struct arg_type<T, std::enable_if_t<std::is_arithmetic_v<T>>>
{
    using type = std::add_const_t<T>;
};

template<typename T>
struct arg_type<T, std::enable_if_t<std::is_reference_v<T>>>
{
private:
    using DataType = std::remove_cvref_t<T>;

public:
    using type = std::add_lvalue_reference_t<std::add_const_t<DataType>>;
};

template<typename T>
struct arg_type<T, std::enable_if_t<std::is_pointer_v<T>>>
{
private:
    using DataType = std::remove_const_t<std::remove_pointer_t<T>>;

    using StringType  = char const*;
    using PointerType = void* const;

public:
    using type = std::conditional_t<std::is_same_v<DataType, char>, StringType, PointerType>;
};

template<typename T>
using arg_type_t = typename arg_type<T>::type;

} // namespace detail

template<typename T>
using entry_arg_type_t = detail::arg_type_t<std::conditional_t<std::is_pointer_v<T>, T, std::add_lvalue_reference_t<T>>>;

template<typename Tag, typename String, typename... Args>
using Entry = detail::Element<Tag, String, entry_arg_type_t<Args...>>;

template<typename T>
using context_arg_type_t = detail::arg_type_t<std::remove_cvref_t<T>>;

template<typename Tag, typename String, typename... Args>
using Context = detail::Element<Tag, String, context_arg_type_t<Args...>>;

using NullContext = Context<NullStringConstant, NullStringConstant, void>;

template<typename Tag, typename String, typename... Args>
detail::Element<Tag, String, Args...>::Element(Args... args) : data {std::tie(args...)}
{
}

template<typename Tag, typename String, typename... Args>
void detail::Element<Tag, String, Args...>::EncodeData(void* data_start)
{
    EncodeData(data_start, std::index_sequence_for<Args...>());
}

template<typename Tag, typename String, typename... Args>
template<size_t... Indices>
void detail::Element<Tag, String, Args...>::EncodeData(void* data_start, std::index_sequence<Indices...>)
{
    static_assert(kNumArgs == std::index_sequence<Indices...>::size(), "Number of arguments must match the size of "
                                                                       "their index sequence");

    size_t offset {0};
    (help::detail::DataCopy(data_start, std::get<Indices>(data), offset), ...);
}

} // namespace log
} // namespace shmit