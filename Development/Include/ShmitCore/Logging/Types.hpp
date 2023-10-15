#pragma once

#include "detail/Element.hpp"

namespace shmit
{
namespace log
{

namespace detail
{

template<typename T>
using entry_arg_type_t = detail::arg_type_t<std::conditional_t<std::is_pointer_v<T>, T, std::add_lvalue_reference_t<T>>>;

template<typename T>
using context_arg_type_t = detail::arg_type_t<std::remove_cvref_t<T>>;

} // namespace detail

enum class Severity : uint8_t
{
    eTrace   = 0,
    eDebug   = 1,
    eInfo    = 2,
    eWarning = 3,
    eError   = 4,
    eFatal   = 5
};

template<typename TagT, typename StringT, typename... ArgsT>
using Entry = detail::Element<TagT, StringT, detail::entry_arg_type_t<ArgsT...>>;

template<typename TagT, typename StringT, typename... ArgsT>
using Context = detail::Element<TagT, StringT, detail::context_arg_type_t<ArgsT...>>;

using NullContext = Context<NullStringConstant, NullStringConstant, void>;

} // namespace log
} // namespace shmit