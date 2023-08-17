#pragma once

#include <ShmitCore/StringConstant.hpp>

namespace shmit
{
namespace log
{
namespace detail
{

template<typename Label>
struct Category
{
    static_assert(is_string_constant_v<Label>, "shmit::log::Category Label parameter must be specialization of "
                                               "shmit::StringConstant");

    static StringConstantId const kLabelId {Label::kId};
};

template<typename T>
struct is_category : public std::false_type
{
    using type = std::false_type;
};

template<typename Label>
struct is_category<Category<Label>> : public std::true_type
{
    using type = std::true_type;
};

template<typename T>
using is_category_t = typename is_category<T>::type;

template<typename T>
constexpr inline bool is_category_v {is_category<T>::value};

} // namespace detail
} // namespace log
} // namespace shmit