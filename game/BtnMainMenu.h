#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class BtnMainMenu : public Entity {
public:
    Vec2i size;
    std::string text;
    Uint8 state;
    bool center;
    BtnMainMenu() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(200, _g.fontSize * 1.1f);
        setCollider(size);
    }
    ~BtnMainMenu() {}
    bool isClicked() {
        return state == 2;
    }
    void process() override {}
    void render(Graphics* graph) override {
        Color* c = &_colors["GRAY"];
        switch (state) {
            case 0: c = &_colors["GRAY"]; break;
            case 1: c = &_colors["YELLOW"]; break;
            case 2: c = &_colors["GREEN"]; break;
        }
        std::string textMod = (state > 0 ? ">> " : "") + text;
        graph->setColor(*c);
        if (center) {
            int textWidth = graph->textWidth(textMod, _g.fontSize);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - _g.fontSize) / 2);
            graph->text(textMod, textPos, _g.fontSize);
        }
        else {
            graph->text(textMod, pos, _g.fontSize);
        }
    }
    void onMouse(bool over) override {
        if (_input.mouseKeyOnce(SDL_BUTTON_LEFT) && over) {
            state = 2;
        }
        else if (over) {
            state = 1;
        }
        else state = 0;
    }
};