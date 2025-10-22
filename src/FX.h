/*
* Screen FX Entity
*/
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

class FX : public Entity {
public:
  FX() : Entity() { tag = "bumper"; }
  ~FX() {}
  void process() override {
    if (!_g.store.getBool("settings_enable_screen_fx")) {
      return;
    }
    // posY = sin(_g.getTick() * 0.001f) * 8;
    posY = (_g.getTick() / (WINDOW_FPS / 10)) % scanSize;
  }
  void render(Graphics *graph) override {
    if (!_g.store.getBool("settings_enable_screen_fx")) {
      return;
    }
    graph->setColor(255, 255, 255, 16);
    for (int i = 0; i <= WINDOW_SIZE.y / scanSize; i++) {
      int offset = (i * scanSize);
      int y = posY + offset;
      graph->line(Vec2(0, y), Vec2(WINDOW_SIZE.x, y), 2);
    }
    // graph->line(Vec2(0,posY), Vec2(WINDOW_SIZE.x, posY), 2);
  }

private:
  int scanSize = 16;
  int posY = 0;
};
