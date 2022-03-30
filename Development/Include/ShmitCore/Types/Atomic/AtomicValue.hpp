#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{

// TODO implement "atomic action" table that is basically a dumb circular queue
// TODO for unit testing see saved photo "AtomicTesting.png"

enum class AtomicAction : short
{
    eNoOp   = 0,

    eSet    = 0x01,

    eAdd    = 0x10
};

typedef struct
{
    AtomicAction mAction;

    bool mIsReady;
    bool mIsClaimed;
} AtomicActionNode;


template <typename T>
class AtomicValue
{
public:

    AtomicValue();
    AtomicValue(const T& init);
    AtomicValue(T* const local);

    virtual ~AtomicValue();

    const T Get() const;
    void Set(const T& set);

    operator T() const;
    const T operator=(const T& rhs);

protected:

    unsigned short mModifyCount;
    T* mData;

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
    // Allocate data pointer
    // TODO platform allocation callback
}

template <typename T>
AtomicValue<T>::AtomicValue(T* const local)
    : mModifyCount(0), mData(local), mIsDynamicallyAllocated(true)
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
    unsigned short modifyCountRef;
    do
    {
        modifyCountRef = mModifyCount
        toReturn = *mData;
    } while (modifyCountRef != mModifyCount);
    
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
    *mData = set;
    mModifyCount++;
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