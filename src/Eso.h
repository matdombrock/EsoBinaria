// EsoBinaria - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "Imp.h"
#include "BottomBar.h"
#include "Cursor.h"
#include "Grid.h"
#include "Huk.h"
#include "BG.h"
#include "FX.h"
#include "ScreenBumper.h"
#include "ScreenEmail.h"
#include "ScreenMainMenu.h"
#include "ScreenSettings.h"
#include "ScreenSetup.h"
#include "TestArea.h"
#include "TopBar.h"
#include "_fonts.h"
#include "_gameMaster.h"

using namespace Imp;

class App : public Imp::Main {
public:
  Grid grid;
  TestArea testArea;
  Cursor cursor;
  TopBar topBar;
  BottomBar bottomBar;
  Huk huk;
  BG bg;
  FX fx;
  ScreenBumper screenBumper;
  ScreenMainMenu screenMainMenu;
  ScreenSetup screenSetup;
  ScreenEmail screenEmail;
  ScreenSettings screenSettings;
  Sound mainMusic = Sound("main.ogg");
  int overlayColor[4] = {0, 0, 0, 0};
  // _g.store will not be initalized and will have no values on first run
  App() : Imp::Main("EsoBinaria", WINDOW_SIZE, WINDOW_FPS, g_gm.store.getInt("settings_window_scale"), "tiles.png") {

    g_gm.init();
    // Must reset after _g.init() so that puzzle is loaded
    testArea.reset();

    clearColor = Color(g_colors["BG"]);
    entityMan.addEntity(&bg);
    entityMan.addEntity(&grid);
    entityMan.addEntity(&bottomBar);
    entityMan.addEntity(&testArea);
    entityMan.addEntity(&topBar);
    entityMan.addEntity(&huk);
    entityMan.addEntity(&screenBumper);
    entityMan.addEntity(&screenSetup);
    entityMan.addEntity(&screenEmail);
    entityMan.addEntity(&screenMainMenu);
    entityMan.addEntity(&screenSettings);
    entityMan.addEntity(&cursor);
    entityMan.addEntity(&fx);

    g_gm.setScreen(SCN_BUMPER);

    Fonts::init(g_gm.fontSize);

    // Default Settings
    initDefaultSettings();

    recolor();
  }
  ~App() {}
  void initDefaultSettings() {
    g_gm.store.initBool("settings_enable_screen_fx", true);
    g_gm.store.initBool("settings_enable_color_overlay", true);
    g_gm.store.initBool("settings_enable_fps", false);
    g_gm.store.initBool("settings_enable_audio", true);
    g_gm.store.initBool("settings_enable_music", true);
    g_gm.store.initBool("settings_enable_auto_save", true);
    g_gm.store.initInt("settings_window_scale", 1);
    g_gm.store.initInt("settings_color_overlay_num", 0);
    // There are more store keys which are not initialized here
    // These are really just the settings
  }
  void recolor() {
    int num = g_gm.store.getInt("settings_color_overlay_num");
    int overlays[8][4] = {{96, 255, 128, 32}, {255, 128, 96, 32},
                          {128, 96, 255, 32}, {255, 255, 255, 32},
                          {128, 255, 96, 32}, {96, 128, 255, 32},
                          {255, 96, 128, 32}, {128, 128, 128, 32}};
    overlayColor[0] = overlays[num][0];
    overlayColor[1] = overlays[num][1];
    overlayColor[2] = overlays[num][2];
    overlayColor[3] = overlays[num][3];
  }
  void render(Graphics *graph) override {
    // Color overlay
    if (g_gm.store.getBool("settings_enable_color_overlay")) {
      graph->setColor(overlayColor[0], overlayColor[1], overlayColor[2],
                      overlayColor[3]);
      graph->rect(Vec2i(0, 0), WINDOW_SIZE, true);
    }
    // FPS meter
    if (g_gm.store.getBool("settings_enable_fps")) {
      graph->setColor(0, 255, 0, 255);
      graph->text(std::to_string(realFPS), Vec2i(0, 0), &Fonts::small);
    }
  }
  void process() override {
    // Handle messages
    std::vector<std::string> msgs = g_gm.getMessages();
    g_gm.clearMessages();
    for (int i = 0; i < msgs.size(); i++) {
      std::string msg = msgs[i];
      if (msg == "mv_down") {
        grid.translate(Vec2i(0, 1));
      }
      if (msg == "mv_up") {
        grid.translate(Vec2i(0, -1));
      }
      if (msg == "mv_left") {
        grid.translate(Vec2i(-1, 0));
      }
      if (msg == "mv_right") {
        grid.translate(Vec2i(1, 0));
      }
      if (msg == "save") {
        auto data = grid.cellsToString();
        DBG("save");
        DBG(data);
        g_gm.store.setString("save_" + g_gm.getPuzzleString(), data);
      }
      if (msg == "load") {
        if (g_gm.store.hasKey("save_" + g_gm.getPuzzleString())) {
          auto data = g_gm.store.getString("save_" + g_gm.getPuzzleString());
          DBG("load");
          DBG(data);
          grid.setCellsFromString(data);
        } else {
          DBG("No save data found");
        }
      }
      if (msg == "reset") {
        grid.reset();
        testArea.reset();
        huk.reset();
      }
      if (msg == "recolor") {
        recolor();
      }
      if (msg == "resize") {
        int scale = g_gm.store.getInt("settings_window_scale");
        scale = scale + 1;
        if (scale > 2)
          scale = 1;
        g_gm.store.setInt("settings_window_scale", scale);
        init("tiles.png", scale);
      }
      if (msg == "init_default_settings") {
        initDefaultSettings();
      }
      if (msg == "quit") {
        exit(0);
      }
    }

    g_gm.incTick();

    mainMusic.play(0, true);

    // Enable / disable audio
    if (!g_gm.store.getBool("settings_enable_audio")) {
#if defined(__APPLE__)
      Mix_MasterVolume(0); // breaks linux
#endif
    } else {
#if defined(__APPLE__)
      Mix_MasterVolume(128); // breaks linux
#endif
    }
    // Enable music
    if (!g_gm.store.getBool("settings_enable_music")) {
      mainMusic.mute();
    } else {
      mainMusic.unmute();
    }

    // FPS meter
    if (g_gm.getTick() % 60 == 0) {
      realFPS = getRealFPS();
    }
  }
  void onLostFocus() override { pauseRenderer = true; }
  void onGainedFocus() override { pauseRenderer = false; }

private:
  int realFPS = 0;
};
