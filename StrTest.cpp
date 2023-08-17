#include <array>
#include <iostream>
#include <string_view>

#include <cstring>

template<typename CharT, CharT... chars>
struct StringConstant
{
private:
    constexpr static size_t kDataSize {sizeof...(chars)};
    constexpr static CharT  kData[kDataSize] {chars...};

public:
    using StringType = std::basic_string_view<CharT>;

    constexpr static StringType value {kData, kDataSize};

    constexpr operator StringType() const noexcept
    {
        return value;
    }

    constexpr StringType operator()() const noexcept
    {
        return value;
    }
};

template<typename CharT, CharT... chars>
constexpr StringConstant<CharT, chars...> operator""_sc()
{
    return {};
}

template<typename... ArgT>
constexpr size_t AggregateSize()
{
    return (sizeof(ArgT) + ...);
};

template<typename Tag, typename Message, typename... ArgT>
struct Posit
{
protected:
    constexpr static size_t kDataSize {AggregateSize<ArgT...>()};

    template<typename T>
    void copy(void* dest, T* src, size_t& n)
    {
        constexpr size_t kSize = sizeof(T);
        (void)std::memcpy(dest, src, kSize);
        n += kSize;
    }

public:
    using TagT     = Tag;
    using MessageT = Message;

    constexpr static typename TagT::StringType     tag {TagT::value};
    constexpr static typename MessageT::StringType message {MessageT::value};

    Posit(ArgT... args)
    {
        size_t n = 0;
        (copy(&data[n], &args, n), ...);
        //(void(std::memcpy(&data[n], &args, sizeof(ArgT))), ..., (n += sizeof(ArgT)));
    }

    std::array<uint8_t, kDataSize> data {};
};

template<typename... ArgT>
struct DataPosit : public Posit<StringConstant<char, '\0'>, StringConstant<char, '\0'>, ArgT...>
{
private:
    using PositT = Posit<StringConstant<char, '\0'>, StringConstant<char, '\0'>, ArgT...>;

public:
    using TagT     = typename PositT::TagT;
    using MessageT = typename PositT::MessageT;

    DataPosit(ArgT... args) : PositT(args...)
    {
    }
};

template<typename StringT>
void print(StringT)
{
    std::cout << StringT::value << std::endl;
}

template<typename PositT>
void log(PositT posit)
{
    std::cout << PositT::tag << std::endl;
    std::cout << PositT::message << std::endl;
    for (uint8_t c : posit.data)
        std::cout << std::hex << static_cast<unsigned int>(c) << ',';
    std::cout << std::endl;
}

#define MAKE_POSIT(name, tag, message, ...) \
    using name##Posit = Posit<decltype(tag##_sc), decltype(message##_sc), __VA_ARGS__>;

#define MAKE_DATA_POSIT(name, ...) using name##Posit = DataPosit<__VA_ARGS__>;

// template<template<typename CharT, CharT... chars> typename Tag, template<typename CharT, CharT... chars> typename
// Message, typename... ArgT> struct make_posit
// {
//     using type = Posit<Tag, Message, ArgT...>;
// }

MAKE_POSIT(Test, "Test", "This is a test", int, bool)
MAKE_DATA_POSIT(TestData, int, bool)

int main()
{
    print("Hello world\n"_sc);
    log(TestPosit(1024, true));
    log(TestDataPosit(512, false));
    return 0;
}