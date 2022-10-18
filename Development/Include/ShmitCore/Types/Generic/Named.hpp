#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <type_traits>

namespace shmit
{

class Named
{
public:
    static constexpr size_t kMaxSize = 16;

    Named();
    Named(char const* name);

    char const* GetName() const;
    void        SetName(char const* name);

    size_t NameLength() const;

private:
    size_t m_name_length;
    char   m_name[kMaxSize + 1]; // +1 for null termination
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