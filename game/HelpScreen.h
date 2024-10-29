#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"
#include "_fonts.h"


class HelpScreen : public Entity {
public:
    EntityManager em;
    std::string text;
    Sprite sprWiz = Sprite(Vec2i(0, 224), Vec2i(32,32), Vec2i(_g.vu(2), _g.vu(2)));
    Sound sndType = Sound("1.wav");
    Font font = Font("HomeVideo.ttf", 24);
    HelpScreen() : Entity() {
        tag = "helpScreen";
        sprWiz.setAnimation({Vec2i(0, 224), Vec2i(32, 224)}, 4);
        // sndType.set("1.wav");
        text = R"(<$FFFF00$>>> New e-Hail: <<: 
<$00FF00$>
Welcome:
<$FFFFFF$>
You have been accepted into the <$FF00FF$>EsoBinaria Research Program<$FFFFFF$>.
Our goal here is to <$FF0000$>[REDACTED]<$FFFFFF$> the EsoBinaria device.
To do this we must understand how it works.
We have a tome documenting its inputs and outputs but no instructions for how to build the device.
You aim is to recreate small parts of the mechanism via reverse-<$FF0000$>[REDACTED]<$FFFFFF$>.
<$00FF00$>
Mission Statement:
<$FFFFFF$>
We are the binary star system, two parts that orbit each other.
We are the eternal void.
We are the light.
We are the EsoBinaria, the device that we seek to <$FF0000$>[REDACTED]<$FFFFFF$>.

------------------------------------------------
<$00FF00$>PRESS THE <ESC> KEY TO ENTER ESO-OS
)";
    }
    ~HelpScreen() {}
    void process() override {
        if (_g.getScreen() != SCN_HELP) return;
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != SCN_HELP) return;
        graph->setColor(_colors["BG3"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE, true);
        graph->setColor(_colors["BG"], 128);
        graph->rect(Vec2i(_g.vu(0.5f), _g.vu(0.5f)), WINDOW_SIZE - Vec2i(_g.vu(1), _g.vu(1)), true);
        graph->setColor(_colors["YELLOW"]);
        int rad = (_g.getTick()/16) % 2 == 0 ? _g.vu(0.7f) : _g.vu(0.75f);
        graph->circle(
            Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.25f) + _g.vu(0.75f), _g.vu(0.75f) + _g.vu(0.75f)),
            rad,
            true
        );
        sprWiz.render(graph, Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.5f), _g.vu(0.5f)));

        graph->setColor(_colors["GREEN"]);
        int pos = std::min((_g.getTick()/3)*3, (int)text.length() - 1);
        if (pos < (int)text.length() - 1) {
            sndType.play(true);
        }
        else {
            sndType.stop();
        }
        std::string textTrunc = text.substr(0, pos);
        graph->textFmt(textTrunc, Vec2i(_g.vu(1), _g.vu(2)), &Fonts::small, WINDOW_SIZE.x - _g.vu(2));
    }
};