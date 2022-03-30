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

    T operator+(const T& rhs) const;
    T operator-(const T& rhs) const;
    T operator*(const T& rhs) const;
    T operator/(const T& rhs) const;

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
T AtomicNumeric<T>::operator+(const T& rhs) const
{
    T data = Get();
    return data + rhs;
}

template <typename T>
T AtomicNumeric<T>::operator-(const T& rhs) const
{
    T data = Get();
    return data - rhs;
}

template <typename T>
T AtomicNumeric<T>::operator*(const T& rhs) const
{
    T data = Get();
    return data * rhs;
}

template <typename T>
T AtomicNumeric<T>::operator/(const T& rhs) const
{
    T data = Get();
    return data / rhs;
}

template <typename T>
T AtomicNumeric<T>::operator++()
{
    mData = Get() + 1;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicNumeric<T>::operator--()
{
    mData = Get() - 1;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicNumeric<T>::operator+=(const T& rhs)
{
    mData = Get() + rhs;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicNumeric<T>::operator-=(const T& rhs)
{
    mData = Get() - rhs;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicNumeric<T>::operator*=(const T& rhs)
{
    mData = Get() * rhs;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicNumeric<T>::operator/=(const T& rhs)
{
    mData = Get() / rhs;
    mModifyCount++;
    return Get();
}

}