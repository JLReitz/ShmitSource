#include <ShmitCore/Platform/Interfaces/AtomicInterface.hpp>
#include <ShmitCore/Platform/PrimitiveAtomic.hpp>

namespace shmit
{
namespace platform
{
namespace atomic
{

extern AtomicInterface* gStaticPlatformInterface;

/**
 * @brief Exclusively compares a value in memory to what is expected and stores a new value if they match
 * 
 * @param size Size of accessed primitive data type, must be defined by enum shmit::size::Primitive
 * @param addr Address of the data
 * @param compare Old value to compare against (max 64 bits)
 * @param swap New value to store if compare is successful (max 64 bits)
 * @return true if successful, false if unsuccessful
 */
bool CompareAndSwap(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t compare, uint64_t swap)
{
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return false;
    }

    return gStaticPlatformInterface->AtomicCompareAndSwap(size, addr, compare, swap);
}

/**
 * @brief Exclusively adds to a value in memory and returns the result
 * 
 * @param size Size of accessed primitive data type, must be defined by enum shmit::size::Primitive
 * @param addr Address of the data
 * @param add Value to add (max 64 bits)
 * @return uint64_t Maximum 64 bits returned
 */
uint64_t FetchAndAdd(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t add)
{
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndAdd(size, addr, add);
}

/**
 * @brief Exclusively subtracts to a value in memory and returns the result
 * 
 * @param size Size of accessed primitive data type, must be defined by enum shmit::size::Primitive
 * @param addr Address of the data
 * @param sub Value to subtract (max 64 bits)
 * @return uint64_t 
 */
uint64_t FetchAndSubtract(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t sub)
{
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndSubtract(size, addr, sub);
}

/**
 * @brief Exclusive read from memory
 * 
 * @param size Size of accessed primitive data type, must be defined by enum shmit::size::Primitive
 * @param addr Address of the data
 * @return uint64_t Maximum 64 bits returned
 */
uint64_t Load(shmit::size::Primitive size, shmit::MemoryAddress_t addr)
{
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return 0;
    }

    return gStaticPlatformInterface->AtomicLoad(size, addr);
}

/**
 * @brief Exclusive store to memory
 * 
 * @param size Size of accessed primitive data type, must be defined by enum shmit::size::Primitive
 * @param addr Address of the data
 * @param store Value to store (max 64 bits)
 * @return true if successful, false if unsuccessful
 */
bool Store(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t store)
{
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return false;
    }

    return gStaticPlatformInterface->AtomicStore(size, addr, store);
}

}
}
}