#include <Fixtures/Containers/SequenceContainerTest.hpp>

#include <ShmitCore/Types/Containers/Buffer.hpp>

int gIncrementingGetIntValue = 69; // Value increments every time GetPushValue() or GetInsertValue() is called

/**
 * @brief Test module for shmit::Buffer<int>. Fulfils the named reqiurements of template `SequenceContainerTestModule`
 * for fixture 'SequenceContainerTest`.
 */
class IntSpecialization
{
public:
    using ContainerType = shmit::Buffer<int>;

    static int GetInitializationValue()
    {
        // Buffers initialized with this value will contain all 1's
        return 1;
    }

    static int GetAssignValue()
    {
        return gIncrementingGetIntValue;
    }

    static int GetInsertValue()
    {
        return gIncrementingGetIntValue++;
    }

    static int GetPushValue()
    {
        return gIncrementingGetIntValue++;
    }

    static std::initializer_list<int> GetInitializerList()
    {
        return mIl;
    }

    static size_t InitializerListSize()
    {
        return mIl.size();
    }

private:
    static constexpr std::initializer_list<int> mIl = {1, 2, 3, 4, 5}; // Buffer test initializer list
};

// Implement type-parameterized STL container test fixtures relevant to shmit::Buffer with the specialized test module
// above
INSTANTIATE_TYPED_TEST_SUITE_P(Buffer, SequenceContainerTest, IntSpecialization);