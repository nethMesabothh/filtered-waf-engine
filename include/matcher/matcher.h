//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_MATCHER_H
#define FILTERED_WAF_ENGINE_MATCHER_H

#endif //FILTERED_WAF_ENGINE_MATCHER_H

#pragma once
#include <string_view>

class Matcher {
public:
    virtual bool matches(std::string_view input) const = 0;

    virtual ~Matcher() = default;
};