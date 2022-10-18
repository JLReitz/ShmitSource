#pragma once

#include "LoggerInterface.hpp"

#include <cstring>
#include <mutex>

namespace shmit
{
namespace log
{

/// @brief Logger extension for publishing entries to an output stream
/// @tparam OStreamType Output stream type
template<class OStreamType>
class OStreamLogger : public Logger
{
public:
    /// @brief Default constructor. The OStreamLogger remains uninitialized and will not publish any entries until
    /// 'Load()' is called.
    /// @param[in] filter Log entries below this level will not be published (default 'eTrace')
    OStreamLogger(Level filter = Level::eTrace);

    /// @brief Constructs and initializes an OStreamLogger, making it available to publish entries
    /// @param[in] oStream Reference to output stream instance
    /// @param[in] filter Log entries below this level will not be published
    OStreamLogger(OStreamType& oStream, Level filter = Level::eTrace);

    /// @brief Loads an output stream in to the logger, making it available to publish entries. If the logger was
    /// previously initialized with a different output stream, that reference will be overwritten by the new one.
    /// @param[in] oStream Reference to output stream instance
    virtual void Load(OStreamType& oStream);

protected:
    OStreamType* mOStream;
    std::mutex   mOStreamMutex;

private:
    /// @brief Publishes an entry
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    void PublishEntry(char const* type, char const* level, char const* id, char const* context,
                      char const* msg) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class OStreamType>
OStreamLogger<OStreamType>::OStreamLogger(Level filter) : mOStream(nullptr), Logger(filter)
{
}

template<class OStreamType>
OStreamLogger<OStreamType>::OStreamLogger(OStreamType& oStream, Level filter) : mOStream(&oStream), Logger(filter)
{
}

//  Public  ===========================================================================================================

template<class OStreamType>
void OStreamLogger<OStreamType>::Load(OStreamType& oStream)
{
    mOStream = &oStream;
}

//  Private ============================================================================================================

template<class OStreamType>
void OStreamLogger<OStreamType>::PublishEntry(char const* type, char const* level, char const* id, char const* context,
                                              char const* msg)
{
    if (mOStream)
    {
        std::lock_guard<std::mutex> scopeLock(mOStreamMutex);

        // Insert elements separately, the *hope* (it is very, very likely) is that the compiler will put them in the
        // correct order
        *mOStream << type;
        *mOStream << ',';
        *mOStream << level;
        *mOStream << ',';
        *mOStream << id;
        *mOStream << ',';
        *mOStream << context;
        *mOStream << ',';
        *mOStream << msg;
        *mOStream << "\n";
    }
}

} // namespace log
} // namespace shmit