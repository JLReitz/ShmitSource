#include <ShmitCore/Platform/PrimitiveAtomic.hpp>

#include <ShmitCore/Types/Context/Contex.hpp>

namespace shmit
{

using namespace shmit::platform;
using namespace shmit::size;

Context::Context()
    : mIsCritical(false), mCriticalLock(0)
{
}

void Context::EnterCritical()
{
    // Check to see if the conext is already critical
    if (mIsCritical)
    {
        // TODO diagnostics -- warning
        return;
    }

    // TODO diagnostics -- trace
    TakeLock();
    mIsCritical = true;
}

void Context::ExitCritical()
{
    // Check to see if the conext is critical
    if (!mIsCritical)
    {
        // TODO diagnostics -- warning
        return;
    }

    // TODO diagnostics -- trace
    ReleaseLock();
    mIsCritical = false;
}

/**
 * @brief What actually locks the context in critical mode
 */
void Context::TakeLock()
{
    while (1)
    {
        // Load the critical lock
        // If it is 0, attempt to store 1 to it
        uint16_t fetch = atomic::Load(e16Bits, &mCriticalLock);
        if (fetch == 0 && atomic::CompareAndSwap(e16Bits, &mCriticalLock, fetch, 1))
        {
            // If the store succeeded, the context is now critical
            // TODO diagnostics -- trace
            return;
        }
    }
}

/**
 * @brief What actually releases the context from critical mode
 */
void Context::ReleaseLock()
{
    // Clear the critical lock
    // TODO diagnostics -- trace
    atomic::Store(e16Bits, &mCriticalLock, 0);
}

}