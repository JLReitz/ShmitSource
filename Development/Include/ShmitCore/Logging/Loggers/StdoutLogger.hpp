#pragma once

#include "OStreamLogger.hpp"

#include <iostream>

namespace shmit
{
namespace log
{

class StdoutLogger : public OStreamLogger<::std::ostream>
{
public:
    StdoutLogger();

    void Load(::std::ostream& oStream) final;
};

} // namespace log
} // namespace shmit