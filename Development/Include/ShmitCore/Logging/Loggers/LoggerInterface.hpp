#pragma once

#include "../Defines.hpp"

namespace shmit
{
namespace log
{

class LoggerInterface
{
public:
    // TODO pass timestamp as well
    virtual void LogEntry(Type type, Level level, const char* id, const char* msg) = 0;
};

} // namespace log
} // namespace shmit