#include <ShmitCore/Named.hpp>

namespace shmit
{

Named::Named(char const* name) : m_name_length {0}, m_name {0}
{
    SetName(name);
}

char const* Named::GetName() const
{
    return m_name;
}

size_t Named::NameLength() const
{
    return m_name_length;
}

void Named::SetName(char const* name)
{
    if (name != nullptr)
    {
        size_t i = 0;
        while ((i < kMaxSize) && (name[i] != '\0'))
        {
            m_name[i] = name[i];
            i++;
        }

        m_name_length = i;
    }
}

} // namespace shmit