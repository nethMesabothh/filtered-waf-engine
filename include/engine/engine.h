//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_ENGINE_H
#define FILTERED_WAF_ENGINE_ENGINE_H

#endif //FILTERED_WAF_ENGINE_ENGINE_H

#pragma once
#include "core/waf_request.hpp"
#include "core/decision.h"
#include "rule/rule_set.h"
#include "matcher/matcher.h"
#include "matcher/string_matcher.h"
#include "rule/prepared_rule_set.h"
#include "matcher/pcre2_matcher.h"

class Engine {
public:
    Decision inspect(const WafRequest &request, const PreparedRuleSet &rules) const;

private:
    std::string_view getTargetValue(const WafRequest &request, RuleTarget target) const;

};
