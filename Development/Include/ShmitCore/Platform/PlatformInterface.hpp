#pragma once

#include "PlatformDefines.hpp"

#include <ShmitCore/Types/Memory/MemoryTypes.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace platform
{

class PlatformInterface
{
public:

    // Atomic operations
    virtual shmit::size::Primitive MaxAtomicPrimitiveSizeInBytes() const = 0;
    virtual shmit::size::Primitive MinAtomicPrimitiveSizeInBytes() const = 0;

    virtual uint64_t AtomicLoad(shmit::size::Primitive size, shmit::memory::MemoryAddress_t addr) const = 0;
    virtual bool AtomicStore(shmit::size::Primitive size, shmit::memory::MemoryAddress_t addr, uint64_t store) = 0;

    virtual bool AtomicCompareAndSwap(shmit::size::Primitive size, shmit::memory::MemoryAddress_t addr, 
                                      uint64_t compare, uint64_t swap) = 0;
    virtual uint64_t AtomicFetchAndAdd(shmit::size::Primitive size, shmit::memory::MemoryAddress_t addr, 
                                       uint64_t add) = 0;
    virtual uint64_t AtomicFetchAndSubtract(shmit::size::Primitive size, shmit::memory::MemoryAddress_t addr, 
                                            uint64_t sub) = 0;

protected:

    PlatformInterface(const PlatformConfiguration& platformConfig);

    const PlatformConfiguration& mPlatformConfig;
};

}
}