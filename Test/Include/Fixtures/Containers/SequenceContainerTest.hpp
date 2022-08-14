#pragma once

#include <deque>
#include <utility>

#include <gtest/gtest.h>

#ifndef MAX_TEST_CONTAINER_SIZE
    #define MAX_TEST_CONTAINER_SIZE 5
#endif

template<class SequenceContainerTestModule>
class SequenceContainerTest : public SequenceContainerTestModule, public ::testing::Test
{
protected:
    using ContainerType         = typename SequenceContainerTestModule::ContainerType;
    using ContainerIteratorType = typename ContainerType::iterator;

    using ElementType = typename ContainerType::value_type;

    using TruthCheckType         = std::deque<ElementType>; // Known-working STL sequence container for verification
    using TruchCheckIteratorType = typename TruthCheckType::iterator;

    void PushBackSequentially(ContainerType& container, size_t numPushes, bool isRValue = false)
    {
        for (size_t i = 0; i < numPushes; i++)
        {
            ElementType pushBackValue = SequenceContainerTestModule::GetPushValue();

            if (isRValue)
            {
                // Create copy of value to push to both containers
                ElementType pushBackValue2 = pushBackValue;

                mTruthCheck.push_back(std::move(pushBackValue));
                container.push_back(std::move(pushBackValue2));
            }
            else
            {
                mTruthCheck.push_back(pushBackValue);
                container.push_back(pushBackValue);
            }
        }
    }

    void PushFrontSequentially(ContainerType& container, size_t numPushes, bool isRValue = false)
    {
        for (size_t i = 0; i < numPushes; i++)
        {
            ElementType pushFrontValue = SequenceContainerTestModule::GetPushValue();

            if (isRValue)
            {
                // Create copy of value to push to both containers
                ElementType pushFrontValue2 = pushFrontValue;

                mTruthCheck.push_front(std::move(pushFrontValue));
                container.push_front(std::move(pushFrontValue2));
            }
            else
            {
                mTruthCheck.push_front(pushFrontValue);
                container.push_front(pushFrontValue);
            }
        }
    }

    void InsertValueAtIndexedPosition(ContainerType& container, size_t position, bool isRValue = false)
    {
        ContainerIteratorType  containerPos  = container.begin() + position;
        TruchCheckIteratorType truthCheckPos = mTruthCheck.begin() + position;

        ElementType insertValue = SequenceContainerTestModule::GetInsertValue();

        if (isRValue)
        {
            // Create copy of value to push to both containers
            ElementType insertValue2 = insertValue;

            mTruthCheck.insert(truthCheckPos, std::move(insertValue));
            container.insert(containerPos, std::move(insertValue2));
        }
        else
        {
            mTruthCheck.insert(truthCheckPos, insertValue);
            container.insert(containerPos, insertValue);
        }
    }

    void InsertFillAtIndexedPosition(ContainerType& container, size_t position, size_t n)
    {
        ContainerIteratorType  containerPos  = container.begin() + position;
        TruchCheckIteratorType truthCheckPos = mTruthCheck.begin() + position;

        ElementType insertValue = SequenceContainerTestModule::GetInsertValue();

        mTruthCheck.insert(truthCheckPos, n, insertValue);
        container.insert(containerPos, n, insertValue);
    }

    void InsertRangeAtIndexedPosition(ContainerType& container, size_t position)
    {
        using IlType = std::initializer_list<ElementType>;

        ContainerIteratorType  containerPos  = container.begin() + position;
        TruchCheckIteratorType truthCheckPos = mTruthCheck.begin() + position;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        mTruthCheck.insert(truthCheckPos, il.begin(), il.end());
        container.insert(containerPos, il.begin(), il.end());
    }

    void InsertInitListAtIndexedPosition(ContainerType& container, size_t position)
    {
        using IlType = std::initializer_list<ElementType>;

        ContainerIteratorType  containerPos  = container.begin() + position;
        TruchCheckIteratorType truthCheckPos = mTruthCheck.begin() + position;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        mTruthCheck.insert(truthCheckPos, il);
        container.insert(containerPos, il);
    }

    TruthCheckType mTruthCheck;
};

// Declare type-parameterized test suite
TYPED_TEST_SUITE_P(SequenceContainerTest);

//  Constructor & assignment tests  ====================================================================================

