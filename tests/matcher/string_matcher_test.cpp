#include <gtest/gtest.h>

#include "matcher/string_matcher.h"

TEST(StringMatcherTest, MatchesExistingText) {
    StringMatcher matcher{"admin"};

    bool result = matcher.matches("/admin/login");

    EXPECT_TRUE(result);
}

TEST(StringMatcherTest, DoesNotMatchMissingText) {
    StringMatcher matcher{"admin"};

    bool result = matcher.matches("/user/login");

    EXPECT_FALSE(result);
}
