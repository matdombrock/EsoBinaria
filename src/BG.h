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

class BG : public Entity {
public:
  BG() : Entity() { tag = "bumper"; }
  ~BG() {}
  void process() override {
    posA.x = (sin(_g.getTick() / 512.0f) + 1.0f) * (WINDOW_SIZE.x * 4);
    posA.y = (cos(_g.getTick() / 512.0f) + 1.0f) * (WINDOW_SIZE.y * 4);
    posB.x = WINDOW_SIZE.x - posA.x;
    posB.y = posA.y;
    posC.x = posA.x;
    posC.y = WINDOW_SIZE.y - posA.y;
    posD.x = WINDOW_SIZE.x - posA.x;
    posD.y = WINDOW_SIZE.y - posA.y;
    // alpha = 8 + (sin(_g.getTick() / 128.0f) + 1.0f) * 8;
    alpha = 8;
  }
  void render(Graphics *graph) override {
    graph->setColor(_colors["BG"]);
    graph->rect(Vec2i(0, 0), WINDOW_SIZE);
    //
    int lineWidth = 512;
    graph->setColor(_colors["RED"], alpha);
    graph->line(Vec2i(0, 0), posA, lineWidth);
    graph->setColor(_colors["BLUE"], alpha);
    graph->line(Vec2i(WINDOW_SIZE.x, 0), posB, lineWidth);
    graph->setColor(_colors["GREEN"], alpha);
    graph->line(Vec2i(0, WINDOW_SIZE.y), posC, lineWidth);
    graph->setColor(_colors["YELLOW"], alpha);
    graph->line(Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y), posD, lineWidth);
    //
  }

private:
  int alpha = 64;
  Vec2i posA = Vec2i(0, 0);
  Vec2i posB = Vec2i(0, 0);
  Vec2i posC = Vec2i(0, 0);
  Vec2i posD = Vec2i(0, 0);
};
