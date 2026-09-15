//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#define PCRE2_CODE_UNIT_WIDTH 8
#include <pcre2.h>

#include "matcher/pcre2_matcher.h"

//constructor
Pcre2Matcher::Pcre2Matcher(std::string pattern)
    : compiledCode_{nullptr, pcre2_code_free} {
    int errorCode = 0;
    PCRE2_SIZE errorOffset = 0;

    pcre2_code *rawCode = pcre2_compile(
        reinterpret_cast<PCRE2_SPTR>(pattern.data()),
        pattern.size(),
        0,
        &errorCode,
        &errorOffset,
        nullptr
    );

    if (rawCode == nullptr) {
        PCRE2_UCHAR errorMessage[256];
        pcre2_get_error_message(
            errorCode,
            errorMessage,
            sizeof(errorMessage)
        );

        throw std::runtime_error(
            "Invalid PCRE2 pattern at offset "
            + std::to_string(errorOffset)
            + ": "
            + reinterpret_cast<const char *>(
                errorMessage
            )
        );
    };


    compiledCode_.reset(rawCode);
}

//function
bool Pcre2Matcher::matches(std::string_view input) const {
    if (!compiledCode_) {
        return false;
    }

    using Pcre2MatchDataPtr =
            std::unique_ptr<pcre2_match_data, decltype(&pcre2_match_data_free)>;

    Pcre2MatchDataPtr matchData{
        pcre2_match_data_create_from_pattern(compiledCode_.get(), nullptr),
        pcre2_match_data_free
    };

    if (!matchData) {
        return false;
    }

    int result = pcre2_match(
        compiledCode_.get(),
        reinterpret_cast<PCRE2_SPTR>(input.data()),
        input.size(),
        0,
        0,
        matchData.get(),
        nullptr
    );

    return result >= 0;
}
