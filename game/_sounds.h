// Deprecaded?
#pragma once
#include "../src/Imp.h"
using namespace Imp;

namespace Sounds {
    Sound menuTrack;
    Sound changeTile;
    Sound toggleScreenMainMenu;
    Sound toggleTests;
    void init() {
        menuTrack = Sound("menu.ogg");
        changeTile = Sound("2.wav");
        toggleScreenMainMenu = Sound("3.wav");
        toggleTests = Sound("4.wav");
    }
};