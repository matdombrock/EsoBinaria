#pragma once
#include "GameMaster.h"
#include "Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_fonts.h"
#include "_gameMaster.h"

#ifndef BUILD_TIME
#define BUILD_TIME 1234
#endif

class ScreenBumper : public Entity {
public:
  Sprite sprBg;
  Sprite sprTitle;
  Sprite sprSDL;
  Sprite sprCPP;
  Font font = Font("HomeVideo.ttf", _g.fontSize);
  Input input;
  ScreenBumper() : Entity() {
    tag = "bumper";

    sprBg = Sprite(Vec2i(176, 176), Vec2i(96, 96),
                   Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.x / 2));

    sprTitle = Sprite(Vec2i(80, 80), Vec2i(96, 32), Vec2i(96 * 8, 32 * 8));
    sprSDL = Sprite(Vec2i(176, 80), Vec2i(48, 32), Vec2i(48 * 4, 32 * 4));
    sprCPP = Sprite(Vec2i(224, 80), Vec2i(32, 32), Vec2i(32 * 4, 32 * 4));
  }
  ~ScreenBumper() {}
  void process() override {
    if (_g.getScreen() != SCN_BUMPER) {
      return;
    }
    input.poll();
    bool pressed = input.anyKey();
    // if (pressed || (_g.getTick() / WINDOW_FPS) > 5) {
    if (pressed) {
      DBG("Exiting bumper via keypress");
      if (_g.store.getBool("completed_email_intro")) {
        _g.setScreen(SCN_MAIN_MENU);
      } else {
        _g.setScreen(SCN_HELP);
      }
    }
  }
  void render(Graphics *graph) override {
    if (_g.getScreen() != SCN_BUMPER)
      return;
    graph->setColor(_colors["GRAY"]);
    graph->text("ESOBINARIA", Vec2i(20, 20), &Fonts::medium);
    graph->text("MATHIEU DOMBROCK 2024-2025", Vec2i(20, 60), &Fonts::medium);
    graph->setColor(_colors["WHITE"]);
    int posX = 220 + sin((_g.getTick() / 16.0f)) * 10;
    graph->text("-- PRESS TO CONTINUE --", Vec2i(posX, 380), &Fonts::medium);
    sprBg.render(graph,
                 WINDOW_SIZE - Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.x / 2));
    sprTitle.render(graph, Vec2i((WINDOW_SIZE.x / 2) - (96 * 4), 110));
    sprSDL.render(graph, Vec2i(32, 550));
    sprCPP.render(graph, Vec2i(248, 550));
    graph->setColor(_colors["GRAY"]);
    std::string text = "ESO-OS version "+ (std::string)VERSION_STRING +" #" + std::to_string(BUILD_TIME);
    graph->text(text, pos + Vec2i(0, WINDOW_SIZE.y - 32), &Fonts::small);
  }
};
