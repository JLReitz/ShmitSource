#pragma once

#include "AtomicValue.hpp"

namespace shmit
{

/**
 * @brief Variant of shmit::AtomicValue which wraps a primitive (8, 16, 32, or 64 bit) data type
 * 
 * @tparam T Contained data type, size must be defined by enum shmit::size::Primitive
 */
template <typename T>
class AtomicPrimitive : public AtomicValue<T>
{
public:

    AtomicPrimitive();
    AtomicPrimitive(const T& init);
    AtomicPrimitive(T* const local);

    const T Get() const override;
    void Set(const T& set) override;

    virtual bool operator!() const;

    virtual bool operator==(const T& rhs) const;
    virtual bool operator!=(const T& rhs) const;

    virtual T operator|(const T& rhs) const;
    virtual T operator&(const T& rhs) const;
    virtual T operator^(const T& rhs) const;

    virtual T operator|=(const T& rhs) const;
    virtual T operator&=(const T& rhs) const;
    virtual T operator^=(const T& rhs) const;

    T operator++();
    T operator--();
    T operator+=(const T& rhs);
    T operator-=(const T& rhs);
    T operator*=(const T& rhs);
    T operator/=(const T& rhs);

private:

    shmit::size::Primitive mPrimitiveTypeSize;
};

// Some typedefs for common possible primitive atomics
typedef AtomicPrimitive<bool>           AtomicBool;

typedef AtomicPrimitive<int>            AtomicInt;
typedef AtomicPrimitive<unsigned int>   AtomicUInt;

typedef AtomicPrimitive<long>           AtomicLong;
typedef AtomicPrimitive<unsigned long>  AtomicULong;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an uninitialized AtomicPrimitive<T> object
 * 
 * @tparam T Contained data type, size must be defined by enum shmit::size::Primitive
 */
template <typename T>
AtomicPrimitive<T>::AtomicPrimitive()
    : AtomicValue<T>()
{
}

/**
 * @brief Construct a new AtomicPrimitive<T> object
 * 
 * @tparam T Contained data type, size must be defined by enum shmit::size::Primitive
 * @param init Initial value assigned to contained type
 */
template <typename T>
AtomicPrimitive<T>::AtomicPrimitive(const T& init)
    : AtomicValue<T>(init)
{
    // Exception or hard-fault trap will block the base class constructor from continuing if allocation fails

    // Make sure the templated type matches the requirements to be an atomic primitive
    if (mMemoryContext->IsPrimitive<T>())
    {
        // Now the size in bytes should cast to one of the shmit::size::Primitive enumerations
        size_t typeSizeInBytes = shmit::platform::help::TypeSizeInBytes<T>();
        mPrimitiveTypeSize = static_cast<shmit::size::Primitive>(typeSizeInBytes);
    }
    else
    {
        // Enter platform hard-fault trap or throw fault
        // TODO
    }
}

/**
 * @brief Construct a new AtomicPrimitive<T> object which contains a stack-located variable
 * 
 * @tparam T Contained data type, size must be defined by enum shmit::size::Primitive
 * @param local Pointer to stack-located variable
 */
template <typename T>
AtomicPrimitive<T>::AtomicPrimitive(T* const local)
    : AtomicValue<T>(local)
{
    // Make sure the templated type matches the requirements to be an atomic primitive
    if (mMemoryContext->IsPrimitive<T>())
    {
        // Now the size in bytes should cast to one of the shmit::size::Primitive enumerations
        size_t typeSizeInBytes = shmit::platform::help::TypeSizeInBytes<T>();
        mPrimitiveTypeSize = static_cast<shmit::size::Primitive>(typeSizeInBytes);
    }
    else
    {
        // Enter platform hard-fault trap or throw fault
        // TODO
    }
}

// Operators //////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool AtomicPrimitive<T>::operator!() const
{
    T data = Get();
    return (data == 0);
}

template <typename T>
bool AtomicPrimitive<T>::operator==(const T& rhs) const
{
    T data = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
    return (data == rhs);
}

template <typename T>
bool AtomicPrimitive<T>::operator!=(const T& rhs) const
{
    T data = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
    return (data != rhs);
}

template <typename T>
T AtomicPrimitive<T>::operator|(const T& rhs) const
{
    T data = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
    return data | rhs;
}

template <typename T>
T AtomicPrimitive<T>::operator&(const T& rhs) const
{
    T data = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
    return data & rhs;
}

template <typename T>
T AtomicPrimitive<T>::operator^(const T& rhs) const
{
    T data = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
    return data ^ rhs;
}

template <typename T>
T AtomicPrimitive<T>::operator|=(const T& rhs) const
{
    using namespace shmit::platform::atomic;
    
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
        post = pre | rhs;
    } while (!shmit::platform::atomic::CompareAndSwap(mPrimitiveTypeSize, mData, pre, post));

    return post;
}

template <typename T>
T AtomicPrimitive<T>::operator&=(const T& rhs) const
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
        post = pre & rhs;
    } while (!shmit::platform::atomic::CompareAndSwap(mPrimitiveTypeSize, mData, pre, post));

    return post;
}

template <typename T>
T AtomicPrimitive<T>::operator^=(const T& rhs) const
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
        post = pre ^ rhs;
    } while (!shmit::platform::atomic::CompareAndSwap(mPrimitiveTypeSize, mData, pre, post));

    return post;
}

template <typename T>
T AtomicPrimitive<T>::operator++()
{
    T value = shmit::platform::atomic::FetchAndAdd(mPrimitiveTypeSize, mData, 1);
    return value;
}

template <typename T>
T AtomicPrimitive<T>::operator--()
{
    T value = shmit::platform::atomic::FetchAndSubtract(mPrimitiveTypeSize, mData, 1);
    return value;
}

template <typename T>
T AtomicPrimitive<T>::operator+=(const T& rhs)
{
    T value = shmit::platform::atomic::FetchAndAdd(mPrimitiveTypeSize, mData, rhs);
    return value;
}

template <typename T>
T AtomicPrimitive<T>::operator-=(const T& rhs)
{
    T value = shmit::platform::atomic::FetchAndSubtract(mPrimitiveTypeSize, mData, rhs);
    return value;
}

template <typename T>
T AtomicPrimitive<T>::operator*=(const T& rhs)
{AtomicPrimitive
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
        post = pre * rhs;
    } while (!shmit::platform::atomic::CompareAndSwap(mPrimitiveTypeSize, mData, pre, post));

    return post;
}

template <typename T>
T AtomicPrimitive<T>::operator/=(const T& rhs)
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = shmit::platform::atomic::Load(mPrimitiveTypeSize, mData);
        post = pre / rhs;
    } while (!shmit::platform::atomic::CompareAndSwap(mPrimitiveTypeSize, mData, pre, post));

    return post;
}

}