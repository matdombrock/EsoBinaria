#pragma once
#include "Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_fonts.h"
#include "_gameMaster.h"
#include "_text.h"

class ScreenEmail : public Entity {
public:
  EntityManager em;
  std::string text;
  Sprite sprWiz =
      Sprite(Vec2i(0, 224), Vec2i(32, 32), Vec2i(_g.vu(2), _g.vu(2)));
  Sound sndType = Sound("tick.wav");
  Font font = Font("HomeVideo.ttf", 24);
  ScreenEmail() : Entity() {
    tag = "ScreenEmail";
    sprWiz.setAnimation({Vec2i(0, 224), Vec2i(32, 224)}, 4);
    text = Text::introEmail;
  }
  ~ScreenEmail() {}
  void process() override {
    if (_g.getScreen() != SCN_HELP)
      return;
    if (init == false) {
      DBG("Starting Email Intro");
      init = true;
      pressed = 0;
      startTicks = _g.getTick();
      _g.store.setBool("completed_email_intro", true);
    }
    else if (GInput.anyKeyOnce()) {
      pressed++;
      if (pressed > 1) {
        _g.setScreen(SCN_PUZZLE);
      }
    }
  }
  void render(Graphics *graph) override {
    if (_g.getScreen() != SCN_HELP)
      return;
    graph->setColor(_colors["BG"], 128);
    graph->rect(Vec2i(_g.vu(0.5f), _g.vu(0.5f)),
                WINDOW_SIZE - Vec2i(_g.vu(1), _g.vu(1)), true);
    graph->setColor(_colors["YELLOW"]);
    int tick = (_g.getTick() - startTicks);
    int rad = (tick / 16) % 2 == 0 ? _g.vu(0.7f) : _g.vu(0.75f);
    graph->circle(Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.25f) + _g.vu(0.75f),
                        _g.vu(0.75f) + _g.vu(0.75f)),
                  rad, true);
    sprWiz.render(graph,
                  Vec2i(WINDOW_SIZE.x - _g.vu(2) - _g.vu(0.5f), _g.vu(0.5f)));

    graph->setColor(_colors["GREEN"]);
    int pos = std::min((tick / 3) * 6, (int)text.length() - 1);
    if (pressed > 0)
      pos = text.length() - 1;
    if (pos < (int)text.length() - 1) {
      sndType.play(1, true);
    } else {
      sndType.stop();
    }
    std::string textTrunc = text.substr(0, pos);
    graph->textFmt(textTrunc, Vec2i(_g.vu(1), _g.vu(2)), &Fonts::small,
                   WINDOW_SIZE.x - _g.vu(2));
  }

private:
  bool init = false;
  int pressed = 0;
  int startTicks = 0;
};
