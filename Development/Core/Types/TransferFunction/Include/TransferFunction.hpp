#pragma once

namespace shmit
{
namespace tf
{

class TransferFunction
{
public:

    virtual double Cycle(double input) = 0;

    virtual double LastOutput() const = 0;
};

}
}