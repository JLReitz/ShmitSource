#include <ShmitCore/Platform/PlatformInterface.hpp>
#include <ShmitCore/Platform/AtomicPrimitive.hpp>

namespace shmit
{
namespace platform
{

/**
 * @brief Exclusively compares a value in memory to what is expected and stores a new value if they match
 * 
 * @param addr Address of 8 bit data type
 * @param compare Old value to compare against
 * @param swap New value to store if compare is successful
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b8::CompareAndSwap(uint8_t* addr, uint8_t compare, uint8_t swap)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicCompareAndSwap(shmit::size::e8Bits, addr, compare, swap);
}

/**
 * @brief Exclusively compares a value in memory to what is expected and stores a new value if they match
 * 
 * @param addr Address of 16 bit data type
 * @param compare Old value to compare against
 * @param swap New value to store if compare is successful
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b16::CompareAndSwap(uint16_t* addr, uint16_t compare, uint16_t swap)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicCompareAndSwap(shmit::size::e16Bits, addr, compare, swap);
}

/**
 * @brief Exclusively compares a value in memory to what is expected and stores a new value if they match
 * 
 * @param addr Address of 32 bit data type
 * @param compare Old value to compare against
 * @param swap New value to store if compare is successful
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b32::CompareAndSwap(uint32_t* addr, uint32_t compare, uint32_t swap)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicCompareAndSwap(shmit::size::e32Bits, addr, compare, swap);
}

/**
 * @brief Exclusively compares a value in memory to what is expected and stores a new value if they match
 * 
 * @param addr Address of 8 bit data type
 * @param compare Old value to compare against
 * @param swap New value to store if compare is successful
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b64::CompareAndSwap(uint64_t* addr, uint64_t compare, uint64_t swap)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicCompareAndSwap(shmit::size::e64Bits, addr, compare, swap);
}

/**
 * @brief Exclusively adds to a value in memory and returns the result
 * 
 * @param addr Address of 8 bit data type
 * @param add Value to add
 * @return uint8_t 
 */
uint8_t shmit::platform::atomic::b8::FetchAndAdd(uint8_t* addr, uint8_t add)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndAdd(shmit::size::e8Bits, addr, add);
}

/**
 * @brief Exclusively adds to a value in memory and returns the result
 * 
 * @param addr Address of 16 bit data type
 * @param add Value to add
 * @return uint16_t 
 */
uint16_t shmit::platform::atomic::b16::FetchAndAdd(uint16_t* addr, uint16_t add)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndAdd(shmit::size::e16Bits, addr, add);
}

/**
 * @brief Exclusively adds to a value in memory and returns the result
 * 
 * @param addr Address of 32 bit data type
 * @param add Value to add
 * @return uint32_t 
 */
uint32_t shmit::platform::atomic::b32::FetchAndAdd(uint32_t* addr, uint32_t add)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndAdd(shmit::size::e32Bits, addr, add);
}

/**
 * @brief Exclusively adds to a value in memory and returns the result
 * 
 * @param addr Address of 64 bit data type
 * @param add Value to add
 * @return uint64_t 
 */
uint64_t shmit::platform::atomic::b64::FetchAndAdd(uint64_t* addr, uint64_t add)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicFetchAndAdd(shmit::size::e64Bits, addr, add);
}

/**
 * @brief Exclusive read from memory
 * 
 * @param addr Address of 8 bit data type
 * @return uint8_t 
 */
uint8_t shmit::platform::atomic::b8::Load(uint8_t* addr)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicLoad(shmit::size::e8Bits, addr);
}

/**
 * @brief Exclusive read from memory
 * 
 * @param addr Address of 16 bit data type
 * @return uint16_t 
 */
uint16_t shmit::platform::atomic::b16::Load(uint16_t* addr)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicLoad(shmit::size::e16Bits, addr);
}

/**
 * @brief Exclusive read from memory
 * 
 * @param addr Address of 32 bit data type
 * @return uint32_t 
 */
uint32_t shmit::platform::atomic::b32::Load(uint32_t* addr)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicLoad(shmit::size::e32Bits, addr);
}

/**
 * @brief Exclusive read from memory
 * 
 * @param addr Address of 64 bit data type
 * @return uint64_t 
 */
uint64_t shmit::platform::atomic::b64::Load(uint64_t* addr)
{
    if (!gStaticPlatformInterface)
    {
        return 0;
    }

    return gStaticPlatformInterface->AtomicLoad(shmit::size::e64Bits, addr);
}

/**
 * @brief Exclusive store to memory
 * 
 * @param addr Address of 8 bit data type
 * @param store Value to store
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b8::Store(uint8_t* addr, uint8_t store)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicStore(shmit::size::e8Bits, addr, store);
}

/**
 * @brief Exclusive store to memory
 * 
 * @param addr Address of 16 bit data type
 * @param store Value to store
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b16::Store(uint16_t* addr, uint16_t store)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicStore(shmit::size::e16Bits, addr, store);
}

/**
 * @brief Exclusive store to memory
 * 
 * @param addr Address of 32 bit data type
 * @param store Value to store
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b32::Store(uint32_t* addr, uint32_t store)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicStore(shmit::size::e32Bits, addr, store);
}

/**
 * @brief Exclusive store to memory
 * 
 * @param addr Address of 64 bit data type
 * @param store Value to store
 * @return true if successful, false if unsuccessful
 */
bool shmit::platform::atomic::b64::Store(uint64_t* addr, uint64_t store)
{
    if (!gStaticPlatformInterface)
    {
        return false;
    }

    return gStaticPlatformInterface->AtomicStore(shmit::size::e64Bits, addr, store);
}

}
}