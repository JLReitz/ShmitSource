#pragma once

#include <StdIncludes.hpp>

namespace shmit
{

template <typename T>
class SharedPointer
{
public:

    SharedPointer();
    SharedPointer(const SharedPointer<T>& rhs);
    SharedPointer(SharedPointer<T>&& rhs);
    ~SharedPointer();

    const SharedPointer<T>& operator=(const SharedPointer<T>& rhs);
    const SharedPointer<T>& operator=(SharedPointer<T>&& rhs);

    T& operator*() const;
    T* operator->() const;

    T* Get() const;

    size_t Count() const;

    static SharedPointer<T> Create(const T& init, size_t numElements = 0);
    static SharedPointer<T> Create(T&& init);

private:

    SharedPointer(T* subject, size_t numElements);

    /**
     * @brief Container length representative. Only for internal use if this object is referencing a container 
     *        structure.
     */
    size_t mElementCount;

    size_t* mImplCount; // TODO should be pointer to atomic
    T* mSubjectPtr;
};

//  Method Implementations  ///////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an uninitialized SharedPointer<T> object
 * 
 * @tparam T Data type to point at
 */
template <typename T>
SharedPointer<T>::SharedPointer()
    : mSubjectPtr(nullptr), mImplCount(nullptr), mElementCount(0)
{
}

/**
 * @brief SharedPointer<T> copy constructor. Links new instance to all others in ancestry.
 * 
 * @tparam T Data type to point at
 * @param rhs SharedPointer<T> with the same template type
 */
template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T>& rhs)
    : mSubjectPtr(rhs.mSubjectPtr), mImplCount(rhs.mImplCount), mElementCount(rhs.mElementCount)
{
    // Increment implementation count
    (*mImplCount)++;
}

/**
 * @brief SharedPointer<T> move constructor. Links new instance to all others that represent the same subject.
 * 
 * @tparam T Data type to point at
 * @param rhs SharedPointer<T> with the same template type
 */
template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer<T>&& rhs)
    : mSubjectPtr(rhs.mSubjectPtr), mImplCount(rhs.mImplCount), mElementCount(rhs.mElementCount)
{
    // Increment implementation count for when rhs inevitably is destructed
    (*mImplCount)++;
}

/**
 * @brief SharedPointer<T> private constructor. Used by Create() exclusively.
 * 
 * @tparam T Data type to point at
 * @param subject Pointer to dynamically-allocated element[s]
 * @param numElements Number of elements if the subject is a container
 */
template <typename T>
SharedPointer<T>::SharedPointer(T* subject, size_t numElements)
    : mSubjectPtr(subject), mElementCount(numElements)
{
    // Allocate implementation count and set to 0
    // TODO platform allocation callback
}

template <typename T>
SharedPointer<T>::~SharedPointer()
{
    if(--mImplCount == 0)
    {
        // Release subject and implementation count
        // TODO platform de-allocation callback
    }
}

/**
 * @brief Returns the number of instances that share the subject, including this one
 * 
 * @tparam T Data type to point at
 * @return size_t 
 */
template <typename T>
size_t SharedPointer<T>::Count() const
{
    return *mImplCount;
}

/**
 * @brief Creates a new SharedPointer<T> that is not linked to others
 * 
 * @tparam T 
 * @param init Initialized value of the subject pointer
 * @param numElements Number of elements this pointer will represent if it is a container
 * @return SharedPointer<T> 
 */
template <typename T>
SharedPointer<T> SharedPointer<T>::Create(const T& init, size_t numElements)
{
    // Allocate memory for the templated type
    // TODO platform allocation callback
    T* subject = nullptr;

    // Invoke private constructor
    return SharedPointer<T>(subject, numElements);
}

/**
 * @brief Creates a new SharedPointer<T> that is not linked to others
 * 
 * @tparam T Data type to point at
 * @param init Initialized value of the subject
 * @return SharedPointer<T> 
 */
template <typename T>
SharedPointer<T> SharedPointer<T>::Create(T&& init)
{
    // Allocate memory for the templated type
    // TODO platform allocation callback
    T* subject = nullptr;

    // Invoke private constructor
    return SharedPointer<T>(subject, 1);
}

/**
 * @brief Returns the subject location
 * 
 * @tparam T Data type to point at
 * @return T* 
 */
template <typename T>
T* SharedPointer<T>::Get() const
{
    return *mSubjectPtr;
}

//  Operators   ///////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
const SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T>& rhs)
{
    mSubjectPtr = rhs.mSubjectPtr;
    mImplCount = rhs.mImplCount;
    mElementCount = rhs.mElementCount;
}

template <typename T>
const SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T>&& rhs)
{
    mSubjectPtr = rhs.mSubjectPtr;
    mImplCount = rhs.mImplCount;
    mElementCount = rhs.mElementCount;

    // Increment implementation count for when rhs inevitably is destructed
    (*mImplCount)++;
}

template <typename T>
T& SharedPointer<T>::operator*() const
{
    return mSubjectPtr;
}

template <typename T>
T* SharedPointer<T>::operator->() const
{
    return mSubjectPtr;
}

}