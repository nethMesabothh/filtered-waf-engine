//
// Created by Neth_Mesaboth(Neth_M on 9/15/2026.
//
#include "rule/rule_preparer.h"

#include "matcher/string_matcher.h"
#include "matcher/pcre2_matcher.h"

#include <memory>

PreparedRuleSet RulePreparer::prepare(
    const RuleSet& ruleSet
) const {
    PreparedRuleSet preparedRuleSet;

    for (const Rule& rule : ruleSet.rules) {

        std::unique_ptr<Matcher> matcher;

        switch (rule.matcher) {

            case MatcherType::String:
                matcher =
                    std::make_unique<StringMatcher>(
                        rule.pattern
                    );
                break;

            case MatcherType::Pcre2:
                matcher =
                    std::make_unique<Pcre2Matcher>(
                        rule.pattern
                    );
                break;
        }

        preparedRuleSet.rules.push_back(
            PreparedRule{
                .id = rule.id,
                .target = rule.target,
                .action = rule.action,
                .matcher = std::move(matcher)
            }
        );
    }

    return preparedRuleSet;
}