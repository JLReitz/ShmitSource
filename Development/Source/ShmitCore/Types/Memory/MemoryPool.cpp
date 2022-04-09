#include <ShmitCore/Types/Memory/MemoryPool.hpp>

namespace shmit
{

MemoryPool::MemoryPool(MemoryAddress_t startAddress, size_t poolSize)
    : mStartAddress(startAddress), mEndAddress(startAddress + poolSize)
{
}

MemoryPool::MemoryPool(MemoryAddress_t startAddress, MemoryAddress_t endAddress)
    : mStartAddress(startAddress), mEndAddress(endAddress)
{
}

MemoryPool::~MemoryPool()
{   
    // Nothing to do for now
}

bool MemoryPool::IsWithin(MemoryAddress_t addr) const
{
    bool isWithin = (addr >= mStartAddress && addr <= mEndAddress);
    return isWithin;
}

MemoryAddress_t MemoryPool::StartAddress() const
{
    return mStartAddress;
}

MemoryAddress_t MemoryPool::EndAddress() const
{
    return mEndAddress;
}

size_t MemoryPool::Size() const
{
    size_t poolSize = (uint64_t)mEndAddress - (uint64_t)mStartAddress;
    return poolSize;
}

}