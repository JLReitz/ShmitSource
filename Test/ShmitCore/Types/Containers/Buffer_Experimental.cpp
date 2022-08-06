#include <Fixtures/Containers/SequenceContainerTest.hpp>

#include <ShmitCore/Types/Containers/Buffer.hpp>

int gPushValueCount = 69;

class IntSpecializationTestModule : public shmit::Buffer<int>
{
public:

    IntSpecializationTestModule()
        :   shmit::Buffer<int>()
    {}

    IntSpecializationTestModule(size_t bufferSize)
        :   shmit::Buffer<int>(bufferSize)
    {}

    IntSpecializationTestModule(size_t bufferSize, const int& init)
        :   shmit::Buffer<int>(bufferSize, init)
    {}

    IntSpecializationTestModule(std::initializer_list<int> il)
        :   shmit::Buffer<int>(il)
    {}

    static int GetInitializationValue()
    {
        return 1;
    }

    static std::initializer_list<int> GetInitializerList()
    {
        return mIl;
    }

    static int GetPushValue()
    {
        return gPushValueCount++;
    }

    bool operator==(const IntSpecializationTestModule& rhs) const
    {
        return ((const shmit::Buffer<int>&)*this == (const shmit::Buffer<int>&)rhs);
    }

private:

    static constexpr std::initializer_list<int> mIl = {1, 2, 3, 4, 5};
};

INSTANTIATE_TYPED_TEST_SUITE_P(Buffer, SequenceContainerTest, IntSpecializationTestModule);