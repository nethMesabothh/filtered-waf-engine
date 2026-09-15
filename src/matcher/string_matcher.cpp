//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#include "matcher/string_matcher.h"
#include <utility>

StringMatcher::StringMatcher(std::string pattern) : pattern_(std::move(pattern)) {
}

bool StringMatcher::matches(std::string_view input) const {
    return input.find(pattern_) != std::string_view::npos;
}