TYPED_TEST_P(SequenceContainerTest, Default_Constructor)
{
    typename TestFixture::ContainerType testModule;

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Iterator_Constructors)
{
    using IlType     = std::initializer_list<typename TestFixture::ElementType>;
    using IlIterator = typename IlType::iterator;

    // Test initializer list constructor
    IlType                              il         = TestFixture::GetInitializerList();
    typename TestFixture::ContainerType testModule = il;

    // Use assertions for the first set of tests, if they fail there's no point testing the next
    ASSERT_EQ(testModule.size(), il.size());

    typename IlType::iterator ilCurr = il.begin();
    for (const typename TestFixture::ElementType& value : testModule)
    {
        ASSERT_NE(ilCurr, il.end());
        ASSERT_EQ(value, *ilCurr++);
    }

    // Test range constructor
    typename TestFixture::ContainerType testModule2(testModule.begin(), testModule.end());

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
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    typename TestFixture::ContainerType testModule(rhs);

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType testModule(rhs);

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Empty_Rhs)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType testModule(std::move(rhs));

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TestFixture::GetInitializationValue());
    typename TestFixture::ContainerType testModule(std::move(rhs));

    EXPECT_EQ(testModule.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Default_Constructed_Rhs)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType testModule(std::move(rhs));

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Empty_Rhs)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType testModule(std::move(rhs));

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    typename TestFixture::ContainerType testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Empty_Rhs)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType testModule;

    testModule = rhs;

    EXPECT_TRUE(testModule == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment)
{
    typename TestFixture::ContainerType testModule;

    testModule = typename TestFixture::ContainerType(MAX_TEST_CONTAINER_SIZE, TestFixture::GetInitializationValue());

    EXPECT_EQ(testModule.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType testModule;

    testModule = typename TestFixture::ContainerType();

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Empty_Rhs)
{
    typename TestFixture::ContainerType testModule;

    testModule = typename TestFixture::ContainerType(MAX_TEST_CONTAINER_SIZE);

    EXPECT_TRUE(testModule.empty());
    EXPECT_EQ(testModule.size(), 0);
}

//  Iterator tests  ====================================================================================================

TYPED_TEST_P(SequenceContainerTest, If_Container_Is_Empty_Then_Iterator_Can_Not_Increment_Forward)
{
    typename TestFixture::ContainerType testModule;

    size_t count = 0;
    for (typename TestFixture::ElementType& value : testModule)
        count++;

    ASSERT_EQ(count, 0);
}

TYPED_TEST_P(SequenceContainerTest, If_Already_At_End_Then_Iterator_Can_Not_Increment_Forward)
{
    typename TestFixture::ContainerType         testModule(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerIteratorType containerEnd = testModule.end();

    containerEnd++;
    ASSERT_EQ(containerEnd, testModule.end());
}

//  Functional tests ===================================================================================================

TYPED_TEST_P(SequenceContainerTest, Push_Back_L_Value)
{
    typename TestFixture::ContainerType testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(testModule, MAX_TEST_CONTAINER_SIZE);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Back_R_Value)
{
    typename TestFixture::ContainerType testModule(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->PushBackSequentially(testModule, MAX_TEST_CONTAINER_SIZE, true);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_L_Value)
{
    typename TestFixture::ContainerType testModule(MAX_TEST_CONTAINER_SIZE);

    this->PushFrontSequentially(testModule, MAX_TEST_CONTAINER_SIZE);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_R_Value)
{
    typename TestFixture::ContainerType testModule(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->PushFrontSequentially(testModule, MAX_TEST_CONTAINER_SIZE, true);

    // Test container should match the truth check sequence
    EXPECT_EQ(testModule.size(), this->mTruthCheck.size());
    for (size_t i = 0; i < MAX_TEST_CONTAINER_SIZE; i++)
        EXPECT_EQ(testModule[i], this->mTruthCheck[i]);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Back_L_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    this->InsertValueAtIndexedPosition(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    ASSERT_EQ(testContainer.back(), this->mTruthCheck.back());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Back_R_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->InsertValueAtIndexedPosition(testContainer, MAX_TEST_CONTAINER_SIZE - 1, true);

    ASSERT_EQ(testContainer.back(), this->mTruthCheck.back());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Fill_Back)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fillCount = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(testContainer, fillCount);

    // Now insert at the end
    size_t emptyCount = MAX_TEST_CONTAINER_SIZE - fillCount;
    this->InsertFillAtIndexedPosition(testContainer, (MAX_TEST_CONTAINER_SIZE - emptyCount), emptyCount);

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.back(), this->mTruthCheck.back());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Range_Back)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t ilSize        = TestFixture::InitializerListSize();
    size_t containerSize = ilSize * 2;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list as a range at the end
    size_t emptyCount = containerSize - ilSize;
    this->InsertRangeAtIndexedPosition(testContainer, (containerSize - emptyCount));

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.back(), this->mTruthCheck.back());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Initializer_List_Back)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t ilSize        = TestFixture::InitializerListSize();
    size_t containerSize = ilSize * 2;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list at the end
    size_t emptyCount = containerSize - ilSize;
    this->InsertInitListAtIndexedPosition(testContainer, (containerSize - emptyCount));

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.back(), this->mTruthCheck.back());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Front_L_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    this->InsertValueAtIndexedPosition(testContainer, 0);

    ASSERT_EQ(testContainer.front(), this->mTruthCheck.front());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Front_R_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->InsertValueAtIndexedPosition(testContainer, 0, true);

    ASSERT_EQ(testContainer.front(), this->mTruthCheck.front());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Fill_Front)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fillCount = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(testContainer, fillCount);

    // Now insert at the end
    size_t emptyCount = MAX_TEST_CONTAINER_SIZE - fillCount;
    this->InsertFillAtIndexedPosition(testContainer, 0, emptyCount);

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.front(), this->mTruthCheck.front());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Range_Front)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t ilSize        = TestFixture::InitializerListSize();
    size_t containerSize = ilSize * 2;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list as a range at the end
    size_t emptyCount = containerSize - ilSize;
    this->InsertRangeAtIndexedPosition(testContainer, 0);

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.front(), this->mTruthCheck.front());
}

