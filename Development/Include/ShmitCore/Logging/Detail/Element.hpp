#pragma once

#include <ShmitCore/Help/Data.hpp>
#include <ShmitCore/StringConstant.hpp>

#include <tuple>

namespace shmit
{
namespace log
{

namespace detail
{

struct ElementHeader
{
    StringConstantId tag_id;
    StringConstantId string_id;
};

constexpr static size_t kElementHeaderSize {help::DataSize_v<ElementHeader>};

template<typename TagT, typename StringT, typename... ArgsT>
struct Element
{
    static_assert(is_string_constant_v<TagT>, "shmit::log::detail::Element TagT parameter must be specialization of "
                                              "shmit::StringConstant");
    static_assert(is_string_constant_v<StringT>, "shmit::log::detail::Element StringT parameter must be specialization "
                                                 "of shmit::StringConstant");

    using type   = Element<TagT, StringT, ArgsT>;
    using Tag    = TagT;
    using String = StringT;
    using Data   = std::tuple<ArgsT...>;

    static StringConstantId const kTagId {Tag::kId};
    static StringConstantId const kStringId {String::kId};

    constexpr static size_t kNumArgs {sizeof...(ArgsT)};
    constexpr static size_t kDataSize {help::DataSize_v<ArgsT...>};
    Data                    data;

    Element(ArgsT... args);

    size_t Encode(uint8_t* data_start);

private:
    template<size_t... Indices>
    size_t EncodeData(uint8_t* data_start, std::index_sequence<Indices...>);
};

template<typename T>
struct is_element : public std::false_type
{
    using type = std::false_type;
};

template<typename TagT, typename StringT, typename... ArgsT>
struct is_element<Element<TagT, StringT, ArgsT...>> : public std::true_type
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

template<typename TagT, typename StringT, typename... ArgsT>
detail::Element<TagT, StringT, ArgsT...>::Element(ArgsT... args) : data {std::tie(args...)}
{
}

template<typename TagT, typename StringT, typename... ArgsT>
size_t detail::Element<TagT, StringT, ArgsT...>::Encode(uint8_t* data_start)
{
    ElementHeader* header {reinterpret_cast<ElementHeader*>(data_start)};
    header->tag_id    = kTagId;
    header->string_id = kStringId;

    size_t encoded_length {kElementHeaderSize +
                           EncodeData(data_start + kElementHeaderSize, std::index_sequence_for<ArgsT...>())};
    return encoded_length;
}

template<typename TagT, typename StringT, typename... ArgsT>
template<size_t... Indices>
size_t detail::Element<TagT, StringT, ArgsT...>::EncodeData(uint8_t* data_start, std::index_sequence<Indices...>)
{
    static_assert(kNumArgs == std::index_sequence<Indices...>::size(), "Number of arguments must match the size of "
                                                                       "their index sequence");

    size_t offset {0};
    (help::detail::DataCopy(data_start, std::get<Indices>(data), offset), ...);
    return offset;
}

} // namespace detail
} // namespace log
} // namespace shmit