#pragma once

namespace shmit
{
namespace FOR
{
namespace cartesian
{

/**
 * Flag to determine which type of axis a set of cartesian data may represent
 */
enum class AxisType : char
{
    eInvalid = 0,

    eLinear     = 1,
    eRotational = 2
};

/**
 * Enumerated indexes for cartesian axis.
 * 
 * Invalid option does not exist since these are used for indexing data containers.
 */
typedef enum
{
    eX      =   0,
    eY      =   1,
    eZ      =   2
} LinearAxis_t;

typedef enum
{
    eYaw    =   0,
    ePitch  =   1,
    eRoll   =   2
} RotationalAxis_t;

#define MAX_NUM_CARTESIAN_AXIS 3

}
}
}