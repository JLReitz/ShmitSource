#pragma once

#include "../Memory/SharedPointer.hpp"

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

    /**
     * @brief Non-destructive access to any element
     * 
     * @param index Element location
     * @param elementOut Data-out reference
     * @return true if successful, false if unsuccessful
     */
    virtual bool Peek(size_t index, T& elementOut) const;

    /**
     * @brief Returns allocated size
     * 
     * @return size_t 
     */
    virtual size_t Size() const;

    /**
     * @brief Returns number of elements contained
     * 
     * @return size_t 
     */
    virtual size_t ElementCount() const;

    /**
     * @brief Checks vacancy status
     * 
     * @return true if full, false if not
     */
    virtual bool IsFull() const;

protected:

    SharedPointer<T> mContainerPtr;
    SharedPointer<size_t> mContainerSize;
    SharedPointer<size_t> mElementCount;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct a new Container object
 * 
 * @param containerSize Allocation size, defaults to 0
 */
template <typename T>
Container<T>::Container(size_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(nullptr), mElementCount(nullptr)
{
    if (containerSize)
    {
        // If container allocation size is nonzero, reserve its spot in memory now
        // TODO platform allocation callback
        // If allocation is successful, set container size accordingly
    }
}

/**
 * @brief Construct a new Container object with at least one initialized element
 * 
 * @param init Initial value assigned to all elements
 * @param containerSize Allocation size, defaults to 1
 */
template <typename T>
Container<T>::Container(const T& init, size_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(nullptr), mElementCount(nullptr)
{
    // Container size must be at least 1
    if (containerSize < 1)
    {
        containerSize = 1;
    }

    // TODO platform allocation callback
    // If allocation is successful, set container size accordingly
}

template <typename T>
Container<T>::~Container()
{
    // Do nothing, shared pointers should destruct themselves if appropriate
}

}