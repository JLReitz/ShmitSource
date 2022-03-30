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
    unsigned short modifyCountRef;
    T prevData;
    do
    {
        modifyCountRef = mModifyCount;
        prevData = mData;

    } while (modifyCountRef != mModifyCount);

    T newData = prevData | rhs;

    mData = Get() | rhs;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicFundamental<T>::operator&=(const T& rhs) const
{
    mData = Get() & rhs;
    mModifyCount++;
    return Get();
}

template <typename T>
T AtomicFundamental<T>::operator^=(const T& rhs) const
{
    mData = Get() ^ rhs;
    mModifyCount++;
    return Get();
}

}