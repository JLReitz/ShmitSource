#include <ShmitCore/Types/Context/Semaphore.hpp>

#include <ShmitCore/Platform/PrimitiveAtomic.hpp>

namespace shmit
{

using namespace shmit::platform;
using namespace shmit::size;

Semaphore::Semaphore(uint16_t maxCriticalEntries)
    : Context(), mMaxLocks(maxCriticalEntries)
{
    // Make sure the maximum number of locks isn't 0
    if (mMaxLocks < 1)
    {
        mMaxLocks = 1;
    }
}

void Semaphore::TakeLock()
{
    // Add 1 to the critical lock
    // If it exceeds the 
    while (1)
    {
        // Add 1 to the critical lock
        // Afterwards, if the value is still below the max threshold, this thread has obtained a lock
        uint16_t afterAdd = atomic::FetchAndAdd(e16Bits, &mCriticalLock, 1);
        if(afterAdd < mMaxLocks)
        {
            // TODO diagnostics -- trace
            return;
        }
        else
        {
            // Subtract 1 from the critical lock and try again
            atomic::FetchAndSubtract(e16Bits, &mCriticalLock, 1);
        }
    }
}

void Semaphore::ReleaseLock()
{
    // Subtract 1 from the critical lock
    // TODO diagnostics -- trace
    atomic::FetchAndSubtract(e16Bits, &mCriticalLock, 1);
}

}