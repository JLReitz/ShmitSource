#pragma once

namespace shmit
{

class Context
{
public:

    virtual void Lock() = 0;
    // TODO scoped lock

    virtual void Unlock() = 0;
};

}