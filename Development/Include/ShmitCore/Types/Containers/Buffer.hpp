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

    /**
     * @brief Destructive access to the back element only
     * 
     * @param elementOut Data-out reference
     * @return true if successful, false if unsuccessful
     */
    virtual bool Pop(T& elementOut);
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Buffer< T> object
 * 
 * @tparam T Contained data type
 * @param bufferSize Allocation size, defaults to 0
 */
template <typename T>
Buffer<T>::Buffer(size_t bufferSize)
    : Container(bufferSize)
{
}

/**
 * @brief Construct a new Buffer< T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param bufferSize Allocation size, defaults to 1
 */
template <typename T>
Buffer<T>::Buffer(const T& init, size_t bufferSize = 1)
    : Container(init, bufferSize)
{
}

}