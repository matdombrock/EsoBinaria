#pragma once
#include "../src/Imp.h"
using namespace Imp;

namespace Fonts {
    Font medium;
    Font large;
    Font small;
    void init(int fontSize) {
        medium = Font("HomeVideo.ttf", fontSize);
        large = Font("HomeVideo.ttf", fontSize * 4);
        small = Font("HomeVideo.ttf", fontSize * 0.75f);
    }
}