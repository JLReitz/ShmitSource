#pragma once

#include "AtomicValue.hpp"

namespace shmit
{

template <typename T>
class AtomicFundamental : public AtomicValue<T>
{
public:

    AtomicFundamental();
    AtomicFundamental(const T& init);
    AtomicFundamental(T* const local);

    virtual bool operator!() const;

    virtual bool operator==(const T& rhs) const;
    virtual bool operator!=(const T& rhs) const;

    virtual T operator|(const T& rhs) const;
    virtual T operator&(const T& rhs) const;
    virtual T operator^(const T& rhs) const;

    virtual T operator|=(const T& rhs) const;
    virtual T operator&=(const T& rhs) const;
    virtual T operator^=(const T& rhs) const;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
AtomicFundamental<T>::AtomicFundamental()
    : AtomicValue<T>()
{
}

template <typename T>
AtomicFundamental<T>::AtomicFundamental(const T& init)
    : AtomicValue<T>(init)
{
}

template <typename T>
AtomicFundamental<T>::AtomicFundamental(T* const local)
    : AtomicValue<T>(local)
{
}

// Operators //////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
bool AtomicFundamental<T>::operator!() const
{
    T data = Get();
    return (data == 0);
}

template <typename T>
bool AtomicFundamental<T>::operator==(const T& rhs) const
{
    T data = Get();
    return (data == rhs);
}

template <typename T>
bool AtomicFundamental<T>::operator!=(const T& rhs) const
{
    T data = Get();
    return (data != rhs);
}

template <typename T>
T AtomicFundamental<T>::operator|(const T& rhs) const
{
    T data = Get();
    return data | rhs;
}

template <typename T>
T AtomicFundamental<T>::operator&(const T& rhs) const
{
    T data = Get();
    return data & rhs;
}

template <typename T>
T AtomicFundamental<T>::operator^(const T& rhs) const
{
    T data = Get();
    return data ^ rhs;
}

template <typename T>
T AtomicFundamental<T>::operator|=(const T& rhs) const
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = Get();
        post = pre | rhs;
    } while (!compare_and_swap(mData, pre, post));

    return post;
}

template <typename T>
T AtomicFundamental<T>::operator&=(const T& rhs) const
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = Get();
        post = pre & rhs;
    } while (!compare_and_swap(mData, pre, post));

    return post;
}

template <typename T>
T AtomicFundamental<T>::operator^=(const T& rhs) const
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = Get();
        post = pre ^ rhs;
    } while (!compare_and_swap(mData, pre, post));

    return post;
}

}