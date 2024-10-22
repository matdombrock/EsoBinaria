#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class BtnTopMenu : public BtnText {
public:
    bool isHomeBtn;
    BtnTopMenu() : BtnText() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        fontSize = _g.fontSize * 0.75f;
        size = Vec2i(80, fontSize * 1.1f);
        isHomeBtn = false;
        setCollider(size);
    }
    ~BtnTopMenu() {}
    void render(Graphics* graph) override {
        if (!available) return;
        // graph->setColor(_colors["BG2"]);
        // graph->rect(pos, size, true);
        Color* c = &_colors["GRAY"];
        switch (state) {
            case 0: c = &_colors["GRAY"]; break;
            case 1: c = &_colors["YELLOW"]; break;
            case 2: c = &_colors["GREEN"]; break;
        }
        graph->setColor(*c);
        if (isHomeBtn) {
            int sz = _g.vu(0.25f);
            //graph->circle(pos + Vec2i(sz/2, sz/2), sz/2, true);
            // draw a diamond with triangles
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(sz, sz/2), pos + Vec2i(sz/2, sz));
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(0, sz/2), pos + Vec2i(sz/2, sz));

            return;
        }
        if (center) {
            int textWidth = graph->textWidth(text);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - fontSize) / 2);
            graph->text(text, textPos);
        }
        else {
            graph->text(" " + text + " ", pos);
        }
    }
};