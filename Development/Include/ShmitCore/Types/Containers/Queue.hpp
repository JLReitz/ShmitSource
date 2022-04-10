#pragma once

#include "Container.hpp"

namespace shmit
{

/**
 * @brief A container that behaves like the line at the grocery store
 * 
 * @tparam T Contained data type
 */
template <typename T>
class Queue : public Container<T>
{
public:

    Queue(size_t queueSize = 0);
    Queue(const T& init, size_t queueSize = 1);

    virtual bool Peek(size_t index, T& elementOut) const;    
    virtual bool Pop(T& elementOut);
    virtual bool Push(const T& element);

    virtual bool IsFull() const;
    virtual size_t ElementCount() const;

protected:

    uint16_t mFrontOfQueue;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Queue<T> object
 * 
 * @tparam T Contained data type
 * @param bufferSize Allocation size, defaults to 0 and maxes at 65,535
 */
template <typename T>
Queue<T>::Queue(size_t queueSize)
    : Container<T>(queueSize), mFrontOfQueue(0)
{
}

/**
 * @brief Construct a new Queue<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param bufferSize Allocation size, defaults to 1 and maxes at 65,535
 */
template <typename T>
Queue<T>::Queue(const T& init, size_t queueSize)
    : Container<T>(init, queueSize), mFrontOfQueue(0)
{
}

/**
 * @brief Returns number of contained elements
 * 
 * @tparam T Contained data type
 * @return size_t 
 */
template <typename T>
size_t Queue<T>::ElementCount() const
{
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t backOfQueue = Load(e16Bits, &mBackOfContainer);

    // Unwrap back index if necessary
    if (backOfQueue < frontOfQueue)
    {
        backOfQueue += mContainerSize;
    }

    return backOfQueue - frontOfQueue;
}

/**
 * @brief Checks vacancy status
 * 
 * @tparam T Contained data type
 * @return true if this instance is full, false if there is empty indexes
 */
template <typename T>
bool Queue<T>::IsFull() const
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t endOfQueue = (frontOfQueue == 0) ? mContainerSize - 1 : frontOfQueue - 1;
    uint16_t backOfQueue = Load(e16Bits, &mBackOfContainer);
    return (backOfQueue == endOfQueue);
}

/**
 * @brief Non-destructive access to any contained element
 * 
 * @tparam T Contained data type
 * @param index Element location
 * @param elementOut Return value by reference. Valid only if this method returns true.
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Queue<T>::Peek(size_t index, T& elementOut) const
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Check for out-of-bounds condition
    if (index >= mContainerSize)
    {
        // Diagnostics -- warning
        return false;
    }

    // Atomically load the front of queue index and then add to the relative index
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    index += frontOfQueue;
    
    // Account for index wrap-around
    index %= mContainerSize;

    // If the requested index is not equal to the back of the queue, it is accessible
    uint16_t backOfQueue = Load(e16Bits, &mBackOfContainer);
    if (index != backOfQueue)
    {
        elementOut = mContainerPtr[index];
        return true;
    }

    // Else the requested index cannot be accessed
    // Diagnostics -- warning
    // Return false
    return false;
}

/**
 * @brief Destructive access to the front element only
 * 
 * @param elementOut Data-out reference
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Queue<T>::Pop(T& elementOut)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Attempt to increment the front of thr queue by 1, unless it is already behind the back of the queue
    uint16_t backOfQueue = Load(e16Bits, &mBackOfContainer);
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t newFrontOfQueue = (frontOfQueue + 1) % mContainerSize;
    uint16_t endOfQueue = (backOfQueue == 0) ? mContainerSize - 1 : backOfQueue - 1;
    while ((frontOfQueue != endOfQueue) && !CompareAndSwap(e16Bits, &mFrontOfQueue, frontOfQueue, newFrontOfQueue))
    {
        backOfQueue = Load(e16Bits, &mBackOfContainer);
        frontOfQueue = Load(e16Bits, &mFrontOfQueue);
        newFrontOfQueue = (frontOfQueue + 1) % mContainerSize;
        endOfQueue = (backOfQueue == 0) ? mContainerSize - 1 : backOfQueue - 1;
    }

    if (frontOfQueue != endOfQueue)
    {
        elementOut = mContainerPtr[frontOfQueue];
        return true;
    }

    // If the front of the queue is already behind the back, there is nothing to pop
    // Diagnostics -- warning
    return false;
}

/**
 * @brief Push a new element to the back
 * 
 * @tparam T Contained data type
 * @param element 
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Queue<T>::Push(const T& element)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Attempt to increment the back of the queue by 1, unless it is already behind the front of the queue
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t endOfQueue = (frontOfQueue == 0) ? mContainerSize - 1 : frontOfQueue - 1;
    uint16_t backOfQueue = Load(e16Bits, &mBackOfContainer);
    uint16_t newBackOfQueue = (backOfQueue + 1) % mContainerSize;
    while ((backOfQueue != endOfQueue) && !CompareAndSwap(e16Bits, &mBackOfContainer, backOfQueue, newBackOfQueue))
    {
        frontOfQueue = Load(e16Bits, &mFrontOfQueue);
        endOfQueue = (frontOfQueue == 0) ? mContainerSize - 1 : frontOfQueue - 1;
        backOfQueue = Load(e16Bits, &mBackOfContainer);
        newBackOfQueue = (backOfQueue + 1) % mContainerSize;
    }

    if (backOfQueue != endOfQueue)
    {
        mContainerPtr[newBackOfQueue] = element;
        return true;
    }

    // If the back of the queue is already behind the front, there is no room to push
    // Diagnostics -- warning
    return false;
}

}