#pragma once

#include <Core/Types/FrameOfReference/Include/FrameOfReferenceDataSet.hpp>
#include <Core/Types/TransferFunction/Include/TransferFunction.hpp>

#include <vector>

namespace shmit
{
namespace control
{

template <typename FrameOfReferenceType>
class TransferFunctionQueue
{
public:

    TransferFunctionQueue(std::vector<shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>&>& tfList)
        : mTfList(tfList)
    {
        // Transfer functions and data must be contained in the same FOR type
        FrameOfReferenceType mockDataSet;
        size_t numberOfStages = mTfList.size();

        for(size_t stage = 0; stage < numberOfStages; stage++)
        {
            shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>& nextStage = mTfList[stage];
            if(mockDataSet.Type() != nextStage.Type())
            {
                // Throw exception
            }
        }
    }

    FrameOfReferenceType Cycle(const FrameOfReferenceType& input)
    {
        size_t numberOfStages = mTfList.size();
        if(numberOfStages > 0)
        {
            shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>* lastStage = nullptr;
            for(size_t stage = 0; stage < numberOfStages; stage++)
            {
                shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>& nextStage = mTfList[stage];

                // Cycle each axis of this stage
                size_t numberOfAxis = nextStage.NumberOfAxis();
                for(size_t axis = 0; axis < numberOfAxis; axis++)
                {
                    shmit::tf::TransferFunction<double>& tf = nextStage[j];
                    
                    // If this is the first-stage input filter, load input
                    if(stage == 0)
                        tf.Cycle(input[axis]);
                    else
                    {
                        // Else, load the last stage's output
                        double lastFilterOutput = (*lastStage)[axis].LastOutput();
                        tf.Cycle(lastFilterOutput);
                    }

                    lastStage = &nextStage;
                }

                // Load last stage results in to output
            }
        }
    }

    shmit::FOR::FrameOfReferenceType FrameType() const
    {
        // Every stage in the filter list has already been verified as the same type
        // Return first stage type for convenience
        return mTfList[0].Type();
    }

private:

    std::vector<shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>&>& mTfList;
};

}
}