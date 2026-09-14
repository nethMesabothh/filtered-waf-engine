cmake_minimum_required(VERSION 4.3)
project(filtered_waf_engine)

set(CMAKE_CXX_STANDARD 20)

add_executable(filtered_waf_engine main.cpp
        include/waf_request.hpp
        include/rule.h
        include/engine.h
        src/engine.cpp
        include/decision.h
        include/rule_set.h
        include/string_matcher.h
        src/string_matcher.cpp
        include/matcher.h
)
