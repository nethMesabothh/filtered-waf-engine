cmake_minimum_required(VERSION 4.3)
project(filtered_waf_engine)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

set(PCRE2_BUILD_PCRE2GREP OFF CACHE BOOL "" FORCE)
set(PCRE2_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(BUILD_STATIC_LIBS ON CACHE BOOL "" FORCE)

FetchContent_Declare(
        pcre2
        GIT_REPOSITORY https://github.com/PCRE2Project/pcre2.git
        GIT_TAG pcre2-10.47
)

FetchContent_MakeAvailable(pcre2)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(filtered_waf_core STATIC
        src/engine/engine.cpp
        src/matcher/string_matcher.cpp
        src/matcher/pcre2_matcher.cpp
        src/rule/rule_preparer.cpp

        include/core/waf_request.hpp
        include/core/decision.h
        include/rule/rule.h
        include/rule/rule_set.h
        include/rule/prepared_rule.h
        include/rule/prepared_rule_set.h
        include/rule/rule_preparer.h
        include/matcher/matcher.h
        include/matcher/string_matcher.h
        include/matcher/pcre2_matcher.h
        include/engine/engine.h
)

target_include_directories(
        filtered_waf_core
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# PUBLIC because include/matcher/pcre2_matcher.h includes <pcre2.h> and exposes
# pcre2_code, so consumers of this library need PCRE2 headers too.
target_link_libraries(
        filtered_waf_core
        PUBLIC
        pcre2-8-static
)

add_executable(filtered_waf_engine
        src/main.cpp
)

target_link_libraries(
        filtered_waf_engine
        PRIVATE
        filtered_waf_core
)

add_executable(filtered_waf_tests
        tests/matcher/string_matcher_test.cpp
        tests/matcher/pcre2_matcher_test.cpp
)

target_link_libraries(
        filtered_waf_tests
        PRIVATE
        filtered_waf_core
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(filtered_waf_tests)