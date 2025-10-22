/*
* Button tile for selecting cell types in the game UI
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "Cell.h"
#include "CellSprites.h"
#include "_colors.h"
#include "_gameMaster.h"

class BtnTile : public Entity {
public:
  CellType type;
  Uint8 state;
  Uint8 oldState;
  Sound sndTick;
  std::function<void()> onClick;
  std::function<void()> onHover;
  BtnTile() : Entity() {
    state = 99; // 99 prevents play on start
    oldState = 99;
    tag = "tileBtn";
    type = CT_CLEAR;
    setCollider(Vec2i(g_gm.cellSize, g_gm.cellSize));
    sndTick.set("tick.wav");
  }
  ~BtnTile() {}
  void render(Graphics *graph) override {
    // Ignore input D and C if puzzle bits are less than 4
    if (type == CT_IND && g_gm.getPuzzleBits() < 4) {
      return;
    }
    Vec2i post = pos.copy();

    if (g_gm.getActiveTile() == type) { // is active
      post += Vec2i(0, -g_gm.cellSize / 4);
      // Draw line to mouse
      graph->setColor(g_colors["YELLOW"], 64);
      graph->line(post + Vec2i(g_gm.vu(0.5f), g_gm.vu(0.5f)),
                  GInput.mousePos() - Vec2i(0, g_gm.vu(0.5f)), 3);
      graph->line(post + Vec2i(g_gm.vu(0.5f), g_gm.vu(0.5f)),
                  GInput.mousePos() - Vec2i(g_gm.vu(0.5f), -g_gm.vu(0.25f)), 6);
      graph->line(post + Vec2i(g_gm.vu(0.5f), g_gm.vu(0.5f)),
                  GInput.mousePos() + Vec2i(g_gm.vu(0.5f), g_gm.vu(0.25f)), 3);
    } else if (state > 0) {
      post += Vec2i(0, -g_gm.cellSize / 8);
    }
    graph->setColor(g_colors["GREEN"]);
    graph->rect(
        pos + Vec2i(g_gm.cellSize / 4, g_gm.cellSize / 8),
        Vec2i(g_gm.cellSize - (g_gm.cellSize / 2), g_gm.cellSize - g_gm.cellSize / 4));
    CellSprites::baseTile.render(graph, post);
    CellSprites::cellMap[type]->render(graph, post);
  }
  void onMouse(bool over) override {
    oldState = state;
    if (over) {
      state = 1;
      if (GInput.mouseKeyOnce(SDL_BUTTON_LEFT)) {
        state = 2;
        g_gm.setActiveTile(type);
      }
      if (state > 0) {
        if (onHover != nullptr)
          onHover();
      }
      if (state == 1 && oldState == 0) {
        sndTick.play();
      }
      return;
    }
    state = 0;
  }
};
