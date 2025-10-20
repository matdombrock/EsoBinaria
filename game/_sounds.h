// Deprecaded?
#pragma once
#include "../src/Imp.h"
using namespace Imp;

namespace Sounds {
    inline Sound menuTrack;
    inline Sound changeTile;
    inline Sound toggleScreenMainMenu;
    inline Sound toggleTests;
    inline void init() {
        menuTrack = Sound("menu.ogg");
        changeTile = Sound("2.wav");
        toggleScreenMainMenu = Sound("3.wav");
        toggleTests = Sound("4.wav");
    }
};
