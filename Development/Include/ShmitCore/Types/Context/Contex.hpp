#pragma once

#include <ShmitCore/StdIncludes.hpp>

namespace shmit
{

/**
 * @brief You tell me what the context is...
 * 
 * Pretty much just a rebranded mutex but with a "contextual" twist. See what I did there?
 */
class Context
{
public:

    Context();

    /**
     * @brief Enters a critical mode which blocks other threads (tasks, interrupts, processes, etc.) from doing the
     *        same with this context until it is released
     */
    void EnterCritical();

    /**
     * @brief Releases the context from critical mode
     */
    void ExitCritical();

protected:

    bool mIsCritical;
    uint16_t* mCriticalLock;

    virtual void TakeLock();
    virtual void ReleaseLock();
};

}