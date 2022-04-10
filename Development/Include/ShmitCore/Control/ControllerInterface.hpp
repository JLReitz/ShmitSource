#pragma once

//#include "ControllerTransferFunction.hpp"

#include <vector>

namespace shmit
{
namespace control
{

template <typename InputDataSetType, typename OutputDataSetType>
class ControllerInterface
{
public:

    ControllerInterface(std::vector<shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>&>& inputFilterTfList, 
                        std::vector<shmit::FOR::FrameOfReferenceDataSet<shmit::tf::TransferFunction>&>& outputFilterTfList)
        : mInputFilterTfList(preFilterTfList), mOutputFilterTfList(postFilterTfList)
    {
        // Ensure that input and output FOR types match between filters and inner controller

    }

    OutputDataSetType Cycle(const InputDataSetType& input)
    {
        // Cycle input through pre-filters
        InputDataSetType controllerInput;
        mInputFilter.Cycle(input, controllerInput);

        // Run through inner controller
        OutputDataSetType controllerOutput; // TODO

        // Cycle output through post-filters
        InputDataSetType output;
        mInputFilter.Cycle(controllerOutput, output);

        return output;
    }

private:

    TransferFunctionQueue<InputDataSetType> mInputFilter;
    TransferFunctionQueue<OutputDataSetType> mOutputFiler;
};

}
}