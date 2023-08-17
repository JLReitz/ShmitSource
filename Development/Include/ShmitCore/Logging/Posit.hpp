#pragma once

#include "Common.hpp"
#include "Element.hpp"

#include <limits>
#include <streambuf>

namespace shmit
{
namespace log
{

struct Posit
{
    using TimeStamp = uint64_t;

    struct Header
    {
        using PayloadSizeType = uint16_t;

        PayloadSizeType  packet_size;
        TimeStamp        timestamp;
        StringConstantId type_id;
        Level            severity;
    };

    struct ElementHeader
    {
        StringConstantId tag_id;
        StringConstantId string_id;
    };

    constexpr static size_t kHeaderSize {help::DataSize_v<Header>};
    constexpr static size_t kElementHeaderSize {help::DataSize_v<ElementHeader>};

    class Payload final : public std::basic_streambuf<uint8_t>
    {
    public:
        using StreamBuf = std::basic_streambuf<uint8_t>;

        using char_type   = typename StreamBuf::char_type;
        using int_type    = typename StreamBuf::int_type;
        using traits_type = typename StreamBuf::traits_type;

        using const_char_type      = std::add_const_t<char_type>;
        using reference_type       = std::add_lvalue_reference_t<char_type>;
        using const_reference_type = std::add_lvalue_reference_t<const_char_type>;

        constexpr static std::string_view kPrefixStr {"\\P{"};
        constexpr static std::string_view kPostfixStr {"}\\"};

        constexpr static size_t kMinSize {kPrefixStr.size() + Posit::kHeaderSize + (Posit::kElementHeaderSize * 2) +
                                          kPostfixStr.size()};
        constexpr static size_t kMaxSize {std::numeric_limits<Posit::Header::PayloadSizeType>::max()};

        Payload() = delete;

        // TODO populate prefix and postfix strings
        Payload(std::basic_string_view<const_char_type> buffer);

    private:
        /**
         * Methods that do nothing in the base class and do not need to be overridden
         * - setbuf()
         * - seekoff() ?
         * - seekpos() ?
         * - sync()
         * - overflow()
         * - pbackfail()
         */

        /**
         * Methods in the base class that do not need to be overridden
         * - eback()
         * - gptr()
         * - egptr()
         * - gbump()
         * - setg()
         * - xsputn()
         * - pbase()
         * - pptr()
         * - epptr()
         * - pbump() ?
         * - setp()
         */

        std::streamsize showmanyc() override noexcept;
        int_type        underflow() override noexcept;
        int_type        uflow() override noexcept;
        std::streamsize xsgetn(char_type* s, std::streamsize count) override noexcept;

        std::basic_string_view<const_char_type> m_buffer {};
    };

    // New
    Posit(StringConstantId type_id, Level severity, std::basic_string_view<Payload::char_type> payload_buffer);

    TimeStamp           Timestamp() const noexcept;
    Level               Severity() const noexcept;
    Payload::char_type* ElementStart() const noexcept;

    Payload GetPayload() const noexcept;

    std::basic_string_view<Payload::char_type> m_payload_buffer;
};

} // namespace log
} // namespace shmit