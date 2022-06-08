#pragma once

#include "../Defines/SerialDefines.hpp"

namespace shmit
{

class SerialPort
{
public:

    virtual unsigned long BaudRate() const = 0;
    virtual serial::FlowControl FlowControl() const = 0;
    virtual serial::Parity Parity() const = 0;
};

}