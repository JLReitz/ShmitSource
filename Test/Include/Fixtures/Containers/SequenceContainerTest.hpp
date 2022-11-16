#pragma once

#include <deque>
#include <utility>

#include <gtest/gtest.h>

#ifndef MAX_TEST_CONTAINER_SIZE
#define MAX_TEST_CONTAINER_SIZE 5
#endif

/**!
 * @brief Verifies a container meets the "Sequence Container" named requirements specified here:
 * https://en.cppreference.com/w/cpp/named_req/SequenceContainer
 * @tparam SequenceContainerTestModule Test module implementation. Named requirements below.
 * @note SequenceContainerTestModule named requirements:
 *  - Types:
 *      - ContainerType: The container specialization this module is testing
 *  - Methods:
 *      - static ContainerType::ValueType GetInitializationValue(): Returns a value to initialize elements within the
 * container
 *      - static ContainerType::ValueType GetAssignValue(): Returns a value to assign to the container
 *      - static ContainerType::ValueType GetEmplaceValue(): Returns a value to emplace within the container
 *      - static ContainerType::ValueType GetInsertValue(): Returns a value to insert in to the container
 *      - static ContainerType::ValueType GetPushValue(): Returns a value to push to the container
 *      - static std::initializer_list<ContainerType::ValueType> GetInitializerList(): Returns an initializer list
 *      - static size_t GetInitializerListSize(): Returns the size of the initializer list
 */
template<class SequenceContainerTestModule>
class SequenceContainerTest : public SequenceContainerTestModule, public ::testing::Test
{
protected:
    using ContainerType         = typename SequenceContainerTestModule::ContainerType;
    using ContainerIteratorType = typename ContainerType::iterator;

    using ValueType = typename ContainerType::value_type;

    using TruthCheckType         = std::deque<ValueType>; // Known-working STL sequence container for verification
    using TruchCheckIteratorType = typename TruthCheckType::iterator;

    /**!
     * @brief Fills a test container and the truth check with n values
     *
     * @param container Test container Test container
     * @param n Number of elements to assign
     */
    void AssignFill(ContainerType& container, size_t n)
    {
        ValueType assign_value = SequenceContainerTestModule::GetAssignValue();

        m_truth_check.assign(n, assign_value);
        container.assign(n, assign_value);
    }

    /**!
     * @brief Fills a test container and the truth check with the values stored within a range
     *
     * @param container Test container Test container
     */
    void AssignRange(ContainerType& container)
    {
        using IlType = std::initializer_list<ValueType>;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        m_truth_check.assign(il.begin(), il.end());
        container.assign(il.begin(), il.end());
    }

    /**!
     * @brief Assigns the contents of an initializer list to the test container and the truth check
     *
     * @param container Test container
     */
    void AssignInitList(ContainerType& container)
    {
        using IlType = std::initializer_list<ValueType>;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        m_truth_check.assign(il);
        container.assign(il);
    }

    /**!
     * @brief Emplaces an element in the back of a test container and the truth check n times, sequentially
     *
     * @param container Test container
     * @param n Number of emplaces to perform
     * @param is_r_value True if emplacement is performed with R-value
     */
    void EmplaceBackSequentially(ContainerType& container, size_t n, bool is_r_value = false)
    {
        for (size_t i = 0; i < n; i++)
        {
            ValueType emplace_value = SequenceContainerTestModule::GetEmplaceValue();

            if (is_r_value)
            {
                // Create copy of value to push to both containers
                ValueType emplace_value_2 = emplace_value;

                m_truth_check.emplace_back(std::move(emplace_value));
                container.emplace_back(std::move(emplace_value_2));
            }
            else
            {
                m_truth_check.emplace_back(emplace_value);
                container.emplace_back(emplace_value);
            }
        }
    }

