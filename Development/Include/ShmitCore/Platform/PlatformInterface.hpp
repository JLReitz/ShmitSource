#pragma once

#include "PlatformDefines.hpp"

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

    virtual uint8_t AtomicLoad8Bits(uint8_t* addr) const = 0;
    virtual uint16_t AtomicLoad16Bits(uint16_t* addr) const = 0;
    virtual uint32_t AtomicLoad32Bits(uint32_t* addr) const = 0;
    virtual uint64_t AtomicLoad64Bits(uint64_t* addr) const = 0;

protected:

    PlatformInterface(const PlatformConfiguration& platformConfig);

    const PlatformConfiguration& mPlatformConfig;
};

}
}