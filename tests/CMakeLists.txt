find_package(GTest REQUIRED)

add_executable(unit_tests
    char_class_element_test.cpp
)

target_link_libraries(unit_tests PRIVATE
    regex_engine
    GTest::gtest
    GTest::gtest_main
)

if(MSVC)
    target_compile_options(unit_tests PRIVATE /W4)
else()
    target_compile_options(unit_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)
gtest_discover_tests(unit_tests)
