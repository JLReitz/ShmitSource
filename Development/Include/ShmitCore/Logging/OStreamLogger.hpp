#pragma once

#include "Logger.hpp"

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
    /*******************************************************************************************************************
     * Default constructor implicitly defined
     * ---------------------------------------
     * Will initialize OStreamLogger in an "unloaded state" where it will not be able to publish any entries until
     * 'Load()' is called.
     *******************************************************************************************************************/

    /// @brief Constructor overload. Initializes OStreamLogger in a "loaded" state.
    /// @param[in] ostream Output stream
    /// @param[in] filter Log entries below this level will not be published
    OStreamLogger(OStreamType& ostream, Level filter);

    /// @brief Loads an output stream in to the logger, making it available to publish entries. If the logger was
    /// previously loaded with a different output stream, that reference will be replaced by the new one.
    /// @param[in] ostream Reference to output stream instance
    virtual void Load(OStreamType& ostream);

protected:
    OStreamType* m_ostream {nullptr}; //!< Pointer to output stream instance (default = 'nullptr')
    std::mutex   m_ostream_mutex;     //!< Controls access to output stream

private:
    /// Publishes an entry
    /// @param[in] type Log type
    /// @param[in] level Log level
    /// @param[in] id Log ID
    /// @param[in] context Log context
    /// @param[in] msg Log message
    void Publish(String const& str) override;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Method definitions in alphabetical order    ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class OStreamType>
OStreamLogger<OStreamType>::OStreamLogger(OStreamType& ostream, Level filter) : Logger(filter), m_ostream(&ostream)
{
}

//  Public  ============================================================================================================

template<class OStreamType>
void OStreamLogger<OStreamType>::Load(OStreamType& ostream)
{
    m_ostream = &ostream;
}

//  Private
//  =============================================================================================================

template<class OStreamType>
void OStreamLogger<OStreamType>::Publish(String const& str)
{
    if (m_ostream)
    {
        std::lock_guard<std::mutex> scoped_lock(m_ostream_mutex);
        *m_ostream << str;
    }
}

} // namespace log
} // namespace shmit