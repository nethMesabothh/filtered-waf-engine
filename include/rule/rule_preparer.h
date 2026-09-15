//
// Created by Neth_Mesaboth(Neth_M on 9/15/2026.
//

#ifndef FILTERED_WAF_ENGINE_RULE_PREPARER_H
#define FILTERED_WAF_ENGINE_RULE_PREPARER_H

#endif //FILTERED_WAF_ENGINE_RULE_PREPARER_H
#pragma once

#include "rule/rule_set.h"
#include "rule/prepared_rule_set.h"

class RulePreparer {
public:
    PreparedRuleSet prepare(
        const RuleSet& ruleSet
    ) const;
};