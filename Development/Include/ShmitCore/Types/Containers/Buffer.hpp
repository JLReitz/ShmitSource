#pragma once

#include "Container.hpp"

namespace shmit
{

/**
 * @brief A container that can let loose from the caboose. Some may also call it a "stack" and they would be... right
 * 
 * @tparam T Contained data type
 */
template <typename T>
class Buffer : public Container<T>
{
public:

    Buffer(uint16_t bufferSize = 0);
    Buffer(const T& init, uint16_t bufferSize = 1);

    virtual bool Pop(T& elementOut);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Buffer<T> object
 * 
 * @tparam T Contained data type
 * @param bufferSize Allocation size, defaults to 0 and maxes at 65,535
 */
template <typename T>
Buffer<T>::Buffer(uint16_t bufferSize)
    : Container<T>(bufferSize)
{
}

/**
 * @brief Construct a new Buffer<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param bufferSize Allocation size, defaults to 1 and maxes at 65,535
 */
template <typename T>
Buffer<T>::Buffer(const T& init, uint16_t bufferSize = 1)
    : Container<T>(init, bufferSize)
{
}

/**
 * @brief Destructive access to the back element only
 * 
 * @param elementOut Data-out reference
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Buffer<T>::Pop(T& elementOut)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Attempt to decrement the back-of-container count by 1, unless it is already 0
    uint16_t backOfBuffer = Load(e16Bits, &mBackOfContainer);
    while ((backOfBuffer > 0) && !CompareAndSwap(e16Bits, &mBackOfContainer, backOfBuffer, backOfBuffer - 1))
    {
        backOfBuffer = Load(e16Bits, &mBackOfContainer);
    }

    if (backOfBuffer > 0)
    {
        elementOut = mContainerPtr[backOfBuffer + 1];
        return true;
    }
    
    // If the back the buffer was already 0, there is nothing to pop
    // Diagnostics -- warning
    return false;
}

}