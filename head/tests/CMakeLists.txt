# this CMakeLists.txt file is only used when host-compiling to build tests
find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)
include(AddBuildAndTestTarget)

add_executable(
        head
        test_main.cpp
)

target_include_directories(head PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)
set_target_properties(head
        PROPERTIES
        CXX_STANDARD 20
        CXX_STANDARD_REQUIRED TRUE)

target_compile_options(head
        PUBLIC
        -Wall
        -Werror
        -Weffc++
        -Wreorder
        -Wsign-promo
        -Wextra-semi
        -Wctor-dtor-privacy
        -fno-rtti)
target_link_libraries(head Catch2::Catch2)

catch_discover_tests(head)
add_build_and_test_target(head)
