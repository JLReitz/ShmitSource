# Target unit tests
add_executable(RatioTest
    RatioTest.cpp
)

# Add ShmitCore include path to targets
# target_include_directories(RatioTest PUBLIC
# ${SHMITCORE_INCLUDES}
# ${SHMITCORE_TEST_INCLUDES}
# )

# Link gtest_main to targets
target_link_libraries(RatioTest
    gtest_main
    ShmitCore
    ShmitCore-Test
)

# Discover all unit tests
gtest_discover_tests(RatioTest)