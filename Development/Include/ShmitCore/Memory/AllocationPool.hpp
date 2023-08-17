#pragma once

#include "MemoryPool.hpp"

namespace shmit
{

/**
 * @brief An interface for custom heap implementations
 */
class AllocationPool : public MemoryPool
{
public:

    template <typename T>
    T* Allocate(size_t iterations = 1);

    template <typename T>
    void Deallocate(T* address, size_t iterations = 1);

protected:

    /**
     * @brief Checks for available space within the pool
     * 
     * @param sizeInBytes Minimum hole size
     * @return MemoryAddress_t Available address if successful, nullptr if unsuccessful
     */
    virtual MemoryAddress_t CheckForAvailableSpace(size_t sizeInBytes) = 0;

    /**
     * @brief Pool-specific allocation procedure. Called once the available space is confirmed.
     * 
     * @param address Allocation address
     * @param sizeInBytes Length to reserve
     */
    virtual void PoolSpecificAllocate(MemoryAddress_t address, size_t sizeInBytes) = 0;

    /**
     * @brief Pool-specific deallocation procedure. Called once the address is confirmed to exist within the pool's 
     *        bounds.
     * 
     * @param address Deallocation address
     * @param sizeInBytes Length to free
     */
    virtual void PoolSpecificDeallocate(MemoryAddress_t address, size_t sizeInBytes) = 0;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Top-level allocation method for all AllocationPool derivatives
 * 
 * @tparam T Type to allocate
 * @param iterations Number of iterations to allocate (default is 1)
 * @return T* Address of new object if successful, nullptr if unsuccessful
 */
template <typename T>
T* AllocationPool::Allocate<T>(size_t iterations)
{
    size_t lengthToAllocateInBytes = sizeof(T) * ((iterations == 0) ? 1 : iterations); // Guard against iterations == 0
    MemoryAddress_t allocationAddress = CheckForAvailableSpace(lengthToAllocateInBytes);

    // If the returned address is not null, allocate
    if(allocationAddress)
    {
        PoolSpecificAllocate(allocationAddress, lengthToAllocateInBytes);
    }

    return allocationAddress;
}

/**
 * @brief Top-level deallocation method for all AllocationPool derivatives
 * 
 * @tparam T Type to deallocate
 * @param address Address to deallocate
 * @param iterations Number of iterations to deallocate (default is 1)
 */
template <typename T>
void AllocationPool::Deallocate<T>(T* address, size_t iterations)
{
    // Check for existence of the provided object within this pool
    if (IsWithin(address))
    {
        // If it does, determine the total length (including all iterations) of space to free and pull the trigger
        size_t lengthToDeallocateInBytes = sizeof(T) * ((iterations == 0) ? 1 : iterations); 
        // ^ Guard against iterations == 0
        PoolSpecificDeallocate(address, lengthToDeallocateInBytes);
    }
}

}