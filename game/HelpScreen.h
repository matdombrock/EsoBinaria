#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"


class HelpScreen : public Entity {
public:
    EntityManager em;
    std::string text;
    Sprite sprWiz;
    HelpScreen() : Entity() {
        tag = "helpScreen";
        sprWiz = Sprite(Vec2i(0, 224), Vec2i(32,32), Vec2i(_g.vu(2), _g.vu(2)));
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
 
------------------------------------------------
<$00FF00$>PRESS THE <ESC> KEY TO ENTER ESO-OS
)";
    }
    ~HelpScreen() {}
    void process() override {
        if (_g.getScreen() != tag) return;
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != tag) return;
        graph->setColor(_colors["BG3"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE, true);
        graph->setColor(_colors["BG"], 128);
        graph->rect(Vec2i(_g.vu(0.5f), _g.vu(0.5f)), WINDOW_SIZE - Vec2i(_g.vu(1), _g.vu(1)), true);
        // (_g.getTick()/16) % 2 == 0 ? graph->setColor(_colors["YELLOW"]) : graph->setColor(_colors["RED"]);
        graph->setColor(_colors["YELLOW"]);
        // graph->rect(
        //     Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.25f), _g.vu(0.75f)), 
        //     Vec2i(_g.vu(1.5f), _g.vu(1.5f)),
        //     true
        // );
        int rad = (_g.getTick()/16) % 2 == 0 ? _g.vu(0.7f) : _g.vu(0.75f);
        graph->circle(
            Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.25f) + _g.vu(0.75f), _g.vu(0.75f) + _g.vu(0.75f)),
            rad,
            true
        );
        sprWiz.render(graph, Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.5f), _g.vu(0.5f)));

        graph->setColor(_colors["GREEN"]);
        int pos = std::min((_g.getTick()/3)*3, (int)(text.length() - 1));

        std::string textTrunc = text.substr(0, pos);
        graph->textFmt(textTrunc, Vec2i(_g.vu(1), _g.vu(2)), WINDOW_SIZE.x - _g.vu(2));
    }
};