#pragma once

#include <StdIncludes.hpp>

#include <stdexcept>
#include <cstdio>

namespace shmit
{
namespace FOR
{

enum class FrameOfReferenceType : char
{
    eInvalid    = 0,

    eCartesian  = 3,
    e6Axis      = 6,

    eQuaternion = 4 // TBD (to be developed)
};

template <typename T>
class FrameOfReferenceDataSet
{
public:

    FrameOfReferenceDataSet()
        : mNumberOfAxis(0), mDataSet(nullptr), mForType(FrameOfReferenceType::eInvalid)
    {
        // Default initialized instances are invalid and unusable
    }

    FrameOfReferenceDataSet(FrameOfReferenceDataSet<T>& rhs)
    {
        // Ensure FOR types match
        if(Type() != rhs.Type())
        {
            char[72] errorStr;
            std::sprintf(errorStr, "FrameOfReferenceDataSet: copy constructor called with mismatching child types");
            throw std::out_of_range(errorStr);
        }

        // If they do, perform deep copy
        for(size_t i = 0; i < mNumberOfAxis; i++)
        {
            mDataSet[i] = rhs.mDataSet[i];
        }
    }

    virtual T Get(size_t axisIndex) const
    {
        // Guard against out of range requests
        if(axisIndex < mNumberOfAxis)
            return mDataSet[axisIndex];

        throw std::out_of_range(mOutOfRangeExceptionMessage);
    }

    virtual void Set(size_t axisIndex, const T& val)
    {
        // Guard against out of range requests
        if(axisIndex < mNumberOfAxis)
            mDataSet[axisIndex] = val;
    }

    size_t NumberOfAxis() const
    {
        return mNumberOfAxis;
    }

    FrameOfReferenceType Type() const
    {
        return mForType;
    }

    virtual T& operator[](size_t axisIndex)
    {
        // Guard against out of range requests
        if(axisIndex < mNumberOfAxis)
            return mDataSet[axisIndex];

        char[72] errorStr;
        std::sprintf(errorStr, "FrameOfReferenceDataSet: requested axis index %d out of range (max %d)", axisIndex, 
            mNumberOfAxis);
        throw std::out_of_range(errorStr);
    }

protected:

    FrameOfReferenceDataSet(FrameOfReferenceType type, T* const dataSet)
        : mNumberOfAxis(static_cast<size_t>(type)), mForType(type), mDataSet(dataSet)
    {
    }

    T* const mDataSet;

private:

    const size_t mNumberOfAxis;
    const FrameOfReferenceType mForType;

    static const char* mOutOfRangeExceptionMessage = "FrameOfReferenceDataSet: requested axis index %d out of range (max %d)";
};

}
}