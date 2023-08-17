#pragma once

#include "Contex.hpp"

namespace shmit
{

/**
 * @brief A less faithful version of shmit::Context. It can "get critical" with multiple threads at a time.
 */
class Semaphore : public Context
{
public:

    /**
     * @brief Construct a new Semaphore object
     * 
     * @param maxCriticalEntries The maximumum number of critical locks allowed to be placed. The default, 1, is not
     *                           recommended
     */
    Semaphore(uint16_t maxCriticalEntries = 1);

private:

    uint16_t mMaxLocks;

    void TakeLock() override;
    void ReleaseLock() override;
};

}