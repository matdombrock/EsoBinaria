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
#include "HelpScreen.h"
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
    MainMenu mainMenu;
    SetupScreen setupScreen;
    HelpScreen helpScreen;
    App() : Imp::Main("EsoBinaria (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png") { 
        clearColor = Color(_colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&testArea);
        entityMan.addEntity(&topMenu);
        entityMan.addEntity(&setupScreen);
        entityMan.addEntity(&helpScreen);
        entityMan.addEntity(&mainMenu);
        entityMan.addEntity(&cursor);

        if (_g.store.getBool("firstStart")) {
            _g.setScreen(SCN_PUZZLE_SETUP);
        } else {
            _g.setScreen(SCN_HELP);
        }

        Sounds::init();
        Fonts::init(_g.fontSize);

        _g.store.setBool("firstStart", true);
    }
    ~App() {}
    void render(Graphics* g) override {
        // g->fxApply(FX_SCANLINES2, _g.getTick());
    }
    void process() override {
        // Pause
        if (_input.keyOnce(SDLK_ESCAPE) || _input.keyOnce(SDLK_BACKQUOTE)) {
            DBG("Hit ESC");
            _g.setScreen(SCN_PUZZLE_SETUP);
        }

        if (_g.getReset()) {
            _g.setReset(false);
            grid.reset();
            testArea.reset();
        }
        if (_g.getQuit()) {
            exit(0);
        }

        _g.incTick();
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
};