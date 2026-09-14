#include <iostream>

#include "include/waf_request.hpp"
#include "include/engine.h"
#include "include/rule_set.h"

int main() {
    WafRequest request{
        "POST",
        "secret/login",
        "username=john"
    };

    WafRequest request1{
        .method = "POST",
        .path = "admin/login",
        .body = "query=drop table users"
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
            }
        }
    };

    Engine engine;

    Decision decision = engine.inspect(request1, ruleSet);

    if (decision.action == DecisionAction::Block) {
        std::cout << "Result: BLOCK\n";
        std::cout << "Matched RuleID: " << decision.matchedRuleId << std::endl;
    } else {
        std::cout << "Result: ALLOW\n";
    }
    return 0;
}
