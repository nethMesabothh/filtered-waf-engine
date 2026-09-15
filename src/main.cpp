#include <iostream>

#include "core/waf_request.hpp"
#include "engine/engine.h"
#include "rule/rule_set.h"
#include "rule/rule_preparer.h"

int main() {
    WafRequest request{
        .method = "POST",
        .path = "/login",
        .body = "query=drop       table users"
    };

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


    try {
        RulePreparer preparer;
        PreparedRuleSet preparedRuleSet =
                preparer.prepare(ruleSet);

        Engine engine;

        Decision decision =
                engine.inspect(
                    request,
                    preparedRuleSet
                );

        if (decision.action == DecisionAction::Block) {
            std::cout << "Result: BLOCK\n";
            std::cout
                    << "Matched RuleID: "
                    << decision.matchedRuleId
                    << '\n';
        } else {
            std::cout << "Result: ALLOW\n";
        }

        return 0;
    } catch (const std::exception &e) {
        std::cerr
                << "Rule preparation failed: "
                << e.what()
                << '\n';

        return 1;
    }
}
