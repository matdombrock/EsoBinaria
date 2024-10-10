#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class BtnNub : public Btn {
public:
    int index;
    BtnNub() : Btn() {
        tag = "nub";
        index = 0;
        size = Vec2i(_g.vu(0.25f), _g.vu(0.25f));
        setCollider(Vec2i(_g.cellSize / 2, _g.cellSize / 2));
    }
    ~BtnNub() {}
    void render(Graphics* graph) override {
        if (!available) return;
        graph->setColor(state ? _colors["GREEN"] : _colors["GRAY"]);
        if (_g.getPuzzleNum() == index) {
            graph->setColor(_colors["YELLOW"]);
        }
        // graph->rect(pos, size, true);
        graph->tri(pos, pos + Vec2i(size.x, 0), pos + Vec2i(0, size.y), _g.getTick() * (index/256.0f) / 512.0f);
        // graph->setColor(_colors["BG"]);
        graph->tri(pos + Vec2i(size.x, size.y), pos + Vec2i(size.x, 0), pos + Vec2i(0, size.y), _g.getTick() * (index/256.0f) / 512.0f);
    }
};