#pragma once

#include <ShmitCore/Types/Atomic/PrimitiveAtomic.hpp>

namespace shmit
{

template <typename T>
class Container
{
public:

    Container(size_t containerSize = 0);
    Container(const T& init, size_t containerSize = 1);
    Container(const Container& copy);

    virtual ~Container();

    virtual bool Peek(size_t index, T& elementOut) const;
    virtual bool Push(const T& element);

    bool IsFull() const;
    size_t ElementCount() const;
    size_t Size() const;

protected:

    T* mContainerPtr;
    size_t mContainerSize;
    AtomicULong mAtomicElementCount;

private:

    size_t mElementCount;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an new Container<T> object
 * 
 * @tparam T Contained data type
 * @param containerSize Allocation size, defaults to 0
 */
template <typename T>
Container<T>::Container(size_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(0), mElementCount(0), mAtomicElementCount(&mElementCount)
{
    if (containerSize)
    {
        // If container allocation size is nonzero, reserve its spot in memory now
        // TODO platform allocation callback
        // If allocation is successful, set container size accordingly
    }
}

/**
 * @brief Construct a new Container<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param containerSize Allocation size, defaults to 1
 */
template <typename T>
Container<T>::Container(const T& init, size_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(0), mElementCount(0), mAtomicElementCount(&mElementCount)
{
    // Container size must be at least 1
    if (containerSize < 1)
    {
        containerSize = 1;
    }

    // TODO platform allocation callback
    // If allocation is successful, set container size accordingly
}

/**
 * @brief Container<T> copy constructor
 * 
 * @tparam T Contained data type
 * @param copy Container<T> object to copy
 */
template <typename T>
Container<T>::Container(const Container& copy)
    : mContainerPtr(copy.mContainerPtr), mContainerSize(copy.mContainerSize), mElementCount(copy.mElementCount),
      mAtomicElementCount(&mElementCount)
{
}

template <typename T>
Container<T>::~Container()
{
    // Do nothing, shared pointers should destruct themselves if appropriate
}

/**
 * @brief Returns number of contained elements
 * 
 * @tparam T Contained data type
 * @return size_t 
 */
template <typename T>
size_t Container<T>::ElementCount() const
{
    return *mAtomicElementCount;
}

/**
 * @brief Checks vacancy status
 * 
 * @tparam T Contained data type
 * @return true if this instance is full, false if there is empty indexes
 */
template <typename T>
bool Container<T>::IsFull() const
{
    return (mAtomicElementCount == mContainerSize);
}

/**
 * @brief Non-destructive access to any element
 * 
 * @tparam T Contained data type
 * @param index Element location
 * @param elementOut Return value by reference. Valid only if this method returns true.
 * @return true if successful, false if unsuccessful
 */
template <typename T>
bool Container<T>::Peek(size_t index, T& elementOut) const
{
    if (index < mAtomicElementCount)
    {
        elementOut = mContainerPtr[index];
        return true;
    }

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
bool Container<T>::Push(const T& element)
{
    // Quickly save spot by atomically incrementing element count
    size_t incremenentedCount = mAtomicElementCount++;

    // Check to see if space has been over-booked
    if(incremenentedCount >= mContainerSize)
    {
        // if it is, quickly decremement element count (atomically) again and return false for failed operation
        mAtomicElementCount--;
        return false;
    }

    // Otherwise, the push will be a success
    // Load the new element in to the original index value before we incremented it
    mContainerPtr[incremenentedCount - 1] = element;
    return true;
}

/**
 * @brief Returns allocated size
 * 
 * @tparam T Contained data type
 * @return size_t 
 */
template <typename T>
size_t Container<T>::Size() const
{
    return mContainerSize;
}

}
