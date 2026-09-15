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

add_executable(filtered_waf_engine
        src/main.cpp
        include/core/waf_request.hpp
        include/core/decision.h
        include/rule/rule.h
        include/rule/rule_set.h
        include/rule/prepared_rule.h
        include/rule/prepared_rule_set.h
        include/rule/rule_preparer.h
        src/rule/rule_preparer.cpp
        include/matcher/matcher.h
        include/matcher/string_matcher.h
        src/matcher/string_matcher.cpp
        include/matcher/pcre2_matcher.h
        src/matcher/pcre2_matcher.cpp
        include/engine/engine.h
        src/engine/engine.cpp
)

target_include_directories(filtered_waf_engine PRIVATE include)

target_link_libraries(
        filtered_waf_engine
        PRIVATE
        pcre2-8-static
)