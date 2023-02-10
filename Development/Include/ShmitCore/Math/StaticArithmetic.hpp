#pragma once

namespace shmit
{
namespace math
{

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Arithmetic metafunction prototypes                  ////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Produces the sum of two compile-time arithmetic-compatible values
 *
 * @tparam Augend
 * @tparam Addend
 */
template<typename Augend, typename Addend>
struct add
{
};

/**!
 * @brief Produces the difference between two compile-time arithmetic-compatible values
 *
 * @tparam Minuend
 * @tparam Subtrahend
 */
template<typename Minuend, typename Subtrahend>
struct subtract
{
};

/**!
 * @brief Produces the product of two compile-time arithmetic-compatible values
 *
 * @tparam Multiplicand
 * @tparam Multiplier
 */
template<typename Multiplicand, typename Multiplier>
struct multiply
{
};

/**!
 * @brief Produces the quotient of two compile-time arithmetic-compatible values
 *
 * @tparam Dividend
 * @tparam Divisor
 */
template<typename Dividend, typename Divisor>
struct divide
{
};

/**!
 * @brief Convenience alias for accessing the returned type from shmit::math::add
 *
 * @tparam Augend
 * @tparam Addend
 */
template<typename Augend, typename Addend>
using add_t = typename add<Augend, Addend>::type;

/**!
 * @brief Convenience alias for accessing the returned type from shmit::math::subtract
 *
 * @tparam Augend
 * @tparam Addend
 */
template<typename Augend, typename Addend>
using subtract_t = typename subtract<Augend, Addend>::type;

/**!
 * @brief Convenience alias for accessing the returned type from shmit::math::multiply
 *
 * @tparam Augend
 * @tparam Addend
 */
template<typename Augend, typename Addend>
using multiply_t = typename multiply<Augend, Addend>::type;

/**!
 * @brief Convenience alias for accessing the returned type from shmit::math::divide
 *
 * @tparam Augend
 * @tparam Addend
 */
template<typename Augend, typename Addend>
using divide_t = typename divide<Augend, Addend>::type;

} // namespace math
} // namespace shmit