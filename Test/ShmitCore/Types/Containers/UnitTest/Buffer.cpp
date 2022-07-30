#include <ShmitCore/Types/Containers/Buffer.hpp>

#include <gtest/gtest.h>

#define TEST_BUF_MAX_SIZE 5

class SequentialBufferTest : public ::testing::Test
{
protected:

    /**
     * GoogleTest framework interface pure virtual method overrides
     * 
     * Must be implemented for each test framework to compile correctly. They do as the names imply.
     */

    virtual void SetUp() override
    {
        // Initialize the truth check with a simple count-up
        mTruthCheck = new int[TEST_BUF_MAX_SIZE];
        for (size_t i = 0; i < TEST_BUF_MAX_SIZE; i++)
        {
            mTruthCheck[i] = i;
        }

        // Resize the test buffer
        mTestBuf.resize(TEST_BUF_MAX_SIZE);
    }

    virtual void TearDown() override
    {
        // Release truth check
        delete[] mTruthCheck;
    }

    /**
     * Custom test framework methods for use in various instances
     */

    void PushBackSequentially(int startValue, size_t numElements)
    {
        for (size_t i = 0; i < numElements; i++)
        {
            mTestBuf.push_back(startValue++);
        }
    }

    void PushFrontSequentially(int startValue, size_t numElements)
    {
        for (size_t i = 0; i < numElements; i++)
        {
            mTestBuf.push_front(startValue++);
        }
    }

    int* mTruthCheck;               // Expected outcome used to check against the Buffer
    shmit::Buffer<int> mTestBuf;    // Test subject
};

TEST(BufferConstructorTest, Default_Constructor)
{
    shmit::Buffer<int> testBuf;
    
    EXPECT_EQ(testBuf.size(), 0);
    EXPECT_EQ(testBuf.max_size(), 0);
    EXPECT_TRUE(testBuf.empty());
    EXPECT_FALSE(testBuf.full());
}

TEST(BufferConstructorTest, Empty_Upon_Construction)
{
    size_t testBufMaxSize = 5;
    shmit::Buffer<int> testBuf(TEST_BUF_MAX_SIZE);

    EXPECT_EQ(testBuf.size(), 0);
    EXPECT_EQ(testBuf.max_size(), TEST_BUF_MAX_SIZE);
    EXPECT_TRUE(testBuf.empty());
    EXPECT_FALSE(testBuf.full());
}

TEST(BufferConstructorTest, Full_Upon_Construction)
{
    int testBufInitValue = 69;
    shmit::Buffer<int> testBuf(TEST_BUF_MAX_SIZE, testBufInitValue);

    EXPECT_EQ(testBuf.size(), TEST_BUF_MAX_SIZE);
    EXPECT_EQ(testBuf.max_size(), TEST_BUF_MAX_SIZE);
    EXPECT_TRUE(testBuf.full());
    EXPECT_FALSE(testBuf.empty());

    for (int check : testBuf)
    {
        EXPECT_EQ(check, testBufInitValue);
    }
}

// TODO copy and move constructor tests

TEST_F(SequentialBufferTest, If_Empty_Then_Pop_Back_Does_Nothing)
{
    mTestBuf.pop_back();

    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
}

TEST_F(SequentialBufferTest, If_Empty_Then_Pop_Front_Does_Nothing)
{
    mTestBuf.clear();
    mTestBuf.pop_front();

    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
}

TEST_F(SequentialBufferTest, Pushing_Back_Until_Full_Retains_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    // Buffer should be full and match the truth check sequence
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);

    size_t count = 0;
    for (int val : mTestBuf)
    {
        EXPECT_LT(count, TEST_BUF_MAX_SIZE);
        EXPECT_EQ(val, mTruthCheck[count++]);
    }
}

TEST_F(SequentialBufferTest, Pushing_Front_Until_Full_Retains_Order)
{
    PushFrontSequentially(0, TEST_BUF_MAX_SIZE);

    // Buffer should be full and match the truth check sequence in reverse
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);

    size_t count = 0;
    for (int val : mTestBuf)
    {
        EXPECT_LT(count, TEST_BUF_MAX_SIZE);
        EXPECT_EQ(val, mTruthCheck[TEST_BUF_MAX_SIZE - 1 - count++]);
    }
}

TEST_F(SequentialBufferTest, If_Full_Then_Pushing_Back_Truncates_Front)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    int testValue = 69;
    mTestBuf.push_back(testValue);

    // Buffer should still be full
    // Back of buffer should be the test value
    // Front of buffer should be the second element in the truth check
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);
    EXPECT_EQ(mTestBuf.back(), testValue);
    EXPECT_EQ(mTestBuf.front(), mTruthCheck[1]);
}

TEST_F(SequentialBufferTest, If_Full_Then_Pushing_Front_Truncates_Back)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);

    int testValue = 69;
    mTestBuf.push_front(testValue);
    
    // Buffer should still be full
    // Front of buffer should be the test value
    // Back of buffer should be the second-to-last element in the truth check
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);
    EXPECT_EQ(mTestBuf.front(), testValue);
    EXPECT_EQ(mTestBuf.back(), mTruthCheck[TEST_BUF_MAX_SIZE - 2]);
}

TEST_F(SequentialBufferTest, Clear_Empties_Buffer)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    mTestBuf.clear();

    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
}

// TODO tests
// Inserting at valid position retains sequential order
//   - insert at front, back, and middle
//   - single, fill, and range inserts
// Inserting past full truncates back elements
//   - single, fill, and range inserts
// Inserting more than the buffer size fills buffer
// Inserting at invalid position does nothing
//   - iterator from other buffer
//   - iterator from test buffer that was incorrectly initialized

// class RandomAccessBufferTest : public SequentialBufferTest
// {
// protected:

//     void InsertElementsInToTruthCheck(size_t startPosition, size_t n, int value)
//     {
//         // Move elements at and beyond the start position back by n until the end of the array is reached
//         size_t sourcePos = startPosition;
//         size_t destPos = startPosition + n;
//         while (destPos < TEST_BUF_MAX_SIZE)
//             mTruthCheck[destPos++] = mTruthCheck[sourcePos++];

//         // Now insert value in to the insertion zone
//         size_t count = 0;
//         size_t insertPos = startPosition;
//         while ((insertPos < TEST_BUF_MAX_SIZE) && (count < n))
//         {
//             mTruthCheck[insertPos++] = value; 
//             count++;
//         }
//     }

// };

// TEST_F(RandomAccessBufferTest, SingleInsertAtValidPositionRetainsOrder)
// {
//     int insertValue = 69;
//     size_t insertPosition = TEST_BUF_MAX_SIZE / 2;
//     InsertElementsInToTruthCheck(insertPosition, 1, insertValue);

//     shmit::Buffer<int>::iterator insertIt(mTestBuf, mTestBuf._M_wrap_index(insertPosition));
//     PushBackSequentially(0, TEST_BUF_MAX_SIZE);
//     mTestBuf.insert(insertIt, insertValue);

//     PrintBuf(mTestBuf);

//     size_t count = 0;
//     for (int check : mTestBuf)
//     {
//         EXPECT_LT(count, TEST_BUF_MAX_SIZE);
//         EXPECT_EQ(check, mTruthCheck[count++]);
//     }
// }