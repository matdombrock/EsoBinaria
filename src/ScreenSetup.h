/*
* The ScreenSetup class provides a level selection screen for the EsoBinaria game.
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "g_colors.h"
#include "g_fonts.h"
#include "g_gameMaster.h"

class BtnNub : public Btn {
public:
  int index = 0;
  bool completed = false;
  BtnNub() : Btn() {
    tag = "nub";
    size = Vec2i(g_gm.vu(0.25f), g_gm.vu(0.25f));
    setCollider(Vec2i(g_gm.cellSize / 2, g_gm.cellSize / 2));
  }
  ~BtnNub() {}
  void render(Graphics *graph) override {
    if (!available)
      return;
    graph->setColor(state       ? g_colors["YELLOW"]
                    : completed ? g_colors["GREEN"]
                                : g_colors["GRAY"]);
    if (g_gm.getPuzzleNum() == index) {
      graph->setColor(g_colors["RED"]);
    }
    // graph->rect(pos, size, true);
    graph->tri(pos, pos + Vec2i(size.x, 0), pos + Vec2i(0, size.y),
               g_gm.getTick() * (index / 256.0f) / 512.0f);
    // graph->setColor(_colors["BG"]);
    graph->tri(pos + Vec2i(size.x, size.y), pos + Vec2i(size.x, 0),
               pos + Vec2i(0, size.y),
               (g_gm.getTick() * (index / 256.0f) / 512.0f));
  }
};

class ScreenSetup : public Entity {
public:
  EntityManager em;
  BtnNub btnsLvl[256];
  BtnText btnEasy;
  BtnText btnMedium;
  BtnText btnHard;
  BtnText btnStart;
  Font font = Font("HomeVideo.ttf", g_gm.fontSize);
  Font fontLrg = Font("HomeVideo.ttf", g_gm.fontSize * 4);
  ScreenSetup() : Entity() {
    tag = "ScreenSetup";
    for (int i = 0; i < 256; i++) {
      int x = i % 16;
      int y = i / 16;
      x *= g_gm.vu(0.5f);
      y *= g_gm.vu(0.5f);
      btnsLvl[i].pos = Vec2i(g_gm.vu(2), g_gm.vu(3)) + Vec2i(x, y);
      btnsLvl[i].available = false;
      btnsLvl[i].onClick = [i]() {
        DBG("Clicked: " + std::to_string(i));
        g_gm.setPuzzleNum(i);
      };
      btnsLvl[i].index = i;
      em.addEntity(&btnsLvl[i]);
    }

    btnEasy.pos = Vec2i(g_gm.vu(10), g_gm.vu(2));
    btnEasy.text = "Erinyes";
    btnEasy.onClick = []() { g_gm.setPuzzleChallenge('e'); };
    em.addEntity(&btnEasy);

    btnMedium.pos = Vec2i(g_gm.vu(10), g_gm.vu(2) + g_gm.vu(1));
    btnMedium.text = "Manticore";
    btnMedium.onClick = []() { g_gm.setPuzzleChallenge('m'); };
    em.addEntity(&btnMedium);

    btnHard.pos = Vec2i(g_gm.vu(10), g_gm.vu(2) + g_gm.vu(2));
    btnHard.text = "Hydra";
    btnHard.onClick = []() { g_gm.setPuzzleChallenge('h'); };
    em.addEntity(&btnHard);

    btnStart.pos = Vec2i(g_gm.vu(10), g_gm.vu(2) + g_gm.vu(3));
    btnStart.text = "Begin >>";
    btnStart.onClick = []() {
      // _g.setReset(true);
      DBG("-------STARTING NEW LEVEL");
      g_gm.setScreen(SCN_PUZZLE);
      g_gm.sendMessage("reset");
    };
    em.addEntity(&btnStart);
  }
  ~ScreenSetup() {}
  void process() override {
    if (g_gm.getScreen() != SCN_PUZZLE_SETUP)
      return;
    char challenge = g_gm.getPuzzleChallenge();
    int maxLevel = challenge == 'e'                 ? 4
                   : g_gm.getPuzzleChallenge() == 'm' ? 64
                                                    : 256;
    for (int i = 0; i < 256; i++) {
      btnsLvl[i].available = true;
      if (btnsLvl[i].index >= maxLevel) {
        btnsLvl[i].available = false;
        continue;
      }
      std::string puzzleStr =
          "completed_lvl_" + g_gm.getPuzzleStringDynamic(3, challenge, i);
      btnsLvl[i].completed = g_gm.store.getBool(puzzleStr);
    }
    if (g_gm.getPuzzleNum() >= maxLevel) {
      g_gm.setPuzzleNum(maxLevel - 1);
    }
    // if (_g.store.getBool("unlocked_medium")) {
    //   btnMedium.available = true;
    // } else {
    //   btnMedium.available = false;
    // }
    // if (_g.store.getBool("unlocked_hard")) {
    //   btnHard.available = true;
    // } else {
    //   btnHard.available = false;
    // }
    em.process();
    em.checkMouse();
  }
  void render(Graphics *graph) override {
    if (g_gm.getScreen() != SCN_PUZZLE_SETUP)
      return;

    graph->setColor(g_colors["BG2"], 180);
    graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, g_gm.fontSize * 4));
    // Draw tri in top left
    graph->setColor(g_colors["BG"]);
    graph->tri(Vec2i(0, 0), Vec2i(g_gm.vu(2), 0), Vec2i(0, g_gm.vu(2)));

    graph->setColor(g_colors["WHITE"]);
    graph->text("   EsoBinaria", Vec2i(g_gm.vu(0), g_gm.vu(0)), &Fonts::large);
    graph->setColor(g_colors["WHITE"]);
    graph->text(g_gm.getPuzzleString() +
                    (g_gm.store.getBool("completed_lvl_" + g_gm.getPuzzleString())
                         ? " - KNOWN"
                         : " - ENIGMA"),
                Vec2i(g_gm.vu(2), g_gm.vu(2)), &Fonts::medium);

    // Draw cursor lines
    // graph->setColor(_colors["GREEN"], 128);
    // graph->line(GInput.mousePos(), Vec2i(0,0));
    // graph->line(GInput.mousePos(), Vec2i(WINDOW_SIZE.x,0));
    // graph->line(GInput.mousePos(), Vec2i(WINDOW_SIZE.x,WINDOW_SIZE.y));
    // graph->line(GInput.mousePos(), Vec2i(0,WINDOW_SIZE.y));

    if (g_gm.getPuzzleChallenge() == 'e') {
      graph->setColor(g_colors["GREEN"], 64);
      graph->rect(btnEasy.pos, Vec2i(g_gm.vu(10), g_gm.vu(10)));
    }
    if (g_gm.getPuzzleChallenge() == 'm') {
      graph->setColor(g_colors["YELLOW"], 128);
      graph->rect(btnMedium.pos, Vec2i(g_gm.vu(10), g_gm.vu(10)));
    }
    if (g_gm.getPuzzleChallenge() == 'h') {
      graph->setColor(g_colors["RED"], 128);
      graph->rect(btnHard.pos, Vec2i(g_gm.vu(10), g_gm.vu(10)));
    }

    // Draw behind start
    graph->setColor(g_colors["RED"], 200);
    graph->rect(btnStart.pos, Vec2i(g_gm.vu(10), g_gm.vu(10)));

    // Draw behind levels
    graph->setColor(g_colors["BG2"]);
    graph->rect(
        Vec2i(g_gm.vu(1.8f), g_gm.vu(2.8f)),
        Vec2i(16 * g_gm.vu(0.5f) + g_gm.vu(0.2f), 16 * g_gm.vu(0.5f) + g_gm.vu(0.2f)));

    em.render(graph);

    // Draw triangle in bottom right corner
    graph->setColor(g_colors["BG"]);
    graph->tri(Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y),
               Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y - g_gm.vu(6)),
               Vec2i(WINDOW_SIZE.x - g_gm.vu(6), WINDOW_SIZE.y));
  }
};
