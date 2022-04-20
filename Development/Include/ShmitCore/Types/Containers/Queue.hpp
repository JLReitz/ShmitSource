#pragma once

#include "Container.hpp"

namespace shmit
{

/**
 * @brief A container that behaves like the line at the grocery store. Accesses and modifications are thread safe.
 * 
 * @tparam T Contained data type
 */
template <typename T>
class Queue : public Container<T>
{
public:

    Queue(size_t queueSize = 0);
    Queue(const T& init, size_t queueSize = 1);

    virtual bool Peek(size_t index, T& elementOut) const override;    
    virtual bool Pop(T& elementOut) override;
    virtual bool Push(const T& element) override;

    virtual bool IsFull() const override;
    virtual size_t ElementCount() const override;

    virtual void Clear() override;

protected:

    // Flags to signal empty or full conditions
    // Stored as uint8_t so that they are memory aligned for atomic operations
    uint8_t mIsEmptyFlag;
    uint8_t mIsFullFlag;

    uint16_t mFrontOfQueue; // First poppable index
    uint16_t mBackOfQueue; 
    // ^ Last poppable index, this lags 'mBackOfContainer' and gates operations from accessing elements which may not
    // be done updating
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
    : Container<T>(queueSize), mFrontOfQueue(0), mBackOfQueue(0), mIsEmptyFlag(true), mIsFullFlag(false)
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
    : Container<T>(init, queueSize), mFrontOfQueue(0), mBackOfQueue(0), mIsEmptyFlag(true), mIsFullFlag(false)
{
}

/**
 * @brief Atomically clears the contained elements
 * 
 * @tparam T Contained data type
 */
template <typename T>
void Queue<T>::Clear()
{
    
}

/**
 * @brief Returns number of contained elements. Value is atomically accessed.
 * 
 * @tparam T Contained data type
 * @return size_t 
 */
template <typename T>
size_t Queue<T>::ElementCount() const
{
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t backOfContainer = Load(e16Bits, &mBackOfContainer);

    // Unwrap the back-of-container index if necessary
    if (backOfContainer < frontOfQueue)
    {
        backOfContainer += mContainerSize;
    }

    return backOfContainer - frontOfQueue;
}

/**
 * @brief Atomically checks vacancy status
 * 
 * @tparam T Contained data type
 * @return true if this instance is full, false if there is empty indexes
 */
template <typename T>
bool Queue<T>::IsFull() const
{
    return shmit::platform::atomic::Load(shmit::size::e8Bits, &mIsFullFlag);
}

/**
 * @brief Atomic non-destructive access to any contained element
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

    // Check for empty or out-of-bounds conditions
    bool isEmptyFlag = Load(e8Bits, &mIsEmptyFlag);
    if (isEmptyFlag || (index >= mContainerSize))
    {
        // Diagnostics -- warning
        return false;
    }

    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t backOfQueue = Load(e16Bits, &mBackOfQueue);
    bool isFullFlag = Load(e8Bits, &mIsFullFlag);

    // Account for back-of-queue wrap-around by unwrapping
    if (backOfQueue < frontOfQueue)
    {
        backOfQueue += mContainerSize;
    }

    // Check for out-of-bounds conditions past the back-of-queue index
    // If the index extends past the back, it is not accessible
    // If the index equals the back, it is only accessible when the full flag is also set
    index += frontOfQueue;
    if ((index > backOfQueue) || ((index == backOfQueue) && !mIsFullFlag))
    {
        // Diagnostics -- warning
        return false;
    }
    
    // Account for index wrap-around
    // Because of the out-of-bounds checks above, we are assured that any wrapped index is <= back-of-queue
    index %= mContainerSize;
    elementOut = mContainerPtr[index];
    return true;
}

/**
 * @brief Atomic destructive access to the front element only
 * 
 * @param elementOut Data-out reference
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Queue<T>::Pop(T& elementOut)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Attempt to increment the front-of-queue by 1 until it is equal to the back-of-queue
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t newFrontOfQueue = (frontOfQueue + 1) % mContainerSize;
    uint16_t backOfQueue = Load(e16Bits, &mBackOfQueue);
    while ((frontOfQueue != backOfQueue) && !CompareAndSwap(e16Bits, &mFrontOfQueue, frontOfQueue, newFrontOfQueue))
    {
        backOfQueue = Load(e16Bits, &mBackOfQueue);
        frontOfQueue = Load(e16Bits, &mFrontOfQueue);
        newFrontOfQueue = (frontOfQueue + 1) % mContainerSize;
    }

    // If the front-of-queue is not equal to the back, there is something to pop
    // If it is, attempt to set the empty flag
    // Another thread may beat this one to the punch, if so this will fail as the queue is now empty
    if ((frontOfQueue != backOfQueue) || CompareAndSwap(e8Bits, &mIsEmptyFlag, false, true))
    {
        elementOut = mContainerPtr[frontOfQueue];

        // Attempt to clear the full flag, will fail silently
        CompareAndSwap(e8Bits, &mIsFullFlag, true, false)

        return true;
    }

    // Diagnostics -- warning
    return false;
}

/**
 * @brief Atomically pushes a new element to the back
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

    // Attempt to increment the back-of-container by 1 unless it is equal to the front
    uint16_t frontOfQueue = Load(e16Bits, &mFrontOfQueue);
    uint16_t backOfContainer = Load(e16Bits, &mBackOfContainer);
    uint16_t newbackOfContainer = (backOfContainer + 1) % mContainerSize;
    while ((newBackOfContainer != frontOfQueue) && 
           !CompareAndSwap(e16Bits, &mBackOfContainer, backOfContainer, newbackOfContainer))
    {
        frontOfQueue = Load(e16Bits, &mFrontOfQueue);
        backOfContainer = Load(e16Bits, &mBackOfContainer);
        newbackOfContainer = (backOfContainer + 1) % mContainerSize;
    }

    // If the new back-of-container index is not equal to the front, there is room to push
    // If it is, attempt to set the full flag
    // Another thread may beat this one to the punch, if so this will fail as there is no more room
    if ((newBackOfContainer != frontOfQueue) || CompareAndSwap(e8Bits, &mIsFullFlag, false, true))
    {
        mContainerPtr[backOfContainer] = element;

        // Block until the back of the queue is equal to the cached back of container, then update it to the new back
        // of container
        while (!CompareAndSwap(e16Bits, &mBackOfQueue, backOfContainer, newbackOfContainer));

        // Attempt to clear the empty flag, will fail silently
        CompareAndSwap(e8Bits, &mIsEmptyFlag, true, false)

        return true;
    }

    // Diagnostics -- warning
    return false;
}

}