#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"


class HelpScreen : public Entity {
public:
    EntityManager em;
    std::string text = "EsoMachina";
    HelpScreen() : Entity() {
        tag = "helpScreen";
        text = R"(<$FFFF00$>>> EsoMachina: 
<$FFFFFF$>
Objective:
- Each puzzle involves creating a solution that satisfies a set of test cases.
- The solution is a function that takes N inputs (A, B, C) and returns a single output.
- The function can be any combination of AND, OR, NOT, XOR, NAND, NOR, and XNOR gates.
)";
    }
    ~HelpScreen() {}
    void process() override {
        if (_g.getScreen() != tag) return;
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != tag) return;
        DBG("Render help screen");
        graph->setColor(_colors["GREEN"]);
        // graph->text("EsoMachina", Vec2i(0, 0), _g.fontSize * 4);
//         graph->textFmt(text, Vec2i(0, 0), _g.fontSize, "HomeVideo.ttf", WINDOW_SIZE.x);
        
        graph->textFmt(text, Vec2i(0, 0), WINDOW_SIZE.x);
    }
};