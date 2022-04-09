#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{
namespace help
{

size_t CharSizeInBytes();
size_t ReferenceSizeInBytes();

/**
 * @brief Returns the size of type T in bytes within this execution environment
 * 
 * @tparam T
 * @return size_t 
 */
template <typename T>
size_t TypeSizeInBytes()
{
    size_t typeSizeInMemUnits = sizeof(T);
    return typeSizeInMemUnits * CharSizeInBytes();
}

}
}