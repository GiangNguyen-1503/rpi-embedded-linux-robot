include(FetchContent)
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
FetchContent_MakeAvailable(googletest)

# Day 5: add test_decision.cpp testing the pure decision function.
# add_executable(test_decision test_decision.cpp)
# target_link_libraries(test_decision PRIVATE pvm_common GTest::gtest_main)
# include(GoogleTest)
# gtest_discover_tests(test_decision)
