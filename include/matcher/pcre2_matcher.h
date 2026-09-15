//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_PCRE2_MATCHER_H
#define FILTERED_WAF_ENGINE_PCRE2_MATCHER_H

#endif //FILTERED_WAF_ENGINE_PCRE2_MATCHER_H

#pragma once

#include <string>
#include <string_view>
#include "matcher/matcher.h"

class Pcre2Matcher : public Matcher {
public:
    explicit Pcre2Matcher(std::string pattern);

    bool matches(std::string_view input) const override;

private:
    std::string pattern_;
};
