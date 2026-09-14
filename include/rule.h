//
// Created by Neth_Mesaboth(Neth_M on 9/11/2026.
//

#ifndef FILTERED_WAF_ENGINE_RULE_H
#define FILTERED_WAF_ENGINE_RULE_H

#endif //FILTERED_WAF_ENGINE_RULE_H

#pragma once

#include <string>

enum class RuleTarget {
    Path,
    Body
};

enum class RuleAction {
    Allow,
    Block,
};

enum class MatcherType {
    String
};

struct Rule {
    int id;
    RuleTarget target;
    MatcherType matcher;
    std::string pattern;
    RuleAction action;
};