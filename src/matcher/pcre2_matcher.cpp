//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>
#include "matcher/pcre2_matcher.h"

#include <memory>

Pcre2Matcher::Pcre2Matcher(std::string pattern) : pattern_(std::move(pattern)) {
}

bool Pcre2Matcher::matches(
    std::string_view input
) const {
    int errorCode = 0;
    PCRE2_SIZE errorOffset = 0;

    using Pcre2CodePtr = std::unique_ptr<pcre2_code, decltype(&pcre2_code_free)>;

    Pcre2CodePtr code{
        pcre2_compile(
            reinterpret_cast<PCRE2_SPTR>(pattern_.data()),
            pattern_.size(),
            0,
            &errorCode,
            &errorOffset,
            nullptr
        ),
        pcre2_code_free
    };


    if (!code) {
        return false;
    }

    using Pcre2MatchDataPtr = std::unique_ptr<pcre2_match_data, decltype(&pcre2_match_data_free)>;


    Pcre2MatchDataPtr matchData{
        pcre2_match_data_create_from_pattern(
            code.get(),
            nullptr
        ), pcre2_match_data_free
    };

    if (!matchData) {
        return false;
    }

    int result = pcre2_match(
        code.get(),
        reinterpret_cast<PCRE2_SPTR>(input.data()),
        input.size(),
        0,
        0,
        matchData.get(),
        nullptr
    );

    return result >= 0;
}
