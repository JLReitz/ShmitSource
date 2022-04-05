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

    size_t CharSizeInBytes() const;
    size_t ReferenceSizeInBytes() const;

    // Atomic operations
    virtual size_t AtomicPrimitiveMaxSizeInBytes() const = 0;

    virtual uint64_t AtomicLoad(shmit::PrimitiveSize_t size, shmit::memory::MemoryAddress_t addr) const = 0;
    virtual bool AtomicStore(shmit::PrimitiveSize_t size, shmit::memory::MemoryAddress_t addr, uint64_t store) = 0;

    virtual bool AtomicCompareAndSwap(shmit::PrimitiveSize_t size, shmit::memory::MemoryAddress_t addr, 
                                      uint64_t compare, uint64_t swap) = 0;
    virtual uint64_t FetchAndAdd(shmit::PrimitiveSize_t size, shmit::memory::MemoryAddress_t addr, uint64_t add) = 0;

protected:

    PlatformInterface(const PlatformConfiguration& platformConfig);

    const PlatformConfiguration& mPlatformConfig;
};

}
}