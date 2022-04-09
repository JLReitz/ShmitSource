#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <ShmitCore/Types/Contex.hpp>
#include <ShmitCore/Types/StdTypes.hpp>
#include <ShmitCore/Types/TypeHelp.hpp>

namespace shmit
{

class MemoryModel : public Context
{
public:

    // TODO move IsPrimitive(size_t) here
    template <typename T>
    bool IsPrimitive();

    virtual shmit::size::Primitive LargestPrimitive() const = 0;
    virtual shmit::size::Primitive SmallestPrimitive() const = 0;

protected:

    virtual bool CheckIsPrimitive(size_t sizeInBytes) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool MemoryModel::IsPrimitive()
{
    size_t typeSizeInBytes = shmit::help::TypeSizeInBytes<T>();
    return CheckIsPrimitive(typeSizeInBytes);
}

}