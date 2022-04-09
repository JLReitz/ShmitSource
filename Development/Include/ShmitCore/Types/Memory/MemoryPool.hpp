#pragma once

#include "MemoryTypes.hpp"

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{

class MemoryPool
{
public:

    MemoryPool(MemoryAddress_t startAddr, size_t poolSize);
    MemoryPool(MemoryAddress_t startAddr, MemoryAddress_t endAddr);

    virtual ~MemoryPool();

    bool IsWithin(MemoryAddress_t addr) const;

    MemoryAddress_t StartAddress() const;
    MemoryAddress_t EndAddress() const;

    size_t Size() const;

protected:

    MemoryAddress_t mStartAddress;
    MemoryAddress_t mEndAddress;
};

}