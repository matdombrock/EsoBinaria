#pragma once
#include <string>
#include <vector>

class TestData {
public:
    int index;
    std::vector<bool> inputs;
    bool output;
    std::string err;
    bool lastCheck;
};