#pragma once
#include "../src/Imp.h"
using namespace Imp;

class Cursor : public Entity {
public:
    Sprite spr;
    Cursor() : Entity() {
        tag = "cursor";
        pos = Vec2i(0, 0);
        spr = Sprite(Vec2i(0, 64), Vec2i(16, 16), Vec2i(32,32));
        Util::hideMouse();
    }
    ~Cursor() {}
    void process() override {
        pos = _input.mousePos();
    }
    void render(Graphics* graph) override {
        spr.render(graph, pos);
    }
};