#pragma once

#include <ShmitCore/StdIncludes.hpp>

#include <memory_resource>
#include <type_traits>
#include <vector>

namespace shmit
{

    // TODO definitions

template <typename T, class TypeTraits = std::, class ALLOCATOR_TYPE>
class PotentiallyAllocatedData
{
public:

    PotentiallyAllocatedData();
    PotentiallyAllocatedData(ALLOCATOR_TYPE& localAllocator);
    PotentiallyAllocatedData(T* const localData, ALLOCATOR_TYPE* localAllocator = nullptr);

    bool IsDynamic() const;

    operator bool() const;
    operator T*() const;

    T& operator*() const;
    T* operator->() const;

protected:

    T* mData;
    size_t mNumElements;
    bool mIsDynamicallyAllocated;
    
    void Create();
    void Destroy();

private:

    ALLOCATOR_TYPE* mLocalAllocator = nullptr;
};

}