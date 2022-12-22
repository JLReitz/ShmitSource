#include <shmitcore/Help/Integer.hpp>

namespace shmit
{

uint64_t to_unsigned(int64_t n)
{
    uint64_t ret = 0;
    if (n > 0)
        ret = static_cast<uint64_t>(n);
    return ret;
}

}