TYPED_TEST_P(SequenceContainerTest, Insert_Initializer_List_Front)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t ilSize        = TestFixture::InitializerListSize();
    size_t containerSize = ilSize * 2;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list at the end
    size_t emptyCount = containerSize - ilSize;
    this->InsertInitListAtIndexedPosition(testContainer, 0);

    // TODO check whole insert zone
    ASSERT_EQ(testContainer.front(), this->mTruthCheck.front());
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_L_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert in the middle
    size_t insertPos = MAX_TEST_CONTAINER_SIZE / 2;
    this->InsertValueAtIndexedPosition(testContainer, insertPos);

    EXPECT_EQ(testContainer.front(), this->mTruthCheck.front());
    EXPECT_EQ(testContainer.back(), this->mTruthCheck.back());
    EXPECT_EQ(testContainer[insertPos], this->mTruthCheck[insertPos]);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_R_Value)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(testContainer, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert in the middle
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    size_t insertPos = MAX_TEST_CONTAINER_SIZE / 2;
    this->InsertValueAtIndexedPosition(testContainer, insertPos, true);

    EXPECT_EQ(testContainer.front(), this->mTruthCheck.front());
    EXPECT_EQ(testContainer.back(), this->mTruthCheck.back());
    EXPECT_EQ(testContainer[insertPos], this->mTruthCheck[insertPos]);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_Fill)
{
    typename TestFixture::ContainerType testContainer(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fillCount = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(testContainer, fillCount);

    // Now insert in the middle of the valid element space
    size_t insertPos  = fillCount / 2;
    size_t emptyCount = MAX_TEST_CONTAINER_SIZE - fillCount;
    this->InsertFillAtIndexedPosition(testContainer, insertPos, emptyCount);

    EXPECT_EQ(testContainer.front(), this->mTruthCheck.front());
    EXPECT_EQ(testContainer.back(), this->mTruthCheck.back());

    // TODO check whole insert zone
    EXPECT_EQ(testContainer[insertPos], this->mTruthCheck[insertPos]);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_Range)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t ilSize               = TestFixture::InitializerListSize();
    size_t containerSize        = ilSize * 2;
    size_t initialContainerSize = containerSize - ilSize;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list as a range at the end
    size_t insertPos = initialContainerSize / 2;
    this->InsertRangeAtIndexedPosition(testContainer, insertPos);

    EXPECT_EQ(testContainer.front(), this->mTruthCheck.front());
    EXPECT_EQ(testContainer.back(), this->mTruthCheck.back());

    // TODO check whole insert zone
    EXPECT_EQ(testContainer[insertPos], this->mTruthCheck[insertPos]);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_InitializerList)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t ilSize               = TestFixture::InitializerListSize();
    size_t containerSize        = ilSize * 2;
    size_t initialContainerSize = containerSize - ilSize;

    typename TestFixture::ContainerType testContainer(containerSize);

    // Fill container halfway
    this->PushBackSequentially(testContainer, ilSize);

    // Now insert the initializer list at the end
    size_t insertPos = initialContainerSize / 2;
    this->InsertInitListAtIndexedPosition(testContainer, insertPos);

    EXPECT_EQ(testContainer.front(), this->mTruthCheck.front());
    EXPECT_EQ(testContainer.back(), this->mTruthCheck.back());

    // TODO check whole insert zone
    EXPECT_EQ(testContainer[insertPos], this->mTruthCheck[insertPos]);
}

//  End of tests =======================================================================================================

// Register tests to test suite
REGISTER_TYPED_TEST_SUITE_P(SequenceContainerTest, Default_Constructor, Iterator_Constructors, Copy_Constructor,
                            Copy_Constructor_With_Default_Constructed_Rhs, Copy_Constructor_With_Empty_Rhs,
                            Move_Constructor, Move_Constructor_With_Default_Constructed_Rhs,
                            Move_Constructor_With_Empty_Rhs, Copy_Assignment,
                            Copy_Assignment_With_Default_Constructed_Rhs, Copy_Assignment_With_Empty_Rhs,
                            Move_Assignment, Move_Assignment_With_Default_Constructed_Rhs,
                            Move_Assignment_With_Empty_Rhs,
                            If_Container_Is_Empty_Then_Iterator_Can_Not_Increment_Forward, Push_Back_L_Value,
                            If_Already_At_End_Then_Iterator_Can_Not_Increment_Forward, Push_Back_R_Value,
                            Push_Front_L_Value, Push_Front_R_Value, Insert_Back_L_Value, Insert_Back_R_Value,
                            Insert_Fill_Back, Insert_Range_Back, Insert_Initializer_List_Back, Insert_Front_L_Value,
                            Insert_Front_R_Value, Insert_Fill_Front, Insert_Range_Front, Insert_Initializer_List_Front,
                            Random_Insert_L_Value, Random_Insert_R_Value, Random_Insert_Fill, Random_Insert_Range,
                            Random_Insert_InitializerList);
