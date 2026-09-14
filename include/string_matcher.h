//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_STRING_MATCHER_H
#define FILTERED_WAF_ENGINE_STRING_MATCHER_H

#endif //FILTERED_WAF_ENGINE_STRING_MATCHER_H

#pragma once
#include <string_view>
#include "matcher.h"

class StringMatcher : public Matcher {
public:
    bool matches(std::string_view input, std::string_view pattern) const override;
};
