#pragma once

#include <string>

struct WafRequest {
    std::string method;
    std::string path;
    std::string body;
};