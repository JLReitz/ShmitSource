#pragma once

#include <deque>
#include <utility>

#include <gtest/gtest.h>

#ifndef MAX_TEST_CONTAINER_SIZE
#define MAX_TEST_CONTAINER_SIZE 5
#endif

template <class ContainerTestModule>
class SequenceContainerTest : public ::testing::Test
{
protected:

    using ElementType = typename ContainerTestModule::value_type;
    using TruthCheckType = std::deque<ElementType>;

    void PushBackSequentially(ContainerTestModule& module, size_t numPushes, bool isRValue = false)
    {
        for (size_t i = 0; i < numPushes; i++)
        {
            ElementType pushBackValue = module.GetPushValue();

            if (isRValue)
            {
                mTruthCheck.push_back(std::move(pushBackValue));
                module.push_back(std::move(pushBackValue));
            }
            else
            {
                mTruthCheck.push_back(pushBackValue);
                module.push_back(pushBackValue);
            }
        }
    }

    void PushFrontSequentially(ContainerTestModule& module, size_t numPushes, bool isRValue = false)
    {
        for (size_t i = 0; i < numPushes; i++)
        {
            ElementType pushFrontValue = ContainerTestModule::GetPushValue();

            if (isRValue)
            {
                mTruthCheck.push_front(std::move(pushFrontValue));
                module.push_front(std::move(pushFrontValue));
            }
            else
            {
                mTruthCheck.push_front(pushFrontValue);
                module.push_front(pushFrontValue);
            }
        }
    }

    TruthCheckType mTruthCheck;
};

// Declare type-parameterized test suite
TYPED_TEST_SUITE_P(SequenceContainerTest);

//  Constructor & assignment tests  ===================================================================================


TYPED_TEST_P(SequenceContainerTest, Default_Constructor)
{
    TypeParam testModule;

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Iterator_Constructors)
{
    using IlType = std::initializer_list<typename TestFixture::ElementType>;
    using IlIterator = typename IlType::iterator;

    // Test initializer list constructor
    IlType il = TypeParam::GetInitializerList();
    TypeParam testModule = il;

    // Use assertions for the first set of tests, if they fail there's no point testing the next
    ASSERT_EQ(testModule.size(), il.size());

    typename IlType::iterator ilCurr = il.begin();
    for (const typename TestFixture::ElementType& value : testModule)
    {
        ASSERT_NE(ilCurr, il.end());
        ASSERT_EQ(value, *ilCurr++);
    }

    // Test range constructor
    TypeParam testModule2(testModule.begin(), testModule.end());

    EXPECT_EQ(testModule2.size(), il.size());

    ilCurr = il.begin();
    for (const typename TestFixture::ElementType& value : testModule2)
    {
        ASSERT_NE(ilCurr, il.end());
        EXPECT_EQ(value, *ilCurr++);
    }
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor)
{
    TypeParam rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    TypeParam testModule(rhs);

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Default_Constructed_Rhs)
{
    TypeParam rhs;
    TypeParam testModule(rhs);

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Empty_Rhs)
{
    TypeParam rhs(MAX_TEST_CONTAINER_SIZE);
    TypeParam testModule(rhs);

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor)
{
    // TODO initializer list constructor
    TypeParam testModule(TypeParam(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue()));

    EXPECT_EQ(testModule.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Default_Constructed_Rhs)
{
    // TODO initializer list constructor
    TypeParam rhs;
    TypeParam testModule(std::move(rhs));

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Empty_Rhs)
{
    // TODO initializer list constructor
    TypeParam testModule(TypeParam(MAX_TEST_CONTAINER_SIZE));

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment)
{
    TypeParam rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    TypeParam testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Default_Constructed_Rhs)
{
    TypeParam rhs;
    TypeParam testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Empty_Rhs)
{
    TypeParam rhs(MAX_TEST_CONTAINER_SIZE);
    TypeParam testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment)
{
    TypeParam testModule;

    testModule = TypeParam(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());

    EXPECT_EQ(testModule.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Default_Constructed_Rhs)
{
    TypeParam testModule;

    testModule = TypeParam();

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Empty_Rhs)
{
    TypeParam testModule;

    testModule = TypeParam(MAX_TEST_CONTAINER_SIZE);

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

//  Iterator tests  ===================================================================================================

TYPED_TEST_P(SequenceContainerTest, If_Empty_Iterator_Can_Not_Increment_Forward)
{
    TypeParam testModule;

    size_t count = 0;
    for (typename TestFixture::ElementType& value : testModule)
        count++;

    ASSERT_EQ(count, 0);
}

//  Functional tests    ===============================================================================================

TYPED_TEST_P(SequenceContainerTest, Push_Back_L_Value)
{
    TypeParam testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(testModule, MAX_TEST_CONTAINER_SIZE);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Back_R_Value)
{
    TypeParam testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(testModule, MAX_TEST_CONTAINER_SIZE, true);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_L_Value)
{
    TypeParam testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushFrontSequentially(testModule, MAX_TEST_CONTAINER_SIZE);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_R_Value)
{
    TypeParam testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushFrontSequentially(testModule, MAX_TEST_CONTAINER_SIZE, true);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

//  End of tests    ===================================================================================================

// Register tests to test suite
REGISTER_TYPED_TEST_SUITE_P(SequenceContainerTest,
                            Default_Constructor,
                            Iterator_Constructors,
                            Copy_Constructor,
                            Copy_Constructor_With_Default_Constructed_Rhs,
                            Copy_Constructor_With_Empty_Rhs,
                            Move_Constructor,
                            Move_Constructor_With_Default_Constructed_Rhs,
                            Move_Constructor_With_Empty_Rhs,
                            Copy_Assignment,
                            Copy_Assignment_With_Default_Constructed_Rhs,
                            Copy_Assignment_With_Empty_Rhs,
                            Move_Assignment,
                            Move_Assignment_With_Default_Constructed_Rhs,
                            Move_Assignment_With_Empty_Rhs,
                            If_Empty_Iterator_Can_Not_Increment_Forward,
                            Push_Back_L_Value,
                            Push_Back_R_Value,
                            Push_Front_L_Value,
                            Push_Front_R_Value);
