#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{
namespace size
{

/**
 * @brief Represents the size of a primitive data type (byte/word aligned)
 */
enum Primitive
{
    e8Bits  =   8,
    e16Bits =   16,
    e32Bits =   32,
    e64Bits =   64
};

} // End namespace size
}