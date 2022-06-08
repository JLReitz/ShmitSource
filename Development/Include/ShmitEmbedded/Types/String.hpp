#pragma once

/**
 * This uninspired take on one of the most common band-aids in modern C++ is meant to provide users of ShmitEmbedded 
 * the opportunity to do the same. To compliment the lack of high level string support within microcontroller-ported 
 * STD libs, a mockup of std::string with a 1:1 interface is provided to those who use the 'EMBEDDED_PLATFORM' flag 
 * with their compiler. Those who don't can still use the shmit::String symbol around their codebase but it will 
 * instead be a typedef of std::string.
 */

// #ifdef EMBEDDED_PLATFORM

// #else
// #include <string>
// #endif

namespace shmit
{

//#ifdef EMBEDDED_PLATORM

/**
 * @brief A mockup of std::string with a 1:1 interface for use in embedded projects
 */
class String
{
public:
};

// #else

// //If not compiling for an embedded project, redefine std::string as shmit::String
// typedef std::string String;

// #endif

}