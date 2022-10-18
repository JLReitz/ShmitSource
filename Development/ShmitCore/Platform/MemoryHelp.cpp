#include <ShmitCore/Platform/MemoryHelp.hpp>

#include <ShmitCore/Platform/Interfaces/MemoryInterface.hpp>

#include <list>
#include <vector>

namespace shmit
{
namespace platform
{
namespace memory
{

extern MemoryInterface* gStaticPlatformInterface;

shmit::MemoryModel* GetMemoryModel(shmit::MemoryAddress_t addr)
{
    // Check against uninitialized platform interface
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return nullptr;
    }

    return gStaticPlatformInterface->FindMemoryModelForAddress(addr);
}

shmit::MemoryPool* GetMemoryPool(shmit::MemoryAddress_t addr)
{
    // Check against uninitialized platform interface
    if (!gStaticPlatformInterface)
    {
        // TODO throw exception and log error
        return nullptr;
    }

    return gStaticPlatformInterface->FindMemoryPoolForAddress(addr);
}

}
}
}