    /**!
     * @brief Emplaces an element in the front of a test container and the truth check n times, sequentially
     *
     * @param container Test container
     * @param n Number of emplaces to perform
     * @param is_r_value True if emplacement is performed with R-value
     */
    void EmplaceFrontSequentially(ContainerType& container, size_t n, bool is_r_value = false)
    {
        for (size_t i = 0; i < n; i++)
        {
            ValueType emplace_value = SequenceContainerTestModule::GetEmplaceValue();

            if (is_r_value)
            {
                // Create copy of value to push to both containers
                ValueType emplace_value_2 = emplace_value;

                m_truth_check.emplace_front(std::move(emplace_value));
                container.emplace_front(std::move(emplace_value_2));
            }
            else
            {
                m_truth_check.emplace_front(emplace_value);
                container.emplace_front(emplace_value);
            }
        }
    }

    /**!
     * @brief Emplaces an element somehwere in a test container and the truth check
     *
     * @param container Test container
     * @param position Emplace position, relative to the front of the container
     * @param is_r_value True if emplacement is performed with R-value
     */
    void EmplaceValueAtIndexedPosition(ContainerType& container, size_t position, bool is_r_value = false)
    {
        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        ValueType emplace_value = SequenceContainerTestModule::GetEmplaceValue();

        if (is_r_value)
        {
            // Create copy of value to push to both containers
            ValueType emplace_value_2 = emplace_value;

            m_truth_check.emplace(truth_check_pos, std::move(emplace_value));
            container.emplace(container_pos, std::move(emplace_value_2));
        }
        else
        {
            m_truth_check.emplace(truth_check_pos, emplace_value);
            container.emplace(container_pos, emplace_value);
        }
    }

    void EraseValueAtIndexedPosition(ContainerType& container, size_t position)
    {
        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        container.erase(container_pos);
        m_truth_check.erase(truth_check_pos);
    }

    void EraseRangeAtIndexedPosition(ContainerType& container, size_t position, size_t range_size)
    {
        ContainerIteratorType  container_pos         = container.begin() + (signed)position;
        ContainerIteratorType  container_erase_end   = container_pos + range_size;
        TruchCheckIteratorType truth_check_pos       = m_truth_check.begin() + (signed)position;
        TruchCheckIteratorType truth_check_erase_end = truth_check_pos + (unsigned)range_size;

        container.erase(container_pos, container_erase_end);
        m_truth_check.erase(truth_check_pos, truth_check_erase_end);
    }

    /**!
     * @brief Inserts an element somehwere in a test container and the truth check
     *
     * @param container Test container
     * @param position Insert position, relative to the front of the container
     * @param is_r_value True if emplacement is performed with R-value
     */
    void InsertValueAtIndexedPosition(ContainerType& container, size_t position, bool is_r_value = false)
    {
        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        ValueType insert_value = SequenceContainerTestModule::GetInsertValue();

        if (is_r_value)
        {
            // Create copy of value to push to both containers
            ValueType insert_value_2 = insert_value;

            m_truth_check.insert(truth_check_pos, std::move(insert_value));
            container.insert(container_pos, std::move(insert_value_2));
        }
        else
        {
            m_truth_check.insert(truth_check_pos, insert_value);
            container.insert(container_pos, insert_value);
        }
    }

    /**!
     * @brief Inserts n elements somehwere in a test container and the truth check
     *
     * @param container Test container
     * @param position Insert position, relative to the front of the container
     * @param n Number of values to insert
     */
    void InsertFillAtIndexedPosition(ContainerType& container, size_t position, size_t n)
    {
        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        ValueType insert_value = SequenceContainerTestModule::GetInsertValue();

        m_truth_check.insert(truth_check_pos, n, insert_value);
        container.insert(container_pos, n, insert_value);
    }

    /**!
     * @brief Inserts a range somehwere in a test container and the truth check
     *
     * @param container Test container
     * @param position Insert position, relative to the front of the container
     */
    void InsertRangeAtIndexedPosition(ContainerType& container, size_t position)
    {
        using IlType = std::initializer_list<ValueType>;

        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        m_truth_check.insert(truth_check_pos, il.begin(), il.end());
        container.insert(container_pos, il.begin(), il.end());
    }

