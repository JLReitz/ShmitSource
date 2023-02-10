#include <ShmitCore/Time/Instant.hpp>

namespace shmit
{
namespace time
{

template<class Clock>
template<class Denomination>
Instant<Clock>::operator Duration<Denomination>() const noexcept
{
    using DenominationRep             = typename Denomination::Rep;
    using DenominationCountsPerSecond = typename Denomination::ToBase;

    // Divide clock's tick rate by the Denomination's conversion rate to get the full conversion ratio
    using Conversion = typename math::Divide<TicksPerSecond, DenominationCountsPerSecond>::Result;

    // Conversion ratio runtime value is guaranteed to be positive, per Denomination's compile-time checks
    // Because of this, we can safely cast the numerator and denominator components to unsigned
    constexpr math::Ratio conversion     = Conversion::value;
    constexpr uintmax_t   conversion_num = static_cast<uintmax_t>(conversion.numerator);
    constexpr uintmax_t   conversion_den = static_cast<uintmax_t>(conversion.denominator);

    // Perform conversion on unsigned ticks in backwards order before casting to Denomination's representation type.
    // This is because the conversion ratio will ~most~ likely be < 1, so we can expect the result to become smaller,
    // increasing the likelyhood that the following cast to the Denomination's representation type will not incur an overflow.
    uintmax_t       duration_count = (m_ticks / conversion_num) * conversion_num;
    DenominationRep duration_rep   = static_cast<DenominationRep>(duration_count);

    return Duration<Denomination>(duration_rep);
}

} // namespace time
} // namespace shmit