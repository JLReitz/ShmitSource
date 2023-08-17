#include <ShmitCore/StdTypes.hpp>

#include <type_traits>
#include <utility>

namespace shmit
{
namespace help
{

template<typename T>
using add_const_lvalue_reference = std::add_lvalue_reference<std::add_const_t<T>>;

template<typename T>
using add_const_lvalue_reference_t = typename add_const_lvalue_reference<T>::type;

namespace detail
{

template<typename Rep, typename... T>
struct DataSize
{
    // TODO add static asserts
    // - Rep must be integral type
    // - Rep must be trivially constructable?
    // - Size calculations depend on the underlying platform size

    using rep = Rep;

    constexpr static rep value {sizeof(std::remove_reference_t<T>) + ...};
    using type = typename std::integral_constant<rep, value>;
};

template<typename T, typename Rep>
inline void DataCopy(void* dest, T* src, typename DataSize<Rep, T>::rep& n);

template<typename Underlying, typename... T>
struct DataPack
{
    // TODO add static asserts
    // - Underlying must be fundamental type
    using DataSize = typename DataSize<size_t, T...>::type;
    using rep      = Underlying;

    constexpr static DataSize::rep kSize {DataSize::value};
    rep                            data[kSize] {};
};

template<typename Underlying, typename... T>
void encode_data_pack(DataPack<Underlying, T...>& data_pack, const T&... args);

template<typename... Args, typename Func, std::size_t... Indices>
void tuple_for_each(const std::tuple<Args...>& t, Func&& func, std::index_sequence<Indices...>)
{
    (func(std::get<Indices>(t)), ...);
}

template<typename... Args, typename Func>
void tuple_for_each(const std::tuple<Args...>& t, Func&& func)
{
    tuple_for_each(t, std::forward<Func>(func), std::index_sequence_for<Args...>());
}

} // namespace detail

template<typename... T>
using DataSize = detail::DataSize<size_t, T...>;

template<typename... T>
using DataPack = detail::DataPack<uint8_t, T...>;

template<typename... T>
using DataSize_t = typename DataSize<T...>::type;

template<typename... T>
constexpr static typename DataSize<T...>::rep DataSize_v {DataSize<T...>::value};

template<typename... T>
constexpr DataPack<T...> make_data_pack(const T&... args);

namespace detail
{

template<typename T, typename Rep>
inline void DataCopy(void* dest, T* src, typename DataSize<Rep, T>::rep& n)
{
    constexpr typename DataSize<Rep, T>::rep kSize {DataSize_v<T>};
    (void)std::memcpy(dest + n, src, kSize);
    n += kSize;
}

template<typename Underlying, typename... T>
inline void encode_data_pack(DataPack<Underlying, T...>& data_pack, const T&... args)
{
    size_t offset {0};
    (DataCopy<T, Underlying>(data_pack.data, args..., offset), ...);
}

} // namespace detail

template<typename... T>
constexpr DataPack<T...>&& make_data_pack(const T&... args)
{
    DataPack<T...> data_pack {};
    detail::encode_data_pack(data_pack, args...);
    return std::move(data_pack);
}

} // namespace help
} // namespace shmit