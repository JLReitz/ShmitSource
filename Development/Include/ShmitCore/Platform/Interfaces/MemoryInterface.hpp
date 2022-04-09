#pragma once

#include <ShmitCore/Types/Memory/MemoryModel.hpp>
#include <ShmitCore/Types/Memory/MemoryPool.hpp>

namespace shmit
{
namespace platform
{

/**
 * @brief Interface for platform memory information and operations. An implementation must exist for the runtime 
 *        environment in order for ShmitCore to function properly.
 */
class MemoryInterface
{
public:

    virtual shmit::MemoryModel* FindMemoryModelForAddress(shmit::MemoryAddress_t addr) = 0;
    virtual shmit::MemoryPool* FindMemoryPoolForAddress(shmit::MemoryAddress_t addr) = 0;
};

}
}