    /**!
     * @brief Inserts the contents of an initializer list somehwere in a test container and the truth check
     *
     * @param container Test container
     * @param position Insert position, relative to the front of the container
     */
    void InsertInitListAtIndexedPosition(ContainerType& container, size_t position)
    {
        using IlType = std::initializer_list<ValueType>;

        ContainerIteratorType  container_pos   = container.begin() + (signed)position;
        TruchCheckIteratorType truth_check_pos = m_truth_check.begin() + (signed)position;

        IlType il = SequenceContainerTestModule::GetInitializerList();

        m_truth_check.insert(truth_check_pos, il);
        container.insert(container_pos, il);
    }

    /**!
     * @brief Pushes an element to the back of a test container and the truth check n times, sequentially
     *
     * @param container Test container
     * @param n Number of pushes to perform
     * @param is_r_value True if push is performed with R-value
     */
    void PushBackSequentially(ContainerType& container, size_t n, bool is_r_value = false)
    {
        for (size_t i = 0; i < n; i++)
        {
            ValueType push_back_value = SequenceContainerTestModule::GetPushValue();

            if (is_r_value)
            {
                // Create copy of value to push to both containers
                ValueType push_back_value_2 = push_back_value;

                m_truth_check.push_back(std::move(push_back_value));
                container.push_back(std::move(push_back_value_2));
            }
            else
            {
                m_truth_check.push_back(push_back_value);
                container.push_back(push_back_value);
            }
        }
    }

    /**!
     * @brief Pushes an element to the front of a test container and the truth check n times, sequentially
     *
     * @param container Test container
     * @param n Number of pushes to perform
     * @param is_r_value True if push is performed with R-value
     */
    void PushFrontSequentially(ContainerType& container, size_t n, bool is_r_value = false)
    {
        for (size_t i = 0; i < n; i++)
        {
            ValueType push_front_value = SequenceContainerTestModule::GetPushValue();

            if (is_r_value)
            {
                // Create copy of value to push to both containers
                ValueType push_front_value_2 = push_front_value;

                m_truth_check.push_front(std::move(push_front_value));
                container.push_front(std::move(push_front_value_2));
            }
            else
            {
                m_truth_check.push_front(push_front_value);
                container.push_front(push_front_value);
            }
        }
    }

    /**!
     * @brief Compares a test container to the truth check
     *
     * @param container Test container
     */
    void ValidateAgainstTruthCheck(ContainerType& container)
    {
        // Test container should match the truth check sequence
        EXPECT_EQ(container.size(), m_truth_check.size());

        size_t count = 0;
        for (ValueType const& value : container)
        {
            ASSERT_LT(count, m_truth_check.size());
            EXPECT_EQ(value, m_truth_check[count++]);
        }
    }

