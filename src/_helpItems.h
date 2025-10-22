/*
* Help items for the logic gate simulator
*/
#pragma once
#include <string>
#include <map>

class HelpItem {
public:
    std::string title;
    std::string desc;
    std::string key;
};

inline std::map<std::string, HelpItem> g_helpItems = {
    {"A", {"IN_A", "Input A", "A"}},
    {"B", {"IN_B", "Input B", "S"}},
    {"C", {"IN_C", "Input C", "D"}},
    {"D", {"IN_D", "Input D", "F"}},
    {"VOID", {"CLEAR", "Clear the cell", "q"}},
    {"BLANK", {"BLANK", "Blank cell", "w"}},
    {"AND", {"AND", "Outputs true if both inputs are true", "e"}},
    {"OR", {"OR", "Outputs true if either input is true", "r"}},
    {"NOT", {"NOT", "Outputs true if input is false", "t"}},
    {"XOR", {"XOR", "Outputs true if inputs are different", "y"}},
    {"NAND", {"NAND", "Outputs false if both inputs are true", "u"}},
    {"NOR", {"NOR", "Outputs false if either input is true", "i"}},
    {"XNOR", {"XNOR", "Outputs false if inputs are different", "o"}},
};
