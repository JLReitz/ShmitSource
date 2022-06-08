#pragma once

#include <ShmitCore/Platform/PrimitiveAtomic.hpp>
#include <ShmitCore/Types/Allocators/AllocatedData.hpp>
#include <ShmitCore/Types/Traits/TypeTraits.hpp>
#include <ShmitCore/Types/Traits/AllocatorTraits.hpp>
namespace shmit
{

/**
 * @brief Managed storage for one or more elements of the same type. Accesses and modifications are thread safe.
 * 
 * @tparam T Contained data type
 */
template <typename T, class TYPE_TRAITS, class ALLOCATOR>
class Container : protected AllocatedData<T, TYPE_TRAITS, ALLOCATOR>
{
public:

    Container(uint16_t containerSize = 0);
    Container(const T& init, uint16_t containerSize = 1);
    Container(T* local, uint16_t containerSize = 1); // TODO

    Container(Container<T>& rhs); // TODO
    Container(Container<T>&& rhs); // TODO

    virtual ~Container();

    virtual bool Peek(size_t index, T& elementOut) const;
    virtual bool Push(const T& element);

    bool Inject(Container<T>& rhs); // TODO

    virtual bool IsFull() const;
    virtual size_t ElementCount() const;
    size_t Size() const;

    virtual void Clear();

protected:

    T* mContainerPtr;
    uint16_t mContainerSize;
    uint16_t mBackOfContainer;

    bool mIsDynamicallyAllocated;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an new Container<T> object
 * 
 * @tparam T Contained data type
 * @param containerSize Allocation size, defaults to 0 and maxes at 65,535
 */
template <typename T>
Container<T>::Container(uint16_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(0), mBackOfContainer(0), mIsDynamicallyAllocated(false)
{
    if (containerSize)
    {
        // If container allocation size is nonzero, reserve its spot in memory now
        // TODO platform allocation callback
        // If allocation is successful, set container size accordingly
        // Set mIsDynamicallyAllocated to true
    }
}

/**
 * @brief Construct a new Container<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param containerSize Allocation size, defaults to 1 and maxes at 65,535
 */
template <typename T>
Container<T>::Container(const T& init, uint16_t containerSize)
    : mContainerPtr(nullptr), mContainerSize(0), mBackOfContainer(0), mIsDynamicallyAllocated(false)
{
    // Container size must be at least 1
    if (containerSize < 1)
    {
        containerSize = 1;
    }

    // TODO platform allocation callback
    // If allocation is successful, set container size accordingly
    // Set mBackOfContainer to 1
    // Set mIsDynamicallyAllocated to true
}

template <typename T>
Container<T>::~Container()
{
    if (mIsDynamicallyAllocated)
    {
        // TODO Do something
    }
}

/**
 * @brief Atomically clears the contained elements
 * 
 * @tparam T Contained data type
 */
template <typename T>
void Container<T>::Clear()
{
    shmit::platform::atomic::Store(shmit::size::e16Bits, &mBackOfContainer, 0);
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
    return mBackOfContainer;
}

/**
 * @brief Atomically checks vacancy status
 * 
 * @tparam T Contained data type
 * @return true if this instance is full, false if there is empty indexes
 */
template <typename T>
bool Container<T>::IsFull() const
{
    return (mBackOfContainer == mContainerSize);
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
bool Container<T>::Peek(size_t index, T& elementOut) const
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // If the requested index fits within the back of the container, it is accessible
    uint16_t backOfContainer = Load(e16Bits, &mBackOfContainer);
    if (index < backOfContainer)
    {
        elementOut = mContainerPtr[index];
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
bool Container<T>::Push(const T& element)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // Quickly save spot by atomically incrementing element count
    size_t backOfContainer = Load(e16Bits, &mBackOfContainer);
    while ((backOfContainer < mContainerSize) && 
           !CompareAndSwap(e16Bits, &mBackOfContainer, backOfContainer, backOfContainer + 1))
    {
        backOfContainer = Load(e16Bits, &mBackOfContainer);
    }

    // If the back of the container does not yet match the size, there is room to push
    if (backOfContainer < mContainerSize)
    {
        mContainerPtr[backOfContainer] = element;
        return true;
    }

    // Otherwise there is no room to push, return false
    // Diagnostics -- warning
    return false;
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
