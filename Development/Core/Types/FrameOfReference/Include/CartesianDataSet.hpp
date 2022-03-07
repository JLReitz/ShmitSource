#pragma once

#include "CartesianDefines.hpp"
#include "FrameOfReferenceDataSet.hpp"

namespace shmit
{
namespace FOR
{
namespace cartesian
{

template <typename T>
class CartesianDataSet : public FrameOfReferenceDataSet<T>
{
public:

    CartesianDataSet()
        : FrameOfReferenceDataSet<T>(shmit::FOR::FrameOfReferenceType::eCartesian, &mThreeAxisData)
    {
    }

private:

    T mThreeAxisData[3];
};

}
}
}