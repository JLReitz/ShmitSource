#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <ShmitCore/Platform/PrimitiveAtomic.hpp>

namespace shmit
{

template <typename T>
class AtomicValue
{
public:

    AtomicValue();
    AtomicValue(const T& init);
    AtomicValue(T* const local);

    virtual ~AtomicValue();

    virtual const T Get() const;
    virtual void Set(const T& set);

    operator T() const;
    const T operator=(const T& rhs);

protected:

    volatile uint16_t mModifyCount;
    T* mData;

    void AllocateData(const T& init);

private:

    bool mIsDynamicallyAllocated;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an uninitialized AtomicValue<T> object
 * 
 * @tparam T Contained data type
 */
template <typename T>
AtomicValue<T>::AtomicValue()
    : mModifyCount(0), mData(nullptr), mIsDynamicallyAllocated(false)
{
}

/**
 * @brief Construct a new AtomicValue<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to contained type
 */
template <typename T>
AtomicValue<T>::AtomicValue(const T& init)
    : mModifyCount(0), mData(nullptr), mIsDynamicallyAllocated(true)
{
    AllocateData(init);
}

/**
 * @brief Construct a new AtomicValue<T> object which contains a stack-located variable
 * 
 * @tparam T T Contained data type
 * @param local Pointer to stack-located variable
 */
template <typename T>
AtomicValue<T>::AtomicValue(T* const local)
    : mModifyCount(0), mData(local), mIsDynamicallyAllocated(false)
{
}

template <typename T>
AtomicValue<T>::~AtomicValue()
{
    if (mIsDynamicallyAllocated)
    {
        // Deallocate stored data
        // TODO platform deallocation callback
    }
}

/**
 * @brief Atomic read of contained type
 * 
 * @tparam T Contained data type
 * @return const T 
 */
template <typename T>
const T AtomicValue<T>::Get() const
{
    T toReturn;
    uint16_t countRefBefore;
    uint16_t countRefAfter;
    do
    {
        countRefBefore = shmit::platform::atomic::Load(shmit::size::e16Bits, &mModifyCount);

        toReturn = *mData; // TODO replace with load operation

        countRefAfter = shmit::platform::atomic::Load(shmit::size::e16Bits, &mModifyCount);
    } while (countRefBefore != countRefAfter);
    
    return toReturn;
}

template <typename T>
void AtomicValue<T>::AllocateData(const T& init)
{
    // Allocate data pointer
    // TODO platform allocation callback

    // Make sure data was allocated
    if (mData != nullptr)
    {
        *mData = init;
    }
    else
    {
        // Throw exception
        // TODO
    }
}

/**
 * @brief Atomic write to contained type
 * 
 * @tparam T Contained data type
 * @param set Value to write
 */
template <typename T>
void AtomicValue<T>::Set(const T& set)
{
    // TODO replace with store operation
    *mData = set;
    shmit::platform::atomic::FetchAndAdd(shmit::size::e16Bits, &mModifyCount, 1);
}

//  Operators   ///////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
AtomicValue<T>::operator T() const
{
    return Get();
}

template <typename T>
const T AtomicValue<T>::operator=(const T& rhs)
{
    Set(rhs);
    return Get();
}

}