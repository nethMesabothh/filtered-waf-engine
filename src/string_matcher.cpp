//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#include "../include/string_matcher.h"

bool StringMatcher::matches(std::string_view input, std::string_view pattern) const {
    return input.find(pattern) != std::string_view::npos;
}
