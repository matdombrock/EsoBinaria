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
        text = R"(<$FFFF00$>>> New e-Hail: <<: 
<$00FF00$>
Welcome:
<$FFFFFF$>
You have been accepted into the <$FF00FF$>EsoMachina Research Program<$FFFFFF$>.
Our goal here is to <$FF0000$>[REDACTED]<$FFFFFF$> the EsoMachina device.
To do this we must understand how it works.
We have a tome documenting its inputs and outputs but no instructions for how to build the device.
You aim is to recreate small parts of the mechanism via reverse-<$FF0000$>[REDACTED]<$FFFFFF$>.
<$00FF00$>
Objective:
<$FFFFFF$>
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
        int pos = std::min((_g.getTick()/3)*3, (int)(text.length() - 1));

        std::string textTrunc = text.substr(0, pos);
        graph->textFmt(textTrunc, Vec2i(_g.vu(1), _g.vu(1)), WINDOW_SIZE.x - _g.vu(2));
    }
};