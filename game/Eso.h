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

#include "Sounds.h"
#include "_fonts.h"


class App : public Imp::Main { 
public:
    Grid grid;
    TestArea testArea;
    Cursor cursor;
    TopMenu TopMenu;
    BottomBar bottomBar;
    MainMenu mainMenu;
    SetupScreen setupScreen;
    HelpScreen helpScreen;
    App() : Imp::Main("EsoBinaria (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png") { 
        clearColor = Color(_colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&testArea);
        entityMan.addEntity(&TopMenu);
        entityMan.addEntity(&setupScreen);
        entityMan.addEntity(&helpScreen);
        entityMan.addEntity(&mainMenu);
        entityMan.addEntity(&cursor);

        _g.setScreen("helpScreen");

        Sounds::init();
        Fonts::init(_g.fontSize);
    }
    ~App() {}
    void process() override {
        _input.poll();
        // Pause
        if (_input.keyOnce(SDLK_ESCAPE)) {
            DBG("Hit ESC");
            _g.setScreen("mainMenu");
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