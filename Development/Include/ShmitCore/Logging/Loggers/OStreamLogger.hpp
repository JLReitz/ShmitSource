#pragma once

#include "LoggerInterface.hpp"

#include <cstring>
#include <mutex>

namespace shmit
{
namespace log
{

template<class OStreamType>
class OStreamLogger : public LoggerInterface
{
public:
    OStreamLogger();
    OStreamLogger(OStreamType& oStream);

    void LogEntry(const Type type, const Level level, const char* id, const char* msg) override;

    virtual void Load(OStreamType& oStream);

protected:
    OStreamType* mOStream;
    std::mutex   mOStreamMutex;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class OStreamType>
OStreamLogger<OStreamType>::OStreamLogger() : mOStream(nullptr)
{
}

template<class OStreamType>
OStreamLogger<OStreamType>::OStreamLogger(OStreamType& oStream) : mOStream(&oStream)
{
}

//  Public  ===========================================================================================================

template<class OStreamType>
void OStreamLogger<OStreamType>::Load(OStreamType& oStream)
{
    mOStream = &oStream;
}

template<class OStreamType>
void OStreamLogger<OStreamType>::LogEntry(const Type type, const Level level, const char* id, const char* msg)
{
    const char* typeStr  = TypeToString(type);
    const char* levelStr = LevelToString(level);

    if (mOStream)
    {
        std::lock_guard<std::mutex> scopeLock(mOStreamMutex);
        *mOStream << typeStr << ',' << levelStr << ',' << id << ',' << msg << "\n";
    }
}

} // namespace log
} // namespace shmit