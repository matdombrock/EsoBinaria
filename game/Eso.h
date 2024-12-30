// EsoBinaria - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "../src/Imp.h"
using namespace Imp;
#include "Grid.h"
#include "TestArea.h"
#include "Cursor.h"
#include "TopMenu.h"
#include "BottomBar.h"
#include "MainMenu.h"
#include "SetupScreen.h"
#include "EmailScreen.h"
#include "Huk.h"
#include "_gameMaster.h"
#include "_sounds.h"
#include "_fonts.h"


class App : public Imp::Main { 
public:
    Grid grid;
    TestArea testArea;
    Cursor cursor;
    TopMenu topMenu;
    BottomBar bottomBar;
    Huk huk;
    MainMenu mainMenu;
    SetupScreen setupScreen;
    EmailScreen emailScreen;
    Sound mainMusic = Sound("main.mp3");
    App() : Imp::Main("EsoBinaria (v0.1-alpha)", WINDOW_SIZE, 30, "tiles.png") { 
        clearColor = Color(_colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&testArea);
        entityMan.addEntity(&topMenu);
        entityMan.addEntity(&huk);
        entityMan.addEntity(&setupScreen);
        entityMan.addEntity(&emailScreen);
        entityMan.addEntity(&mainMenu);
        entityMan.addEntity(&cursor);

        if (_g.store.getBool("email_intro")) {
            _g.setScreen(SCN_PUZZLE_SETUP);
        } else {
            _g.setScreen(SCN_HELP);
        }

        Sounds::init();
        Fonts::init(_g.fontSize);

        _g.store.setBool("email_intro", true);
        // Set some DBG flags
        _g.store.setBool("unlocked_medium", true);
        _g.store.setBool("unlocked_hard", true);
        _g.store.setBool("settings_scanlines_disable", false);
        // _g.store.setBool("completed_lvl_3.e0", true);
        // _g.store.setBool("completed_lvl_3.e1", true);
    }
    ~App() {}
    void render(Graphics* g) override {
        if (_g.store.getBool("settings_scanlines_disable")) return;
        float amt = std::sin(_g.getTick() / 128.0f) * 0.5f + 0.5f;
        amt *= 0.75f;
        // g->fxApply(FX_SCANLINES2, _g.getTick(), amt);
        g->setColor(128, 96, 255, 32);
        g->rect(Vec2i(0, 0), WINDOW_SIZE);
        // FPS meter
        g->setColor(0, 255, 0, 255);
        g->text(std::to_string(realFPS), Vec2i(0, 0), &Fonts::small);
    }
    void process() override {
        if (_g.getReset()) {
            _g.setReset(false);
            grid.reset();
            testArea.reset();
            huk.reset();
        }
        if (_g.getQuit()) {
            exit(0);
        }

        _g.incTick();

        mainMusic.play(0, true);     
        // FPS meter
        if (_g.getTick() % 60 == 0) {
            realFPS = getRealFPS();
        }   
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
private:
    int realFPS = 0;
};