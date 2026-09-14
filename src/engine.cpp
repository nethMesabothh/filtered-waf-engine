//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#include "../include/engine.h"

std::string_view Engine::getTargetValue(const WafRequest &request, RuleTarget target) const {
    switch (target) {
        case RuleTarget::Path:
            return request.path;
        case RuleTarget::Body:
            return request.body;
    }

    return {};
}

Decision Engine::inspect(const WafRequest &request, const RuleSet& ruleSet) const {
    for (const Rule &rule: ruleSet.rules) {


        std::string_view targetValue = getTargetValue(request, rule.target);

        bool matched = targetValue.find(rule.pattern) != std::string::npos;

        if(matched) {
            if (rule.action == RuleAction::Block) {
                return {
                    .action = DecisionAction ::Block,
                    .matchedRuleId = rule.id,
                };
            }
        }

    }
    return {
        .action = DecisionAction ::Allow,
        .matchedRuleId = -1,
    };
}
