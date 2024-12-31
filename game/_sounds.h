// Deprecaded?
#pragma once
#include "../src/Imp.h"
using namespace Imp;

namespace Sounds {
    Sound menuTrack;
    Sound changeTile;
    Sound toggleMainMenu;
    Sound toggleTests;
    void init() {
        menuTrack = Sound("menu.ogg");
        changeTile = Sound("2.wav");
        toggleMainMenu = Sound("3.wav");
        toggleTests = Sound("4.wav");
    }
};