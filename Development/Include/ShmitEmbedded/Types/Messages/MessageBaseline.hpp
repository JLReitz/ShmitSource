#pragma once

#include <ShmitCore/Types/Containers/Container.hpp>

namespace shmit
{
namespace embed
{

/**
 * @brief Interface which declares the baseline expectations for a "shmitty" message
 */
class MessageBaseline
{
public:

    /**
     * @brief Calculate the serialized length
     * 
     * @return size_t 
     */
    virtual size_t SerializedLength() const = 0;

    /**
     * @brief Extract message data in a serialized format
     * 
     * @param dataBuf Serialized data container
     * @return size_t Number of characters written to the container
     */
    virtual size_t Serialize(shmit::Container<char>& dataBuf) = 0;

    /**
     * @brief Extract message data in a human-readable format, preferably with labels
     * 
     * @param dataBuf Formatted string container
     * @return size_t Number of characters written to the container
     */
    virtual size_t PrintFields(shmit::Container<char>& dataBuf) = 0;
};

}
}