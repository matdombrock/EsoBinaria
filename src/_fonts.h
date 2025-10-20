#pragma once
#include "Imp.h"
using namespace Imp;

namespace Fonts {
    inline Font medium;
    inline Font large;
    inline Font small;
    inline void init(int fontSize) {
        medium = Font("HomeVideo.ttf", fontSize);
        large = Font("HomeVideo.ttf", fontSize * 4);
        small = Font("HomeVideo.ttf", fontSize * 0.75f);
    }
}