    TruthCheckType m_truth_check;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Declare type-parameterized-test suite           ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPED_TEST_SUITE_P(SequenceContainerTest);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Constructor & assignment tests          ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPED_TEST_P(SequenceContainerTest, Default_Constructor)
{
    typename TestFixture::ContainerType test_container;

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Iterator_Constructors)
{
    using IlType = std::initializer_list<typename TestFixture::ValueType>;

    // Test initializer list constructor
    IlType                              il = TestFixture::GetInitializerList();
    typename TestFixture::ContainerType test_container(il);

    // Use assertions for the first set of tests, if they fail there's no point testing the next
    ASSERT_EQ(test_container.size(), il.size());

    typename IlType::iterator il_curr = il.begin();
    for (const typename TestFixture::ValueType& value : test_container)
    {
        ASSERT_NE(il_curr, il.end());
        ASSERT_EQ(value, *il_curr++);
    }

    // Test range constructor
    typename TestFixture::ContainerType test_container_2(il.begin(), il.end());

    EXPECT_EQ(test_container_2.size(), il.size());

    il_curr = il.begin();
    for (const typename TestFixture::ValueType& value : test_container_2)
    {
        ASSERT_NE(il_curr, il.end());
        EXPECT_EQ(value, *il_curr++);
    }
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    typename TestFixture::ContainerType test_container(rhs);

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType test_container(rhs);

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Constructor_With_Empty_Rhs)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType test_container(rhs);

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TestFixture::GetInitializationValue());
    typename TestFixture::ContainerType test_container(std::move(rhs));

    EXPECT_EQ(test_container.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Default_Constructed_Rhs)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType test_container(std::move(rhs));

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Constructor_With_Empty_Rhs)
{
    // TODO initializer list constructor
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType test_container(std::move(rhs));

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());
    typename TestFixture::ContainerType test_container;

    test_container = rhs;

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType rhs;
    typename TestFixture::ContainerType test_container;

    test_container = rhs;

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Copy_Assignment_With_Empty_Rhs)
{
    typename TestFixture::ContainerType rhs(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerType test_container;

    test_container = rhs;

    EXPECT_TRUE(test_container == rhs);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment)
{
    typename TestFixture::ContainerType test_container;

    test_container =
        typename TestFixture::ContainerType(MAX_TEST_CONTAINER_SIZE, TestFixture::GetInitializationValue());

    EXPECT_EQ(test_container.size(), MAX_TEST_CONTAINER_SIZE);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Default_Constructed_Rhs)
{
    typename TestFixture::ContainerType test_container;

    test_container = typename TestFixture::ContainerType();

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Move_Assignment_With_Empty_Rhs)
{
    typename TestFixture::ContainerType test_container;

    test_container = typename TestFixture::ContainerType(MAX_TEST_CONTAINER_SIZE);

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Initializer_List_Assignment)
{
    using IlType = std::initializer_list<typename TestFixture::ValueType>;

    // Test initializer list constructor
    IlType                              il = TestFixture::GetInitializerList();
    typename TestFixture::ContainerType test_container;

    test_container = il;

    EXPECT_EQ(test_container.size(), il.size());

    typename IlType::iterator il_curr = il.begin();
    for (const typename TestFixture::ValueType& value : test_container)
    {
        EXPECT_NE(il_curr, il.end());
        EXPECT_EQ(value, *il_curr++);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Iterator tests          ////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPED_TEST_P(SequenceContainerTest, If_Container_Is_Empty_Then_Iterator_Can_Not_Increment_Forward)
{
    typename TestFixture::ContainerType test_container;

    size_t count = 0;
    for (typename TestFixture::ValueType& value : test_container)
    {
        (void)value; // Unused
        count++;
    }

    ASSERT_EQ(count, 0);
}

TYPED_TEST_P(SequenceContainerTest, If_Already_At_End_Then_Iterator_Can_Not_Increment_Forward)
{
    typename TestFixture::ContainerType         test_container(MAX_TEST_CONTAINER_SIZE);
    typename TestFixture::ContainerIteratorType containerEnd = test_container.end();

    containerEnd++;
    ASSERT_EQ(containerEnd, test_container.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Functional tests            ////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TYPED_TEST_P(SequenceContainerTest, At_Accessor_And_Square_Bracket_Operator_Return_Same_Elements)
{
    typename TestFixture::ContainerType test_container(TestFixture::GetInitializerList());

    for (size_t i = 0; i < test_container.size(); i++)
        EXPECT_EQ(test_container.at(i), test_container[i]);
}

TYPED_TEST_P(SequenceContainerTest, Clear_Sets_Empty_State)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());

    test_container.clear();

    EXPECT_TRUE(test_container.empty());
    EXPECT_EQ(test_container.size(), 0);
}

TYPED_TEST_P(SequenceContainerTest, Assign_Fill)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->AssignFill(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, If_Full_Then_Assign_Fill_Replaces_Elements)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());

    this->AssignFill(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Assign_Range)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->AssignRange(test_container);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, If_Full_Then_Assign_Range_Replaces_Elements)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());

    this->AssignRange(test_container);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Assign_Initializer_List)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->AssignInitList(test_container);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, If_Full_Then_Assign_Initializer_List_Replaces_Elements)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE, TypeParam::GetInitializationValue());

    this->AssignInitList(test_container);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Emplace_Back_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->EmplaceBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Emplace_Back_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->EmplaceBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Emplace_Front_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->EmplaceFrontSequentially(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Emplace_Front_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->EmplaceFrontSequentially(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Emplace_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now emplace in the middle
    size_t emplacePos = MAX_TEST_CONTAINER_SIZE / 2;
    this->EmplaceValueAtIndexedPosition(test_container, emplacePos);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Emplace_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now emplace in the middle
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    size_t emplacePos = MAX_TEST_CONTAINER_SIZE / 2;
    this->EmplaceValueAtIndexedPosition(test_container, emplacePos, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Back_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    this->InsertValueAtIndexedPosition(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Back_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->InsertValueAtIndexedPosition(test_container, MAX_TEST_CONTAINER_SIZE - 1, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Fill_Back)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fill_count = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(test_container, fill_count);

    // Now insert at the end
    size_t empty_count = MAX_TEST_CONTAINER_SIZE - fill_count;
    this->InsertFillAtIndexedPosition(test_container, (MAX_TEST_CONTAINER_SIZE - empty_count), empty_count);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Range_Back)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t il_size        = TestFixture::InitializerListSize();
    size_t container_size = il_size * 2;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list as a range at the end
    size_t empty_count = container_size - il_size;
    this->InsertRangeAtIndexedPosition(test_container, (container_size - empty_count));

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Initializer_List_Back)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t il_size        = TestFixture::InitializerListSize();
    size_t container_size = il_size * 2;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list at the end
    size_t empty_count = container_size - il_size;
    this->InsertInitListAtIndexedPosition(test_container, (container_size - empty_count));

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Front_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    this->InsertValueAtIndexedPosition(test_container, 0);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Front_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert at the end
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->InsertValueAtIndexedPosition(test_container, 0, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Fill_Front)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fill_count = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(test_container, fill_count);

    // Now insert at the front
    size_t empty_count = MAX_TEST_CONTAINER_SIZE - fill_count;
    this->InsertFillAtIndexedPosition(test_container, 0, empty_count);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Range_Front)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t il_size        = TestFixture::InitializerListSize();
    size_t container_size = il_size * 2;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list as a range at the end
    this->InsertRangeAtIndexedPosition(test_container, 0);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Insert_Initializer_List_Front)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t il_size        = TestFixture::InitializerListSize();
    size_t container_size = il_size * 2;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list at the end
    this->InsertInitListAtIndexedPosition(test_container, 0);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert in the middle
    size_t insert_pos = MAX_TEST_CONTAINER_SIZE / 2;
    this->InsertValueAtIndexedPosition(test_container, insert_pos);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container to MAX_TEST_CONTAINER_SIZE - 1
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    // Now insert in the middle
    // 'true' flag in last argument (default = 'false') signals an R-value operation
    size_t insert_pos = MAX_TEST_CONTAINER_SIZE / 2;
    this->InsertValueAtIndexedPosition(test_container, insert_pos, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_Fill)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // Fill container halfway
    size_t fill_count = MAX_TEST_CONTAINER_SIZE / 2;
    this->PushBackSequentially(test_container, fill_count);

    // Now insert in the middle of the valid element space
    size_t insert_pos  = fill_count / 2;
    size_t empty_count = MAX_TEST_CONTAINER_SIZE - fill_count;
    this->InsertFillAtIndexedPosition(test_container, insert_pos, empty_count);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_Range)
{
    // Using test module's initializer list to supply the range
    // Fetch the size and construct a container twice as large
    size_t il_size                = TestFixture::InitializerListSize();
    size_t container_size         = il_size * 2;
    size_t initial_container_size = container_size - il_size;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list as a range at the end
    size_t insert_pos = initial_container_size / 2;
    this->InsertRangeAtIndexedPosition(test_container, insert_pos);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Insert_InitializerList)
{
    // Fetch the test module's initializer list size and construct a container twice as large
    size_t il_size                = TestFixture::InitializerListSize();
    size_t container_size         = il_size * 2;
    size_t initial_container_size = container_size - il_size;

    typename TestFixture::ContainerType test_container(container_size);

    // Fill container halfway
    this->PushBackSequentially(test_container, il_size);

    // Now insert the initializer list at the end
    size_t insert_pos = initial_container_size / 2;
    this->InsertInitListAtIndexedPosition(test_container, insert_pos);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Push_Back_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Push_Back_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_L_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushFrontSequentially(test_container, MAX_TEST_CONTAINER_SIZE);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Push_Front_R_Value)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    // 'true' flag in last argument (default = 'false') signals an R-value operation
    this->PushFrontSequentially(test_container, MAX_TEST_CONTAINER_SIZE, true);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Erase_Single_Front)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseValueAtIndexedPosition(test_container, 0);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Erase_Single_Back)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseValueAtIndexedPosition(test_container, MAX_TEST_CONTAINER_SIZE - 1);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Erase_Single)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseValueAtIndexedPosition(test_container, MAX_TEST_CONTAINER_SIZE / 2);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Erase_Range_Front)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseRangeAtIndexedPosition(test_container, 0, MAX_TEST_CONTAINER_SIZE / 2);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Erase_Range_Back)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    size_t range_size  = MAX_TEST_CONTAINER_SIZE / 2;
    size_t erase_start = MAX_TEST_CONTAINER_SIZE - range_size;
    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseRangeAtIndexedPosition(test_container, erase_start, range_size);

    this->ValidateAgainstTruthCheck(test_container);
}

