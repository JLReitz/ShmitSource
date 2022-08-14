#include <Fixtures/Containers/SequenceContainerTest.hpp>

#include <ShmitCore/Types/Containers/Buffer.hpp>

int gIncrementingGetValue = 69; // Value increments every time GetPushValue() or GetInsertValue() is called

class IntSpecializationTestModule : public shmit::Buffer<int>
{
public:
    IntSpecializationTestModule() : shmit::Buffer<int>()
    {
    }

    IntSpecializationTestModule(size_t bufferSize) : shmit::Buffer<int>(bufferSize)
    {
    }

    IntSpecializationTestModule(size_t bufferSize, const int& init) : shmit::Buffer<int>(bufferSize, init)
    {
    }

    IntSpecializationTestModule(std::initializer_list<int> il) : shmit::Buffer<int>(il)
    {
    }

    template<class IteratorTypeArg>
    IntSpecializationTestModule(IteratorTypeArg i, IteratorTypeArg j) : shmit::Buffer<int>(i, j)
    {
    }

    static int GetInitializationValue()
    {
        // Buffers initialized with this value will contain all 1's
        return 1;
    }

    static std::initializer_list<int> GetInitializerList()
    {
        return mIl;
    }

    static int GetPushValue()
    {
        return gIncrementingGetValue++;
    }

    static int GetInsertValue()
    {
        return gIncrementingGetValue++;
    }

    bool operator==(const IntSpecializationTestModule& rhs) const
    {
        return ((const shmit::Buffer<int>&)*this == (const shmit::Buffer<int>&)rhs);
    }

private:
    static constexpr std::initializer_list<int> mIl = {1, 2, 3, 4, 5}; // Buffer test initializer list
};

INSTANTIATE_TYPED_TEST_SUITE_P(Buffer, SequenceContainerTest, IntSpecializationTestModule);