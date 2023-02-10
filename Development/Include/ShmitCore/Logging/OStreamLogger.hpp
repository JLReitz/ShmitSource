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
    void Publish(char const* type, char const* level, char const* context, char const* id, char const* msg) override;
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
void OStreamLogger<OStreamType>::Publish(char const* type, char const* level, char const* context, char const* id,
                                         char const* msg)
{
    if (m_ostream)
    {
        std::lock_guard<std::mutex> scoped_lock(m_ostream_mutex);

        // Insert elements separately, the *hope* (it is very, very likely) is that the compiler will put them in the
        // correct order
        *m_ostream << type;
        *m_ostream << ',';
        *m_ostream << level;
        *m_ostream << ',';
        *m_ostream << context;
        *m_ostream << ',';
        *m_ostream << id;
        *m_ostream << ',';
        *m_ostream << msg;
        *m_ostream << "\n";
    }
}

} // namespace log
} // namespace shmit