TYPED_TEST_P(SequenceContainerTest, Random_Erase_Range)
{
    typename TestFixture::ContainerType test_container(MAX_TEST_CONTAINER_SIZE);

    this->PushBackSequentially(test_container, MAX_TEST_CONTAINER_SIZE);
    this->EraseRangeAtIndexedPosition(test_container, MAX_TEST_CONTAINER_SIZE / 2, MAX_TEST_CONTAINER_SIZE / 2);

    this->ValidateAgainstTruthCheck(test_container);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Register tests to test suite            ////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

REGISTER_TYPED_TEST_SUITE_P(
    SequenceContainerTest, Default_Constructor, Iterator_Constructors, Copy_Constructor,
    Copy_Constructor_With_Default_Constructed_Rhs, Copy_Constructor_With_Empty_Rhs, Move_Constructor,
    Move_Constructor_With_Default_Constructed_Rhs, Move_Constructor_With_Empty_Rhs, Copy_Assignment,
    Copy_Assignment_With_Default_Constructed_Rhs, Copy_Assignment_With_Empty_Rhs, Move_Assignment,
    Move_Assignment_With_Default_Constructed_Rhs, Move_Assignment_With_Empty_Rhs, Initializer_List_Assignment,
    If_Container_Is_Empty_Then_Iterator_Can_Not_Increment_Forward,
    If_Already_At_End_Then_Iterator_Can_Not_Increment_Forward,
    At_Accessor_And_Square_Bracket_Operator_Return_Same_Elements, Clear_Sets_Empty_State, Assign_Fill,
    If_Full_Then_Assign_Fill_Replaces_Elements, Assign_Range, If_Full_Then_Assign_Range_Replaces_Elements,
    Assign_Initializer_List, If_Full_Then_Assign_Initializer_List_Replaces_Elements, Emplace_Back_L_Value,
    Emplace_Back_R_Value, Emplace_Front_L_Value, Emplace_Front_R_Value, Random_Emplace_L_Value, Random_Emplace_R_Value,
    Insert_Back_L_Value, Insert_Back_R_Value, Insert_Fill_Back, Insert_Range_Back, Insert_Initializer_List_Back,
    Insert_Front_L_Value, Insert_Front_R_Value, Insert_Fill_Front, Insert_Range_Front, Insert_Initializer_List_Front,
    Random_Insert_L_Value, Random_Insert_R_Value, Random_Insert_Fill, Random_Insert_Range,
    Random_Insert_InitializerList, Push_Back_L_Value, Push_Back_R_Value, Push_Front_L_Value, Push_Front_R_Value,
    Erase_Single_Front, Erase_Single_Back, Random_Erase_Single, Erase_Range_Front, Erase_Range_Back,
    Random_Erase_Range);
