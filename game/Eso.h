// EsoBinaria - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "../src/Imp.h"
using namespace Imp;
#include "Grid.h"
#include "TestArea.h"
#include "Cursor.h"
#include "TopBar.h"
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
    TopBar TopBar;
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
        entityMan.addEntity(&TopBar);
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

        if (_g.store.hasKey("email_intro")) _g.store.setBool("email_intro", true);
        // Default Settings
        if (!_g.store.hasKey("settings_enable_scanlines")) 
            _g.store.setBool("settings_enable_scanlines", false);
        if (!_g.store.hasKey("settings_enable_color_overlay")) 
            _g.store.setBool("settings_enable_color_overlay", true);   
        if (!_g.store.hasKey("settings_enable_fps")) 
            _g.store.setBool("settings_enable_fps", true);
        if (!_g.store.hasKey("settings_enable_fps")) 
            _g.store.setBool("settings_enable_fps", true);
        if (!_g.store.hasKey("settings_enable_audio")) 
            _g.store.setBool("settings_enable_audio", false);
        // Set some DBG flags
        _g.store.setBool("unlocked_medium", true);
        _g.store.setBool("unlocked_hard", true);
        // _g.store.setBool("completed_lvl_3.e0", true);
        // _g.store.setBool("completed_lvl_3.e1", true);
    }
    ~App() {}
    void render(Graphics* g) override {
        if (_g.store.getBool("settings_enable_scanlines")) {
            float amt = std::sin(_g.getTick() / 128.0f) * 0.5f + 0.5f;
            amt *= 0.75f;
            g->fxApply(FX_SCANLINES2, _g.getTick(), amt);
        }
        // overlay
        if (_g.store.getBool("settings_enable_color_overlay")) {
            g->setColor(128, 96, 255, 32);
            g->rect(Vec2i(0, 0), WINDOW_SIZE);
        }
        // FPS meter
        if (_g.store.getBool("settings_enable_fps")) {
            g->setColor(0, 255, 0, 255);
            g->text(std::to_string(realFPS), Vec2i(0, 0), &Fonts::small);
        }
    }
    void process() override {
        // Handle messages
        std::vector<std::string> msgs = _g.getMessages();
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
        }
        _g.clearMessages();

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