#include <ShmitCore/Platform/TypeHelp.hpp>

namespace shmit
{
namespace platform
{
namespace help
{

const size_t byteSizeInBits = 8; // Prove me wrong

size_t CharSizeInBytes()
{
    size_t platformCharSizeInBits = __CHAR_BIT__;
    return platformCharSizeInBits / byteSizeInBits;
}

size_t ReferenceSizeInBytes()
{
    /**
     * Character type size depends on the platform architecture, it may not always be 8 bits. 
     * 
     * This is why we check:
     * Chars are pretty much guaranteed to be the smallest primitive possible ('sizeof(char) == 1' should always
     * return true). This means that 'sizeof()', for all intents and purposes, returns an object's size not in bytes 
     * but in chars (think of them as memory units). We can use that to guarantee that we calculate its true size in 
     * bytes.
     */
    size_t referenceSizeInMemUnits = sizeof(void*);
    return referenceSizeInMemUnits * CharSizeInBytes();
}

}
}
}