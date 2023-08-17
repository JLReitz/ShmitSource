#pragma once

#include <stdint.h>

namespace shmit
{

enum class OperationalState : uint8_t
{
    eOk      = 0,
    ePending = 1,
    eFaulted = 2
};

class Status
{
    /// @brief Common events
    enum
    {
        eNone = 0,

        ePreviousErrorCleared = 1,

        eRuntimeError,
        eInvalidParameter,
        eHardwareFault
    };
};

} // namespace shmit