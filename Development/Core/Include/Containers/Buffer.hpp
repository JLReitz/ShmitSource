#pragma once

#include "Container.hpp"

namespace shmit
{

template <typename T>
class Buffer : public Container
{
public:

    Buffer(size_t bufferSize = 0);
    Buffer(const T& init, size_t bufferSize = 1);

    virtual bool Push(const T& element);

    /**
     * @brief Destructive access to the back element only
     * 
     * @param elementOut Data-out reference
     * @return true if successful, false if unsuccessful
     */
    virtual bool Pop(T& elementOut);

private:

    size_t mImplCount;

    T* mBufferPtr;
    size_t mBufferSize;
    size_t mBufferLength;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Buffer object
 * 
 * @param bufferSize Buffer allocation size, defaults to 0
 */
template <typename T>
Buffer<T>::Buffer(size_t bufferSize)
    : Container(bufferSize)
{
}

/**
 * @brief Construct a new Buffer object with at least one initialized element
 * 
 * @param init Initial value assigned to all elements
 * @param bufferSize Buffer allocation size, defaults to 1
 */
template <typename T>
Buffer<T>::Buffer(const T& init, size_t bufferSize = 1)
    : Container(init, bufferSize)
{
}

/**
 * @brief Push a new element to the back
 * 
 * @param element
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Buffer<T>::Push(const T& element)
{
    // TODO Enter platform atomic mode

    // Check vacancy status
    if (!IsFull())
    {
        // If not full, add new element to the end
        mBufferPtr[mBufferLength] = element;
        mBufferLength++;

        // TODO Exit platform atomic mode

        return true;
    }

    // TODO Exit platform atomic mode

    return false;
}

}