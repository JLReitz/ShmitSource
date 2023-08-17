
#pragma once

#include "Element.hpp"

namespace shmit
{
namespace log
{

using ContextId = uint16_t;

template<typename Name>
struct Context : public Element
{
    static_assert(is_string_constant_v<Name>, "shmit::log::Context Name parameter must be specialization of "
                                              "shmit::StringConstant");

    constexpr Context() = default;
    constexpr Context(StringConstantId subject_name_id);
};

template<typename T>
struct is_context : public std::false_type
{
    using type = std::false_type;
};

template<typename Name, typename Subject>
struct is_context<Context<Name, Subject>> : public std::true_type
{
    using type = std::true_type;
};

template<typename T>
using is_context_t = typename is_context<T>::type;

template<typename T>
constexpr inline bool is_context_v {is_context<T>::value};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Context constructor definitions                             ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename Name, typename Subject>
Context<Name, Subject>::Context(Subject const* subject)
{
    // If Subject is a class that extends shmit::Named and has a valid reference, cache its name ID
    if constexpr (kSubjectIsClass && is_named_v<Subject> && subject)
        if (subject)
            kSubjectNameId = subject->GetName();
        else
            kSubjectNameId = NullStringConstant::kId;
    else
    {
        // Otherwise the subject is not used, so a name is not available
        // Cache the ID for NullStringConstant instead
        (void)subject;
        kSubjectNameId = NullStringConstant::kId;
    }
}

} // namespace log
} // namespace shmit