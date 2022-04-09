#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{
namespace help
{

size_t CharSizeInBytes();
size_t ReferenceSizeInBytes();

template <typename T>
size_t TypeSizeInBytes()
{
    size_t typeSizeInMemUnits = sizeof(T);
    return typeSizeInMemUnits * CharSizeInBytes();
}

}
}