#pragma once

#include "Types.hpp"

#include <limits>
#include <streambuf>

namespace shmit
{
namespace log
{

class Posit
{
    using TimeStamp = uint64_t;

    struct Header
    {
        using Size = uint16_t;

        Size             packet_size;
        TimeStamp        timestamp;
        StringConstantId type_id;
        Severity         severity;
    };

    constexpr static size_t kHeaderSize {help::DataSize_v<Header>};

    class Packet final : public std::basic_streambuf<uint8_t>
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

        constexpr static size_t kMinSize {kPrefixStr.size() + Posit::kHeaderSize + (detail::kElementHeaderSize * 2) +
                                          kPostfixStr.size()};
        constexpr static size_t kMaxSize {std::numeric_limits<Posit::Header::Size>::max()};

        Packet() = delete;

        // TODO populate prefix and postfix strings
        Packet(std::basic_string_view<const_char_type> buffer);

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
    Posit(StringConstantId type_id, Severity severity, std::basic_string_view<Packet::char_type> payload_buffer);

    size_t Size() const noexcept;

    TimeStamp          GetTimestamp() const noexcept;
    Severity           GetSeverity() const noexcept;
    Packet::char_type* GetStartOfData() const noexcept;

    Packet GetPayload() const noexcept;

    std::basic_string_view<Packet::char_type> m_payload_buffer;
};

} // namespace log
} // namespace shmit