#include <gtest/gtest.h>

#include <stdexcept>

#include "matcher/pcre2_matcher.h"

TEST(Pcre2MatcherTest, MatchesRegex) {
    Pcre2Matcher matcher{R"(drop\s+table)"};

    bool result = matcher.matches("query=drop       table users");

    EXPECT_TRUE(result);
}

TEST(Pcre2MatcherTest, DoesNotMatchDifferentText) {
    Pcre2Matcher matcher{R"(drop\s+table)"};

    bool result = matcher.matches("query=select users");

    EXPECT_FALSE(result);
}

TEST(Pcre2MatcherTest, RejectsInvalidRegex) {
    EXPECT_THROW(
        Pcre2Matcher{R"((drop\s+table)"},
        std::runtime_error
    );
}
