#pragma once

#include <ShmitCore/Types/Memory/MemoryModel.hpp>
#include <ShmitCore/Types/Memory/MemoryPool.hpp>

namespace shmit
{
namespace platform
{
namespace memory
{

shmit::MemoryModel* GetMemoryModel(shmit::MemoryAddress_t addr);
shmit::MemoryPool* GetMemoryPool(shmit::MemoryAddress_t addr);

}
}
}