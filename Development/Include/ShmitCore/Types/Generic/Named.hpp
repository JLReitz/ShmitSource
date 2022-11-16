#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <type_traits>

namespace shmit
{

class Named
{
public:
    static constexpr size_t kMaxSize = 32; //!< A name may be, at max, 32 characters long

    Named() = default;
    Named(char const* name);

    char const* GetName() const;
    void        SetName(char const* name);

    size_t NameLength() const;

private:
    size_t m_name_length {0};
    char   m_name[kMaxSize + 1] {0}; // +1 size for null termination
};

template<typename T>
struct IsNamed : std::false_type
{
};

template<>
struct IsNamed<Named> : std::true_type
{
};

} // namespace shmit