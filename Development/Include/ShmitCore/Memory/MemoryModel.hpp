#pragma once

#include <ShmitCore/StdTypes.hpp>

#include <ShmitCore/Context
/Context.hpp>
#include <ShmitCore/Types/TypeHelp.hpp>

namespace shmit
{

/**
 * @brief Provides architectural support for a section of memory
 */
class MemoryModel : public Context
{
public:
    // TODO move IsPrimitive(size_t) here
    template<typename T>
    bool IsPrimitive();

    virtual shmit::size::Primitive LargestPrimitive() const  = 0;
    virtual shmit::size::Primitive SmallestPrimitive() const = 0;

protected:
    virtual bool CheckIsPrimitive(size_t sizeInBytes) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
bool MemoryModel::IsPrimitive()
{
    size_t typeSizeInBytes = shmit::help::TypeSizeInBytes<T>();
    return CheckIsPrimitive(typeSizeInBytes);
}

} // namespace shmit