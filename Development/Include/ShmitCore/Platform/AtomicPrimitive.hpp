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
 * The same operations are available to each size defined within shmit::size::Primitive.
 */

// 8 bit values
namespace b8
{

uint8_t Load(uint8_t* addr);
bool Store(uint8_t* addr, uint8_t store);

bool CompareAndSwap(uint8_t* addr, uint8_t compare, uint8_t swap);
uint8_t FetchAndAdd(uint8_t* addr, uint8_t add);

} // End namespace b8

// 16 bit values
namespace b16
{

uint16_t Load(uint16_t* addr);
bool Store(uint16_t* addr, uint16_t store);

bool CompareAndSwap(uint16_t* addr, uint16_t compare, uint16_t swap);
uint16_t FetchAndAdd(uint16_t* addr, uint16_t add);

} // End namespace b16

// 32 bit values
namespace b32
{

uint32_t Load(uint32_t* addr);
bool Store(uint32_t* addr, uint32_t store);

bool CompareAndSwap(uint32_t* addr, uint32_t compare, uint32_t swap);
uint32_t FetchAndAdd(uint32_t* addr, uint32_t add);

} // End namespace b32

// 64 bit values
namespace b64
{

uint64_t Load(uint64_t* addr);
bool Store(uint64_t* addr, uint64_t store);

bool CompareAndSwap(uint64_t* addr, uint64_t compare, uint64_t swap);
uint64_t FetchAndAdd(uint64_t* addr, uint64_t add);

} // End namespace b64

}
}
}