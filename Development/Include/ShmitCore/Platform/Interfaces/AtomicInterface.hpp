#pragma once

#include <ShmitCore/Types/Memory/MemoryTypes.hpp>
#include <ShmitCore/Types/StdTypes.hpp>

namespace shmit
{
namespace platform
{

class AtomicInterface
{
public:

    virtual uint64_t AtomicLoad(shmit::size::Primitive size, shmit::MemoryAddress_t addr) const = 0;
    virtual bool AtomicStore(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t store) = 0;

    virtual bool AtomicCompareAndSwap(shmit::size::Primitive size, shmit::MemoryAddress_t addr, 
                                      uint64_t compare, uint64_t swap) = 0;
    virtual uint64_t AtomicFetchAndAdd(shmit::size::Primitive size, shmit::MemoryAddress_t addr, uint64_t add) = 0;
    virtual uint64_t AtomicFetchAndSubtract(shmit::size::Primitive size, shmit::MemoryAddress_t addr, 
                                            uint64_t sub) = 0;
};

}
}