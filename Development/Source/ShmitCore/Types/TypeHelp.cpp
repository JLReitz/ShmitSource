#include <ShmitCore/Types/TypeHelp.hpp>

namespace shmit
{
namespace platform
{
namespace help
{

const size_t byteSizeInBits = 8; // Prove me wrong

size_t staticPlatformCharSizeInBytes = 0;
size_t staticPlatformReferenceSizeInBytes = 0;

size_t CharSizeInBytes()
{
    // This condition should be run once upon the first time this function is called
    if (staticPlatformCharSizeInBytes == 0)
    {
        size_t platformCharSizeInBits = __CHAR_BIT__;
        staticPlatformCharSizeInBytes = platformCharSizeInBits / byteSizeInBits;
    }

    return staticPlatformCharSizeInBytes;
}

size_t ReferenceSizeInBytes()
{
    // This condition should be run once upon the first time this function is called
    if (staticPlatformReferenceSizeInBytes == 0)
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
        staticPlatformReferenceSizeInBytes = referenceSizeInMemUnits * CharSizeInBytes();
    }

    return staticPlatformReferenceSizeInBytes;
}

}
}
}