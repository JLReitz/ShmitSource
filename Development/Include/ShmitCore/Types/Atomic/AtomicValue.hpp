#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <ShmitCore/Platform/MemoryHelp.hpp>
#include <ShmitCore/Platform/PrimitiveAtomic.hpp>

#include <ShmitCore/Types/TypeHelp.hpp>

namespace shmit
{

/**
 * @brief Guarantees wait-free thread-safe access to the contained data
 * 
 * @tparam T Contained data type
 */
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

    bool mIsDynamicallyAllocated;
    shmit::MemoryModel* mMemoryContext;
    T* mData;

    T* AllocateData(const T& init);
    void DeallocateData(T* addr);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    mData = AllocateData(init);
    mMemoryContext = shmit::platform::memory::GetMemoryModel(mData);
}

/**
 * @brief Construct a new AtomicValue<T> object which contains a stack-located variable
 * 
 * @tparam T Contained data type
 * @param local Pointer to stack-located variable
 */
template <typename T>
AtomicValue<T>::AtomicValue(T* const local)
    : mModifyCount(0), mData(local), mIsDynamicallyAllocated(false)
{
    mMemoryContext = shmit::platform::memory::GetMemoryModel(mData);
}

template <typename T>
AtomicValue<T>::~AtomicValue()
{
    DeallocateData(mData);
}

template <typename T>
T*  AtomicValue<T>::AllocateData(const T& init)
{
    // Allocate data pointer
    T* newData = nullptr; // TODO platform allocation callback

    // Make sure data was allocated
    // If exceptions are enabled and a bad allocation occurs this check should never be reached, you can never be too
    // careful though
    if (mData == nullptr)
    {
        // Jump to global hard-fault trap
        // TODO
    }

    return newData;
}

template <typename T>
void AtomicValue<T>::DeallocateData(T* addr)
{
    // Only proceed if the contained data was dynamically allocated and is valid
    if (mIsDynamicallyAllocated && addr)
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
    // Lock the memory context to not allow other accesses during this operation
    mMemoryContext->EnterCritical();
    toReturn = *mData;
    mMemoryContext->ExitCritical();
    
    return toReturn;
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
    // Lock the memory context to not allow other accesses during this operation
    mMemoryContext->EnterCritical();
    mData = set;
    mMemoryContext->ExitCritical();
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