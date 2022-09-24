#pragma once

#include "LoggerInterface.hpp"

namespace shmit
{
namespace log
{

class VoidLogger : public LoggerInterface
{
public:
    void LogEntry(const Type type, const Level level, const char* id, const char* msg) override;
};

} // namespace log
} // namespace shmit