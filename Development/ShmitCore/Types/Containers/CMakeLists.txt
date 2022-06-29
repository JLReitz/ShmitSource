# Target unit tests
add_executable(BufferTest
    Types/Containers/unit-test/Buffer.cc
)

# Add ShmitCore include path to targets
target_include_directories(BufferTest PUBLIC ${SHMITCORE_INCLUDE_PATH})

# Link gtest_main to targets
target_link_libraries(BufferTest
    gtest_main
)

# Discover all unit tests
include(GoogleTest)
gtest_discover_tests(BufferTest)