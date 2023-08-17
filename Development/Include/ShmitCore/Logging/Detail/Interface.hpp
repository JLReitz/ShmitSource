#pragma once

#include <ShmitCore/Logging/Posit.hpp>

namespace shmit
{
namespace log
{
namespace detail
{

class Interface
{
protected:
    void LoadTimestamp(Posit& posit);
};

} // namespace detail
} // namespace log
} // namespace shmit