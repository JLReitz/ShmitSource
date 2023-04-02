#pragma once

#include "Instant.hpp"

namespace shmit
{
namespace time
{

class Timer
{
public:
    virtual bool IsExpired() const noexcept     = 0;
    virtual bool IsOverExpired() const noexcept = 0;

    template<typename Denomination>
    virtual void Reset(Duration<Denomination> const& duration) noexcept = 0;
};

class PeriodicTimer final : public Timer
{
public:
    Periodic() = delete;

    Periodic(Timer& timer);

    bool IsExpired() const noexcept override;
    bool IsOverExpired() const noexcept override;

    template<typename Denomination>
    void Reset(Duration<Denomination> const& duration) noexcept override;

private:
    Timer&      m_timer;
    Nanoseconds m_duration_ns;
};

template<typename Clock>
class BasicTimer : public Timer
{
public:
    using Instant = Clock::Instant;

    bool IsExpired() const noexcept override;
    bool IsOverExpired() const noexcept override;

    template<typename Denomination>
    void Reset(Duration<Denomination> const& duration) noexcept override;

private:
    bool           m_is_expired {true};
    Instant<Clock> m_expire {0};
};

template<typename Clock>
bool BasicTimer<Clock>::IsExpired() const noexcept override
{
}

template<typename Clock>
bool BasicTimer<Clock>::IsOverExpired() const noexcept override
{
}

template<typename Clock>
template<typename Denomination>
void BasicTimer<Clock>::Reset(Duration<Denomination> const& duration) noexcept override
{
    Instant now  = Clock::now();
    m_expire     = now + duration;
    m_is_expired = false;
}

} // namespace time
} // namespace shmit
