#pragma once

#include "Instant.hpp"

namespace shmit
{
namespace time
{

/**!
 * @brief Single-shot interface for polling expiration over time
 *
 */
class Timer
{
public:
    /**!
     * @brief Checks for expiration
     *
     * @retval true if expired
     * @retval false otherwise
     */
    virtual bool IsExpired() const noexcept = 0;

    /**!
     * @brief Checks for expiration overrun past a threshold set by the implementation
     *
     * @note This is intended to provide an error check for higher level components that may want to treat this
     * situation differently. As such, implementations should set their criteria for this check to whether something out
     * of the ordinary has occured.
     *
     * @retval true if over-expired
     * @retval false otherwise
     */
    virtual bool IsOverExpired() const noexcept = 0;

    /**!
     * @brief Resets the timer
     *
     */
    virtual void Reset() noexcept = 0;
};

/**!
 * @brief Wraps a Timer to add automatic reset functionality
 *
 */
class PeriodicTimer final : public Timer
{
public:
    PeriodicTimer() = delete;

    /**!
     * @brief Constructs a PeriodicTimer
     *
     * @param timer Timer to wrap
     */
    PeriodicTimer(Timer& timer);

    /**!
     * @brief Checks for expiration and resets the underlying timer if it has occurred
     *
     * @retval true if expired
     * @retval false otherwise
     */
    bool IsExpired() const noexcept override;

    /**!
     * @brief Checks for over-expiration in the underlying timer
     *
     * @retval true if over-expired
     * @retval false otherwise
     */
    bool IsOverExpired() const noexcept override;

    /**!
     * @brief Resets the underlying timer
     *
     */
    void Reset() noexcept override;

private:
    Timer& m_timer;
};

/**!
 * @brief Basic Timer implementation that utilizes a static clock to poll for expiration
 *
 * @tparam Clock Time source. Must meet the ShmitCore named requirements for Clock.
 */
template<typename Clock>
class BasicTimer : public Timer
{
public:
    using TimeInstant = Instant<Clock>; /*! Instant specialization used by the timer */

    /**!
     * @brief Constructs a zeroed out BasicTimer which will expire immediately
     *
     */
    BasicTimer() = default;

    /**!
     * @brief Constructs a BasicTimer with a set duration
     *
     * @tparam Denomination Units of time
     * @param duration Timer duration
     */
    template<typename Denomination>
    BasicTimer(Duration<Denomination> const& duration);

    /**!
     * @copydoc Timer::IsExpired()
     *
     */
    bool IsExpired() const noexcept override;

    /**!
     * @brief Checks for an expiration overrun past the timer's duration a second time
     *
     * @retval true if timer should have expired twice
     * @retval false otherwise
     */
    bool IsOverExpired() const noexcept override;

    /**!
     * @copydoc Timer::Reset()
     *
     */
    void Reset() noexcept override;

    /**!
     * @brief Modifies the timer duration then resets
     *
     * @tparam Denomination Must meet the named requirements
     * @param duration Expiration duration
     */
    template<typename Denomination>
    void Set(Duration<Denomination> const& duration) noexcept;

private:
    bool        m_is_expired {false};
    Nanoseconds m_duration_ns {0};
    TimeInstant m_expire {0};
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BasicTimer constructor definitions                  ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename Clock>
template<typename Denomination>
BasicTimer<Clock>::BasicTimer(Duration<Denomination> const& duration)
{
    Set(duration);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  BasicTimer method definitions in alphabetical order                     ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  Public      ========================================================================================================

template<typename Clock>
bool BasicTimer<Clock>::IsExpired() const noexcept
{
    if (!m_is_expired)
    {
        TimeInstant now = Clock::Now();
        if (now >= m_expire)
            m_is_expired = true;
    }

    return m_is_expired;
}

template<typename Clock>
bool BasicTimer<Clock>::IsOverExpired() const noexcept
{
    if (IsExpired())
    {
        // Calculate the timer's overage and compare against the set duration
        // If the overage exceeds the original length of the timer, it is over-expired
        TimeInstant now        = Clock::Now();
        Nanoseconds overage_ns = now - m_expire;
        if (overage_ns > m_duration_ns)
            return true;
    }

    return false;
}

template<typename Clock>
void BasicTimer<Clock>::Reset() noexcept
{
    // Use the previous expiration instant as the basis for the next
    // If the timer is over-expired, use the current instant in time instead
    Instant reset_start = m_expire;
    if (IsOverExpired())
        reset_start = Clock::Now();

    m_expire     = reset_start + m_duration_ns;
    m_is_expired = false;
}

template<typename Clock>
template<typename Denomination>
void BasicTimer<Clock>::Set(Duration<Denomination> const& duration) noexcept
{
    Nanoseconds m_duration_ns = duration_cast<Nanoseconds>(duration);
    Reset();
}

} // namespace time
} // namespace shmit
