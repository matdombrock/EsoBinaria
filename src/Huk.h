#pragma once
#include "Imp.h"
#include "_gameMaster.h"
#include "_colors.h"
#include "_fonts.h"
#include "_text.h"
using namespace Imp;

class Huk : public Entity {
    Sprite spr = Sprite(Vec2i(64, 224), Vec2i(32,32), Vec2i(_g.vu(2), _g.vu(2)));
    Sound sndNext = Sound("wiggle1.wav");
    bool show = false;
public:
    Huk() : Entity() {
        tag = "huk";
        spr.setAnimation({Vec2i(64, 224), Vec2i(96, 224)}, 8);
        sndNext.volume = 16;
    }
    ~Huk() {}
    void reset() {
        hasShown = false;
        messageIndex = 0;
    }
    void process() override {
        if (_g.getScreen() != SCN_PUZZLE) return;
        // TODO: All this code for showing the text is a bit messy
        if (!hasShown && _g.getPuzzleBits() == 3 && _g.getPuzzleChallenge() == 'e') {
            show = true;
            hasShown = true;
            _g.setHukActive(true);
            sndNext.play();
            if (_g.getPuzzleNum() == 0) {
                messages = {Text::hukE0a, Text::hukE0b, Text::hukE0c, Text::hukE0d, Text::hukE0e, Text::hukE0f, Text::hukE0g, Text::hukE0h, Text::hukE0i, Text::hukE0j, Text::hukE0k};
            }
            if (_g.getPuzzleNum() == 1) {
                messages = {Text::hukE1a, Text::hukE1b, Text::hukE1c, Text::hukE1d, Text::hukE1e, Text::hukE1f, Text::hukE1g, Text::hukE1h, Text::hukE1i, Text::hukE1j, Text::hukE1k};
            }
            if (_g.getPuzzleNum() == 2) {
                messages = {Text::hukE2a, Text::hukE2b, Text::hukE2c, Text::hukE2d, Text::hukE2e, Text::hukE2f};
            }
            if (_g.getPuzzleNum() == 3) {
                messages = {Text::hukE3a, Text::hukE3b, Text::hukE3c, Text::hukE3d, Text::hukE3e, Text::hukE3f};
            }
        }
        if (!hasShown && _g.getPuzzleBits() == 3 && _g.getPuzzleChallenge() == 'm') {
            if (_g.getPuzzleNum() == 0) {
                show = true;
                hasShown = true;
                _g.setHukActive(true);
                sndNext.play();
                messages = {Text::hukM0a, Text::hukM0b, Text::hukM0c, Text::hukM0d, Text::hukM0e, Text::hukM0f};
            }
            if (_g.getPuzzleNum() == 1) {
                show = true;
                hasShown = true;
                _g.setHukActive(true);
                sndNext.play();
                messages = {Text::hukM1a, Text::hukM1b, Text::hukM1c, Text::hukM1d, Text::hukM1e, Text::hukM1f, Text::hukM1g};
            }
        }
        if (!hasShown && _g.getPuzzleBits() == 3 && _g.getPuzzleChallenge() == 'h') {
            if (_g.getPuzzleNum() == 69) {
                show = true;
                hasShown = true;
                _g.setHukActive(true);
                sndNext.play();
                messages = {Text::hukH69a};
            }
        }
        if (!show) return;
        if (GInput.anyKeyOnce()) {
            messageIndex++;
            if (messageIndex >= messages.size()) {
                show = false;
                _g.setHukActive(false);
            }
            else sndNext.play();
        }
    }
    void render(Graphics* graph) override {
        if (!show) return;
        int topY = WINDOW_SIZE.y - _g.vu(8);
        graph->setColor(_colors["GRAY"]);
        graph->rect(Vec2i(0 + _g.vu(0.25f), topY), Vec2i(WINDOW_SIZE.x - _g.vu(0.5f), _g.vu(4)));
        graph->setColor(_colors["BG3"]);
        float inOff = _g.vu(0.1f);
        float inOff2 = inOff * 2;
        graph->rect(Vec2i(inOff + _g.vu(0.25f), topY + inOff), Vec2i(WINDOW_SIZE.x - inOff2 - _g.vu(0.5f), _g.vu(4) - inOff2));
        
        graph->setColor(_colors["BLUE"]);
        inOff = _g.vu(0.25f) + _g.vu(0.01f * ((_g.getTick() / 2) % 16));
        inOff2 = inOff * 2;
        graph->rect(Vec2i(inOff, topY + inOff), Vec2i(_g.vu(2) - inOff2, _g.vu(2) - inOff2));
        spr.render(graph, Vec2i(0, topY));

        graph->setColor(_colors["GREEN"]);
        if (messageIndex == messages.size() - 1) {
            graph->setColor(_colors["YELLOW"]);
        }
        graph->textFmt(messages[messageIndex], Vec2i(_g.vu(2), topY + _g.vu(0.5f)), &Fonts::medium, WINDOW_SIZE.x - _g.vu(2.5f));
    }
private:
    std::vector<std::string> messages = {};
    int messageIndex = 0;
    bool hasShown = false;
};
