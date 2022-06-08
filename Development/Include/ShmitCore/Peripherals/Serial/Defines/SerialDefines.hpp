#pragma once

namespace shmit
{
namespace serial
{
    
enum class FlowControl
{
    eNone   = 0,

    eSwCtrl   = 1,
    eHwCtrl     = 2
};

enum class Parity
{
    eNone   = 0,

    e1Bit   = 1,
    e2Bits  = 2
};

struct SerialPortConfig
{
    
}

}
}