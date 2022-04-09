#pragma once

#include <ShmitCore/Types/Memory/MemoryModel.hpp>
#include <ShmitCore/Types/Memory/MemoryPool.hpp>

namespace shmit
{
namespace platform
{
namespace memory
{

/**
 * @brief Get the memory model which presides over the given address
 * 
 * @param addr Address in memory
 * @return shmit::MemoryModel* 
 */
shmit::MemoryModel* GetMemoryModel(shmit::MemoryAddress_t addr);

/**
 * @brief Get the memory section handler for the given address
 * 
 * @param addr Address in memory
 * @return shmit::MemoryModel* 
 */
shmit::MemoryPool* GetMemoryPool(shmit::MemoryAddress_t addr);

}
}
}