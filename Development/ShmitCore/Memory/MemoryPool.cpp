#include <ShmitCore/Memory/MemoryPool.hpp>

namespace shmit
{

/**
 * @brief Construct a new MemoryPool object
 *
 * @param startAddr Start address of the pool
 * @param poolSize Size of the pool
 */
MemoryPool::MemoryPool(MemoryAddress_t startAddr, size_t poolSize)
    : mStartAddr(startAddr), mEndAddr((char*)startAddr + poolSize)
{
}

/**
 * @brief Construct a new MemoryPool object
 *
 * @param startAddr Start address of the pool
 * @param endAddr End address of the pool
 */
MemoryPool::MemoryPool(MemoryAddress_t startAddr, MemoryAddress_t endAddr) : mStartAddr(startAddr), mEndAddr(endAddr)
{
}

MemoryPool::~MemoryPool()
{
    // Nothing to do for now
}

/**
 * @brief Checks whether the provided address is within the pool
 *
 * @param addr Address in memory
 * @return true if the address exists within the pool, false if not
 */
bool MemoryPool::IsWithin(MemoryAddress_t addr) const
{
    bool isWithin = (addr >= mStartAddr && addr <= mEndAddr);
    return isWithin;
}

/**
 * @brief Get the starting address of the pool
 *
 * @return MemoryAddress_t
 */
MemoryAddress_t MemoryPool::StartAddress() const
{
    return mStartAddr;
}

/**
 * @brief Get the end address of the pool
 *
 * @return MemoryAddress_t
 */
MemoryAddress_t MemoryPool::EndAddress() const
{
    return mEndAddr;
}

/**
 * @brief Get the size of the pool
 *
 * @return size_t
 */
size_t MemoryPool::Size() const
{
    size_t poolSize = (uint64_t)mEndAddr - (uint64_t)mStartAddr;
    return poolSize;
}

} // namespace shmit