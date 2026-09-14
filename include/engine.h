//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_ENGINE_H
#define FILTERED_WAF_ENGINE_ENGINE_H

#endif //FILTERED_WAF_ENGINE_ENGINE_H

#pragma once
#include "waf_request.hpp"
#include "decision.h"
#include "rule_set.h"

class Engine {
public:
    Decision inspect(const WafRequest &request, const RuleSet &rules) const;

    private: std::string_view getTargetValue(const WafRequest &request, RuleTarget target) const;
};
