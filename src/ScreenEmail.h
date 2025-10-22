/*
 * The ScreenEmail class displays an introductory email screen with animated
 * text and a wizard sprite.
 */
#pragma once
#include "Imp.h"
using namespace Imp;
#include "g_colors.h"
#include "g_fonts.h"
#include "g_gameMaster.h"
#include "g_text.h"

class ScreenEmail : public Entity {
public:
  EntityManager em;
  std::string text;
  Sprite sprWiz =
      Sprite(Vec2i(0, 224), Vec2i(32, 32), Vec2i(g_gm.vu(2), g_gm.vu(2)));
  Sound sndType = Sound("tick.wav");
  Font font = Font("HomeVideo.ttf", 24);
  ScreenEmail() : Entity() {
    tag = "ScreenEmail";
    sprWiz.setAnimation({Vec2i(0, 224), Vec2i(32, 224)}, 4);
    text = Text::introEmail;
  }
  ~ScreenEmail() {}
  void process() override {
    if (g_gm.getScreen() != SCN_HELP)
      return;
    if (init == false) {
      DBG("Starting Email Intro");
      init = true;
      pressed = 0;
      startTicks = g_gm.getTick();
      g_gm.store.setBool("completed_email_intro", true);
    } else if (GInput.anyKeyOnce()) {
      pressed++;
      if (pressed > 1) {
        g_gm.setScreen(SCN_PUZZLE);
      }
    }
  }
  void render(Graphics *graph) override {
    if (g_gm.getScreen() != SCN_HELP)
      return;
    graph->setColor(g_colors["BG"], 128);
    graph->rect(Vec2i(g_gm.vu(0.5f), g_gm.vu(0.5f)),
                WINDOW_SIZE - Vec2i(g_gm.vu(1), g_gm.vu(1)), true);
    graph->setColor(g_colors["YELLOW"]);
    int tick = (g_gm.getTick() - startTicks);
    int rad = (tick / 16) % 2 == 0 ? g_gm.vu(0.7f) : g_gm.vu(0.75f);
    graph->circle(Vec2i(WINDOW_SIZE.x - g_gm.vu(2) - g_gm.vu(0.25f) + g_gm.vu(0.75f),
                        g_gm.vu(0.75f) + g_gm.vu(0.75f)),
                  rad, true);
    sprWiz.render(graph,
                  Vec2i(WINDOW_SIZE.x - g_gm.vu(2) - g_gm.vu(0.5f), g_gm.vu(0.5f)));

    graph->setColor(g_colors["GREEN"]);
    int pos = std::min((tick / 3) * 6, (int)text.length() - 1);
    if (pressed > 0)
      pos = text.length() - 1;
    if (pos < (int)text.length() - 1) {
      sndType.play(1, true);
    } else {
      sndType.stop();
    }
    std::string textTrunc = text.substr(0, pos);
    graph->textFmt(textTrunc, Vec2i(g_gm.vu(1), g_gm.vu(2)), &Fonts::small,
                   WINDOW_SIZE.x - g_gm.vu(2));
  }

private:
  bool init = false;
  int pressed = 0;
  int startTicks = 0;
};
