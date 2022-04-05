#pragma once

#include "AtomicFundamental.hpp"

namespace shmit
{

template <typename T>
class AtomicNumeric : public AtomicFundamental<T>
{
public:

    AtomicNumeric();
    AtomicNumeric(const T& init);
    AtomicNumeric(T* const local);

    T operator++();
    T operator--();
    T operator+=(const T& rhs);
    T operator-=(const T& rhs);
    T operator*=(const T& rhs);
    T operator/=(const T& rhs);

};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
AtomicNumeric<T>::AtomicNumeric()
    : AtomicValue<T>()
{
}

template <typename T>
AtomicNumeric<T>::AtomicNumeric(const T& init)
    : AtomicValue<T>(init)
{
}

template <typename T>
AtomicNumeric<T>::AtomicNumeric(T* const local)
    : AtomicValue<T>(local)
{
}

// Operators //////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T AtomicNumeric<T>::operator++()
{
    T value = fetch_and_add(mData, 1);
    return value;
}

template <typename T>
T AtomicNumeric<T>::operator--()
{
    T value = fetch_and_add(mData, -1);
    return value;
}

template <typename T>
T AtomicNumeric<T>::operator+=(const T& rhs)
{
    T value = fetch_and_add(mData, rhs);
    return value;
}

template <typename T>
T AtomicNumeric<T>::operator-=(const T& rhs)
{
    T sub = -1 * rhs;
    T value = fetch_and_add(mData, sub);
    return value;
}

template <typename T>
T AtomicNumeric<T>::operator*=(const T& rhs)
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = Get();
        post = pre * rhs;
    } while (!compare_and_swap(mData, pre, post));

    return post;
}

template <typename T>
T AtomicNumeric<T>::operator/=(const T& rhs)
{
    T pre; // Pre-operation value
    T post; // Post-operation value
    do
    {
        pre = Get();
        post = pre / rhs;
    } while (!compare_and_swap(mData, pre, post));

    return post;
}

}