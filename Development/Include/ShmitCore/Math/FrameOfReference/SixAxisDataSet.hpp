#pragma once

#include "CartesianDataSet.hpp"

namespace shmit
{
namespace FOR
{
namespace cartesian
{

typedef enum
{
    eX      =   0,
    eY      =   1,
    eZ      =   2,
    eYaw    =   3,
    ePitch  =   4,
    eRoll   =   5
} CombinedAxis_t;

#define MAX_NUM_COMBINED_CARTESIAN_AXIS 6

template <typename T>
class SixAxisDataSet : public FrameOfReferenceDataSet<T>
{
public:

    SixAxisDataSet()
        : FrameOfReferenceDataSet<T>(shmit::FOR::FrameOfReferenceType::e6Axis, &mSixAxisData)
    {
    }

    CartesianDataSet<T> Get(AxisType axisType) const
    {
        // Create uninitialized CartesianDataSet
        CartesianDataSet<T> distanceData;

        // If requested axis type is invalid, return uninitialized data set
        if(axisType != AxisType::eInvalid)
        {
            size_t startIndex;
            size_t endIndex;
            if(axisType == AxisType::eLinear)
            {
                // Copy first set of cartesian axis from this object to the new one
                startIndex = 0;
                endIndex = MAX_NUM_CARTESIAN_AXIS;
            }
            else // Rotational axis type
            {
                // Copy last set of cartesian from this object to the new one
                startIndex = MAX_NUM_CARTESIAN_AXIS;
                endIndex = MAX_NUM_COMBINED_CARTESIAN_AXIS;
            }

            // Perform deep copy
            size_t j = 0;
            for(size_t i = startIndex; i < endIndex; i++)
            {
                distanceData.mDataSet[j++] = mSixAxisData[i];
            }
        }

        return distanceData;
    }

    void Set(AxisType axisType, const CartesianDataSet<T>& set)
    {
        // If requested axis type is invalid, pass through
        if(axisType != AxisType::eInvalid)
        {
            size_t startIndex;
            size_t endIndex;
            if(axisType == AxisType::eLinear)
            {
                // Copy first set of cartesian axis from this object to the new one
                startIndex = 0;
                endIndex = MAX_NUM_CARTESIAN_AXIS;
            }
            else // Rotational axis type
            {
                // Copy last set of cartesian from this object to the new one
                startIndex = MAX_NUM_CARTESIAN_AXIS;
                endIndex = MAX_NUM_COMBINED_CARTESIAN_AXIS;
            }

            // Perform deep copy
            for(size_t i = 0; i < MAX_NUM_CARTESIAN_AXIS; i++)
            {
                mSixAxisData[i] = set.mDataSet[i];
            }
        }

    }

private:

    T mSixAxisData[6];
};

}
}
}