#pragma once

#include <ShmitCore/Help/String.hpp>

#include <type_traits>

namespace shmit
{

/**!
 * @brief A class with individually named instances
 *
 */
class Named
{
public:
    static constexpr size_t kMaxSize = 32; /*! A name may be, at max, 32 characters long */

    Named() = default;

    /**!
     * @brief Construct a Named object with designated name
     *
     * @param[in] name Name of instance
     */
    Named(char const* name);

    /**!
     * @brief Get the name of the instance
     *
     * @return char const* C-style string
     */
    char const* GetName() const;

    /**!
     * @brief Sets the name of the instance
     *
     * @param[in] name Name of instance
     */
    void SetName(char const* name);

    /**!
     * @brief Gets the length of the instance's name
     *
     * @return size_t
     */
    size_t NameLength() const;

private:
    size_t m_name_length {0};
    char   m_name[kMaxSize + 1] {0}; // +1 to max length for null termination
};

/**!
 * @brief Get the name of an instance, if its type is derived from Named
 *
 * @tparam T Any type
 * @param[in] instance Reference to the instance
 * @param[out] name Outbound c-string of the instance's name, if it exists, or the instances location in memory if not
 * @return size_t Number of characters written to the c-string
 */
template<typename T>
size_t get_name_of_instance(T const& instance, char* name);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Metafunction definitions            ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**!
 * @brief Check for if a type is derived from Named
 *
 * @tparam T Any type
 */
template<typename T>
using is_named = std::is_base_of<Named, T>;

/**!
 * @brief Accesses the returned type from is_named
 *
 * @tparam T Any type
 */
template<typename T>
using is_named_t = typename std::is_base_of<Named, T>::type;

/**!
 * @brief Accesses the returned value from is_named
 *
 * @tparam T Any type
 */
template<typename T>
inline constexpr bool is_named_v = std::is_base_of<Named, T>::value;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Namespace function definitions in alphabetical order        ////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
inline size_t get_name_of_instance(T const& instance, char* name)
{
    size_t char_count = 0;
    if constexpr (is_named_v<T>)
    {
        // If the instance's type is derived from Named, log its name... if it is valid.
        // Otherwise substitute the instance's name for its location in memory
        if (instance.NameLength() > 0)
            char_count = protected_snprintf(name, Named::kMaxSize, "%s", instance.GetName());
        else
            char_count = protected_snprintf(name, Named::kMaxSize, "%p", &instance);
    }
    // Else, if the instance cannot be named, just print its location
    else
        char_count = protected_snprintf(name, Named::kMaxSize, "%p", &instance);

    return char_count;
}

} // namespace shmit