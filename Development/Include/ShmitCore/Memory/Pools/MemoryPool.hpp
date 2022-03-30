#pragma once

#include <ShmitCore/StdIncludes.hpp>
#include <ShmitCore/Types/Memory/MemoryTypes.hpp>

namespace shmit
{
namespace memory
{

class MemoryPool
{
public:

    MemoryPool();
    MemoryPool(MemoryAddress_t startAddr, size_t poolSizeInBytes);
    MemoryPool(MemoryAddress_t startAddr, MemoryAddress_t endAddr);

    virtual ~MemoryPool();

    bool IsWithin(MemoryAddress_t check) const;

    MemoryAddress_t StartAddress() const;
    MemoryAddress_t EndAddress() const;

    size_t Size() const;

protected:

    MemoryAddress_t mStartAddress;
    size_t mPoolSizeInUnitsOfMemory;
};

}
}