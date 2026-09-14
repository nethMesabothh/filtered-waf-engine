//
// Created by Neth_Mesaboth(Neth_M on 9/14/2026.
//

#ifndef FILTERED_WAF_ENGINE_DECISION_H
#define FILTERED_WAF_ENGINE_DECISION_H

#endif //FILTERED_WAF_ENGINE_DECISION_H

enum class DecisionAction  {
    Allow,
    Block
};

struct Decision {
    DecisionAction  action;
    int matchedRuleId;
};