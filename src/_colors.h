/*
* Predefined colors for Imp applications
* Available globally via _colors
*/
#pragma once
#include <string>
#include <map>
#include "Imp.h"
using namespace Imp;

inline std::map<std::string, Color> _colors = {
    {"BG", Color(20,25,20)},
    {"BG2", Color(45,50,45)},
    {"BG3", Color(55,55,60)},
    {"GREEN", Color(94,180,93)},
    {"YELLOW", Color(238,198,75)},
    {"RED", Color(186,89,90)},
    {"BLUE", Color(77,151,120)},
    {"GRAY", Color(155,155,155)},
    {"WHITE", Color(255,255,255)},
};
