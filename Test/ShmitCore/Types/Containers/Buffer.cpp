#include <ShmitCore/Types/Containers/Buffer.hpp>

#include <deque>

#include <gtest/gtest.h>

#define TEST_BUF_MAX_SIZE 5

class SequentialBufferTest : public ::testing::Test
{
public:

    SequentialBufferTest()
        : mTestBuf(TEST_BUF_MAX_SIZE)
    {}

protected:

    /**
     * GoogleTest framework interface pure virtual method overrides
     * 
     * Must be implemented for each test framework to compile correctly. They do as the names imply.
     */

    virtual void SetUp() override
    {
        // Do nothing... for now...
    }

    virtual void TearDown() override
    {
        // Do nothing... for now...
    }

    /**
     * Custom test framework methods for use in various instances
     */

    void PopBackSequentially(size_t numPops)
    {
        for (size_t i = 0; i < numPops; i++)
        {
            mTestBuf.pop_back();
            mTruthCheck.pop_back();
        }
    }

    void PopFrontSequentially(size_t numPops)
    {
        for (size_t i = 0; i < numPops; i++)
        {
            mTestBuf.pop_front();
            mTruthCheck.pop_front();
        }
    }

    void PushBackSequentially(int startValue, size_t numElements)
    {
        for (size_t i = 0; i < numElements; i++)
        {
            mTestBuf.push_back(startValue);
            mTruthCheck.push_back(startValue++);
        }
    }

    void PushFrontSequentially(int startValue, size_t numElements)
    {
        for (size_t i = 0; i < numElements; i++)
        {
            mTestBuf.push_front(startValue);
            mTruthCheck.push_front(startValue++);
        }
    }

    std::deque<int> mTruthCheck;   // Expected outcome used to check against the Buffer
    shmit::Buffer<int> mTestBuf;    // Test subject
};

class RandomAccessBufferTest : public SequentialBufferTest
{};

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

TEST_F(SequentialBufferTest, Push_Back_L_Value)
{

}

TEST_F(SequentialBufferTest, Push_Back_R_Value)
{
    
}

TEST_F(SequentialBufferTest, Push_Front_L_Value)
{

}

TEST_F(SequentialBufferTest, Push_Front_R_Value)
{
    
}

TEST_F(SequentialBufferTest, Pushing_Back_Until_Max_Size_Reached_Retains_Intended_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    // Buffer should match the truth check sequence
    EXPECT_EQ(mTestBuf.size(), mTruthCheck.size());

    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Pushing_Front_Until_Max_Size_Reached_Retains_Intended_Order)
{
    PushFrontSequentially(0, TEST_BUF_MAX_SIZE);

    // Buffer should match the truth check sequence
    EXPECT_EQ(mTestBuf.size(), mTruthCheck.size());

    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Clear_Empties_Buffer)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    mTestBuf.clear();

    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
}

TEST_F(SequentialBufferTest, Popping_Back_Removes_Last_Element)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopBackSequentially(1);
    
    // Buffer should not be full and its contents should match the truth check sequence
    EXPECT_EQ(mTestBuf.size(), mTruthCheck.size());
    EXPECT_FALSE(mTestBuf.full());

    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Popping_Front_Removes_First_Element)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopFrontSequentially(1);
    
    // Buffer should not be full and its contents should match the truth check sequence
    EXPECT_EQ(mTestBuf.size(), mTruthCheck.size());
    EXPECT_FALSE(mTestBuf.full());

    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Popping_Back_And_Pushing_Back_Retains_Intended_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopBackSequentially(TEST_BUF_MAX_SIZE - 1);

    int testVal = 69;
    PushBackSequentially(testVal, TEST_BUF_MAX_SIZE - 1);

    // Buffer contents should match the truth check sequence
    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Popping_Back_And_Pushing_Front_Retains_Intended_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopBackSequentially(TEST_BUF_MAX_SIZE - 1);

    int testVal = 69;
    PushFrontSequentially(testVal, TEST_BUF_MAX_SIZE - 1);

    // Buffer should be full again and match the truth check sequence
    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Popping_Front_And_Pushing_Back_Retains_Intended_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopFrontSequentially(TEST_BUF_MAX_SIZE - 1);

    int testVal = 69;
    PushBackSequentially(testVal, TEST_BUF_MAX_SIZE - 1);

    // Buffer should be full again and match the truth check sequence
    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, Popping_Front_And_Pushing_Front_Retains_Intended_Order)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);
    PopFrontSequentially(TEST_BUF_MAX_SIZE - 1);

    int testVal = 69;
    PushFrontSequentially(testVal, TEST_BUF_MAX_SIZE - 1);

    // Buffer should be full again and match the truth check sequence
    std::deque<int>::iterator check = mTruthCheck.begin();
    for (int val : mTestBuf)
        EXPECT_EQ(val, *check++);
}

TEST_F(SequentialBufferTest, If_Empty_Then_Pop_Back_Does_Nothing)
{
    // Buffer is initialized empty by default
    mTestBuf.pop_back();

    // Should still be empty
    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
}

TEST_F(SequentialBufferTest, If_Empty_Then_Pop_Front_Does_Nothing)
{
    // Buffer is initialized empty by default
    mTestBuf.pop_front();

    // Should still be empty
    EXPECT_TRUE(mTestBuf.empty());
    EXPECT_FALSE(mTestBuf.full());
    EXPECT_EQ(mTestBuf.size(), 0);
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

    int testValue = 69;
    mTestBuf.push_front(testValue);
    
    // Buffer should still be full
    // Front of buffer should be the test value
    // Back of buffer should be the second-to-last element in the truth check
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);
    EXPECT_EQ(mTestBuf.front(), testValue);
    EXPECT_EQ(mTestBuf.back(), mTruthCheck[TEST_BUF_MAX_SIZE - 2]);
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

TEST_F(RandomAccessBufferTest, If_Full_Then_Insert_Front_Truncates_Back)
{
    PushBackSequentially(0, TEST_BUF_MAX_SIZE);

    int testValue = 69;
    mTestBuf.insert(mTestBuf.front(), testValue);

    // Buffer should still be full
    // Front of buffer should be the test value
    // Back of buffer should be the second-to-last element in the truth check
    EXPECT_EQ(mTestBuf.size(), TEST_BUF_MAX_SIZE);
    EXPECT_EQ(mTestBuf.front(), testValue);
    EXPECT_EQ(mTestBuf.back(), mTruthCheck[TEST_BUF_MAX_SIZE - 2]);
}

TEST_F(RandomAccessBufferTest, If_Full_Then_Insert_Back_Truncates_Front)
{
    
}

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