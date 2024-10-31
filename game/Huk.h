#pragma once
#include "../src/Imp.h"
#include "_gameMaster.h"
#include "_colors.h"
#include "_fonts.h"
#include "_text.h"
using namespace Imp;

class Huk : public Entity {
    Sprite spr = Sprite(Vec2i(64, 224), Vec2i(32,32), Vec2i(_g.vu(2), _g.vu(2)));
    bool show = false;
public:
    Huk() : Entity() {
        tag = "huk";
        // spr.setAnimation({Vec2i(0, 0), Vec2i(32, 0)}, 4);
    }
    ~Huk() {}
    void reset() {
        hasShown = false;
        messageIndex = 0;
    }
    void process() override {
        if (_g.getScreen() != SCN_PUZZLE) return;
        if (!hasShown && _g.getPuzzleBits() == 3 && _g.getPuzzleChallenge() == 'e') {
            show = true;
            hasShown = true;
            if (_g.getPuzzleNum() == 0) {
                messages = {Text::huk0a, Text::huk0b, Text::huk0c, Text::huk0d, Text::huk0e, Text::huk0f, Text::huk0g, Text::huk0h, Text::huk0i, Text::huk0j, Text::huk0k};
            }
            if (_g.getPuzzleNum() == 1) {
                messages = {Text::huk1a, Text::huk1b, Text::huk1c, Text::huk1d, Text::huk1e, Text::huk1f, Text::huk1g, Text::huk1h, Text::huk1i, Text::huk1j, Text::huk1k};
            }
            if (_g.getPuzzleNum() == 2) {
                messages = {Text::huk2a, Text::huk2b, Text::huk2c, Text::huk2d, Text::huk2e, Text::huk2f};
            }
            if (_g.getPuzzleNum() == 3) {
                messages = {Text::huk3a, Text::huk3b, Text::huk3c, Text::huk3d, Text::huk3e, Text::huk3f};
            }
        }
        if (!show) return;
        if (_input.anyKeyOnce()) {
            messageIndex++;
            if (messageIndex >= messages.size()) {
                show = false;
            }
            // show = false;
        }
    }
    void render(Graphics* graph) override {
        if (!show) return;
        int topY = WINDOW_SIZE.y - _g.vu(8);
        graph->setColor(_colors["GRAY"]);
        graph->rect(Vec2i(0, topY), Vec2i(WINDOW_SIZE.x, _g.vu(4)));
        graph->setColor(_colors["BG3"]);
        float inOff = _g.vu(0.1f);
        float inOff2 = inOff * 2;
        graph->rect(Vec2i(inOff, topY + inOff), Vec2i(WINDOW_SIZE.x - inOff2, _g.vu(4) - inOff2));
        
        graph->setColor(_colors["BLUE"]);
        inOff = _g.vu(0.25f) + _g.vu(0.01f * ((_g.getTick() / 2) % 16));
        inOff2 = inOff * 2;
        graph->rect(Vec2i(inOff, topY + inOff), Vec2i(_g.vu(2) - inOff2, _g.vu(2) - inOff2));
        spr.render(graph, Vec2i(0, topY));

        graph->setColor(_colors["GREEN"]);
        if (messageIndex == messages.size() - 1) {
            graph->setColor(_colors["YELLOW"]);
        }
        graph->textFmt(messages[messageIndex], Vec2i(_g.vu(2), topY + _g.vu(0.5f)), &Fonts::medium, WINDOW_SIZE.x - _g.vu(2.25f));
    }
private:
    std::vector<std::string> messages = {};
    int messageIndex = 0;
    bool hasShown = false;
};