#include <iostream>

#include "include/waf_request.hpp"
#include "include/rule.h"

int main() {
    WafRequest request{
        "POST",
        "/login",
        "username=john"
    };

    Rule rule{
        .id = 100,
        .pattern = "login"
    };

    if (request.path.find(rule.pattern) != std::string::npos) {
        std::cout << "Result: BLOCK\n";
    }else {
        std::cout << "Result: ALLOW\n";
    }
    return 0;
}
