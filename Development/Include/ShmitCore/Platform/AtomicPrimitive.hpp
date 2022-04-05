#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{
namespace platform
{
namespace atomic
{

/**
 * Atomic operations are split into seperate namespaces based on the size of the data primitives being accessed.
 * The same operations are available to each size.
 * 
 * The available operations are as follows:
 * 
 *  Load    -   Exclusive read from memory
 *  Store   -   Exclusive store to memory
 * 
 *  CompareAndSwap  -   Compares the value in memory to what is expected and stores the new value if they match.
 *                      Returns true if swap was successful, false if not
 *  FetchAndAdd     -   Adds to the value in memory and returns the new value
 */

// 8 bit values
namespace b8
{

    uint8_t Load(uint8_t* addr);
    bool Store(uint8_t* addr, uint8_t store);

    bool CompareAndSwap(uint8_t* addr, uint8_t oldValue, uint8_t newValue);
    uint8_t FetchAndAdd(uint8_t* addr, uint8_t add);

} // End namespace b8

// 16 bit values
namespace b16
{

    uint16_t Load(uint16_t* addr);
    bool Store(uint16_t* addr, uint16_t store);

    bool CompareAndSwap(uint16_t* addr, uint16_t oldValue, uint16_t newValue);
    uint16_t FetchAndAdd(uint16_t* addr, uint16_t add);

} // End namespace b16

// 32 bit values
namespace b32
{

    uint32_t Load(uint32_t* addr);
    bool Store(uint32_t* addr, uint32_t store);

    bool CompareAndSwap(uint32_t* addr, uint32_t oldValue, uint32_t newValue);
    uint32_t FetchAndAdd(uint32_t* addr, uint32_t add);

} // End namespace b32

// 64 bit values
namespace b64
{
    uint64_t Load(uint64_t* addr);
    bool Store(uint64_t* addr, uint64_t store);

    bool CompareAndSwap(uint64_t* addr, uint64_t oldValue, uint64_t newValue);
    uint64_t FetchAndAdd(uint64_t* addr, uint64_t add);

} // End namespace b64

}
}
}