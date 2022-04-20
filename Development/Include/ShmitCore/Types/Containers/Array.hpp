#pragma once

#include "Container.hpp"

namespace shmit
{

/**
 * @brief A container that acts and feels like a normal array with extra protection. Accesses and modifications are 
 *        thread safe.
 * 
 * @tparam T Contained data type
 */
template <typename T>
class Array : public Container<T>
{
public:

    Array(uint16_t arraySize = 0);
    Array(const T& init, uint16_t arraySize = 1);

    T& operator[](size_t index);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Construct an new Array<T> object
 * 
 * @tparam T Contained data type
 * @param containerSize Allocation size, defaults to 0 and maxes at 65,535
 */
template <typename T>
Array<T>::Array(uint16_t arraySize)
    : Container<T>(arraySize)
{
}

/**
 * @brief Construct a new Array<T> object
 * 
 * @tparam T Contained data type
 * @param init Initial value assigned to all elements
 * @param containerSize Allocation size, defaults to 1 and maxes at 65,535
 */
template <typename T>
Array<T>::Array(const T& init, uint16_t arraySize)
    : Container<T>(init, arraySize)
{
}

//  Operators   ///////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Fetches a reference to a contained element
 * 
 * @tparam T Contained data type
 * @param index Element location
 * @return T&
 */
template <typename T>
T& Array<T>::operator[](size_t index)
{
    using namespace shmit::platform::atomic;
    using namespace shmit::size;

    // If the requested index fits within the back of the container, it is accessible
    uint16_t backOfContainer = Load(e16Bits, &mBackOfContainer);
    if (index < backOfContainer)
    {
        return mContainerPtr[index];
    }

    // TODO
    // Diagnostics -- warning
    // Exceptions if enabled, platform hard fault handler if not
    return T(); // This should not be reached, eventually
}

}