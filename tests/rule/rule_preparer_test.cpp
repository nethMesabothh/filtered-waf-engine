#include <gtest/gtest.h>

#include <stdexcept>

#include "rule/rule_preparer.h"

TEST(RulePreparerTest, PreparesStringRule) {
    RuleSet ruleSet{
        .rules = {
            {
                .id = 100,
                .target = RuleTarget::Path,
                .matcher = MatcherType::String,
                .pattern = "admin",
                .action = RuleAction::Block
            }
        }
    };

    RulePreparer preparer;

    PreparedRuleSet prepared = preparer.prepare(ruleSet);

    ASSERT_EQ(prepared.rules.size(), 1);

    const PreparedRule& rule = prepared.rules[0];

    EXPECT_EQ(rule.id, 100);
    EXPECT_EQ(rule.target, RuleTarget::Path);
    EXPECT_EQ(rule.action, RuleAction::Block);

    EXPECT_TRUE(rule.matcher->matches("/admin/login"));
    EXPECT_FALSE(rule.matcher->matches("/user/login"));
}

TEST(RulePreparerTest, PreparesPcre2Rule) {
    RuleSet ruleSet{
        .rules = {
            {
                .id = 102,
                .target = RuleTarget::Body,
                .matcher = MatcherType::Pcre2,
                .pattern = R"(drop\s+table)",
                .action = RuleAction::Block
            }
        }
    };

    RulePreparer preparer;

    PreparedRuleSet prepared = preparer.prepare(ruleSet);

    ASSERT_EQ(prepared.rules.size(), 1);

    const PreparedRule& rule = prepared.rules[0];

    EXPECT_EQ(rule.id, 102);
    EXPECT_EQ(rule.target, RuleTarget::Body);
    EXPECT_EQ(rule.action, RuleAction::Block);

    EXPECT_TRUE(rule.matcher->matches("query=drop       table users"));
    EXPECT_FALSE(rule.matcher->matches("query=select users"));
}

TEST(RulePreparerTest, RejectsInvalidPcre2Rule) {
    RuleSet ruleSet{
        .rules = {
            {
                .id = 102,
                .target = RuleTarget::Body,
                .matcher = MatcherType::Pcre2,
                .pattern = R"((drop\s+table)",
                .action = RuleAction::Block
            }
        }
    };

    RulePreparer preparer;

    EXPECT_THROW(preparer.prepare(ruleSet), std::runtime_error);
}

TEST(RulePreparerTest, PreparesMultipleRules) {
    RuleSet ruleSet{
        .rules = {
            {
                .id = 100,
                .target = RuleTarget::Path,
                .matcher = MatcherType::String,
                .pattern = "admin",
                .action = RuleAction::Block
            },
            {
                .id = 101,
                .target = RuleTarget::Body,
                .matcher = MatcherType::String,
                .pattern = "drop table",
                .action = RuleAction::Block
            },
            {
                .id = 102,
                .target = RuleTarget::Body,
                .matcher = MatcherType::Pcre2,
                .pattern = R"(drop\s+table)",
                .action = RuleAction::Block
            }
        }
    };

    RulePreparer preparer;

    PreparedRuleSet prepared = preparer.prepare(ruleSet);

    ASSERT_EQ(prepared.rules.size(), 3);

    EXPECT_EQ(prepared.rules[0].id, 100);
    EXPECT_EQ(prepared.rules[1].id, 101);
    EXPECT_EQ(prepared.rules[2].id, 102);
}
