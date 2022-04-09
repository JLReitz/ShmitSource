#pragma once

#include <ShmitCore/Types/Memory/MemoryTypes.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace platform
{
namespace atomic
{

/**
 * Atomic operations ensure thread-safe access and/or modifications are made to targeted primitive data types.
 * The following operations can only be used on data sizes defined by enum shmit::size::Primitive.
 */

uint64_t Load(shmit::size::Primitive size, shmit::MemoryAddress_t addr);
bool Store(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t store);

bool CompareAndSwap(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t compare, uint64_t swap);
uint64_t FetchAndAdd(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t add);
uint64_t FetchAndSubtract(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t sub);

}
}
}