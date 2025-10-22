/*
* The ScreenMainMenu class displays the main menu
*/
#pragma once
#include "Imp.h"
#include "GameMaster.h"
using namespace Imp;
#include "_colors.h"
#include "_fonts.h"
#include "_gameMaster.h"

#ifndef BUILD_TIME
#define BUILD_TIME 1234
#endif

class BtnScreenMainMenu : public Entity {
public:
  Vec2i size;
  std::string text;
  Uint8 state;
  bool center;
  BtnScreenMainMenu() : Entity() {
    tag = "btn";
    state = 0;
    center = false;
    text = "Button";
    size = Vec2i(200, g_gm.fontSize * 1.1f);
    setCollider(size);
  }
  ~BtnScreenMainMenu() {}
  bool isClicked() { return state == 2; }
  void process() override {}
  void render(Graphics *graph) override {
    Color *c = &g_colors["GRAY"];
    switch (state) {
    case 0:
      c = &g_colors["GRAY"];
      break;
    case 1:
      c = &g_colors["YELLOW"];
      break;
    case 2:
      c = &g_colors["GREEN"];
      break;
    }
    std::string textMod = (state > 0 ? ">> " : "") + text;
    graph->setColor(*c);
    if (center) {
      int textWidth = graph->textWidth(textMod, &Fonts::medium);
      Vec2i textPos =
          pos + Vec2i((size.x - textWidth) / 2, (size.y - g_gm.fontSize) / 2);
      graph->text(textMod, textPos, &Fonts::medium);
    } else {
      graph->text(textMod, pos, &Fonts::medium);
    }
  }
  void onMouse(bool over) override {
    if (GInput.mouseKeyOnce(SDL_BUTTON_LEFT) && over) {
      state = 2;
    } else if (over) {
      state = 1;
    } else
      state = 0;
  }
};

class ScreenMainMenu : public Entity {
public:
  EntityManager em;
  BtnScreenMainMenu btnResume;
  BtnScreenMainMenu btnSettings;
  BtnScreenMainMenu btnExit;
  Sprite sprBg;
  Sprite sprTitle;
  Font font = Font("HomeVideo.ttf", g_gm.fontSize);
  ScreenMainMenu() : Entity() {
    tag = "menu";

    btnResume.pos = Vec2i(20, 60);
    btnResume.text = "Solve-ware";
    em.addEntity(&btnResume);

    btnSettings.pos = Vec2i(20, 60 + g_gm.fontSize * 1.1f * 2);
    btnSettings.text = "Pref-ware";
    em.addEntity(&btnSettings);

    btnExit.pos = Vec2i(20, 60 + g_gm.fontSize * 1.1f * 3);
    btnExit.text = "Goodbye";
    em.addEntity(&btnExit);

    sprBg = Sprite(Vec2i(176, 176), Vec2i(96, 96),
                   Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.x / 2));

    sprTitle = Sprite(Vec2i(80, 80), Vec2i(96, 32),
                      Vec2i(96 * 4, 32 * 4));
  }
  ~ScreenMainMenu() {}
  void process() override {
    if (g_gm.getScreen() != SCN_MAIN_MENU) {
      return;
    }
    em.process();
    em.checkMouse();
    if (btnResume.isClicked()) {
      g_gm.setScreen(SCN_PUZZLE);
    }
    if (btnExit.isClicked()) {
      // _g.setQuit(true);
      g_gm.sendMessage("quit");
    }
    if (btnSettings.isClicked()) {
      g_gm.setScreen(SCN_SETTINGS);
    }
  }
  void render(Graphics *graph) override {
    if (g_gm.getScreen() != SCN_MAIN_MENU)
      return;
    graph->setColor(g_colors["WHITE"]);
    graph->text("ESO-OS", Vec2i(20, 20), &Fonts::medium);
    em.render(graph);
    sprBg.render(graph,
                 WINDOW_SIZE - Vec2i(WINDOW_SIZE.x / 2, WINDOW_SIZE.x / 2));
    sprTitle.render(graph, Vec2i((WINDOW_SIZE.x - 96 * 4) - 16, 16));
    graph->setColor(g_colors["WHITE"]);
    std::string text = "ESO-OS version "+ (std::string)VERSION_STRING +" #" + std::to_string(BUILD_TIME);
    graph->text(text, pos + Vec2i(0, WINDOW_SIZE.y - 32), &Fonts::small);
  }
};
