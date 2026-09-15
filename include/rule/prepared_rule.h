//
// Created by Neth_Mesaboth(Neth_M on 9/15/2026.
//

#ifndef FILTERED_WAF_ENGINE_PREPARED_RULE_H
#define FILTERED_WAF_ENGINE_PREPARED_RULE_H

#endif //FILTERED_WAF_ENGINE_PREPARED_RULE_H

#pragma once

#include <memory>

#include "matcher/matcher.h"
#include "rule/rule.h"

struct PreparedRule {
    int id;
    RuleTarget target;
    RuleAction action;

    std::unique_ptr<Matcher> matcher;
};