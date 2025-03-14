#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class BtnMainMenu : public Entity {
public:
    Vec2i size;
    std::string text;
    Uint8 state;
    bool center;
    BtnMainMenu() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(200, _g.fontSize * 1.1f);
        setCollider(size);
    }
    ~BtnMainMenu() {}
    bool isClicked() {
        return state == 2;
    }
    void process() override {}
    void render(Graphics* graph) override {
        Color* c = &_colors["GRAY"];
        switch (state) {
            case 0: c = &_colors["GRAY"]; break;
            case 1: c = &_colors["YELLOW"]; break;
            case 2: c = &_colors["GREEN"]; break;
        }
        std::string textMod = (state > 0 ? ">> " : "") + text;
        graph->setColor(*c);
        if (center) {
            int textWidth = graph->textWidth(textMod);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - _g.fontSize) / 2);
            graph->text(textMod, textPos);
        }
        else {
            graph->text(textMod, pos);
        }
    }
    void onMouse(bool over) override {
        if (_input.mouseKeyOnce(SDL_BUTTON_LEFT) && over) {
            state = 2;
        }
        else if (over) {
            state = 1;
        }
        else state = 0;
    }
};

class MainMenu : public Entity {
public:
    EntityManager em;
    BtnMainMenu btnResume;
    BtnMainMenu btnRestart;
    BtnMainMenu btnNew;
    BtnMainMenu btnSettings;
    BtnMainMenu btnExit;
    Sprite sprBg;
    MainMenu() : Entity() {
        tag = "menu";

        btnResume.pos = Vec2i(20, 60);
        btnResume.text = "Resume";
        em.addEntity(&btnResume);

        btnRestart.pos = Vec2i(20, 60 + _g.fontSize * 1.1f);
        btnRestart.text = "Restart";
        em.addEntity(&btnRestart);

        btnNew.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 2);
        btnNew.text = "New";
        em.addEntity(&btnNew);

        btnSettings.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 3);
        btnSettings.text = "Settings";
        em.addEntity(&btnSettings);

        btnExit.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 4);
        btnExit.text = "Exit";
        em.addEntity(&btnExit);

        sprBg = Sprite(Vec2i(176, 176), Vec2i(96, 96), Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));\
    }
    ~MainMenu() {}
    void process() override {
        if (_g.getScreen() != "mainMenu") {
            Sounds::menuTrack.volDown(4, 8);
            // Sounds::menuTrack.stop();
            return;
        }
        // Sounds::menuTrack.volume = 255;
        if (!Sounds::menuTrack.isPlaying()) Sounds::menuTrack.volume = 0;
        Sounds::menuTrack.volUp(4, 128);
        Sounds::menuTrack.play(true);
        em.process();
        em.checkMouse();
        if (btnResume.isClicked()) {
            _g.setScreen("puzzle");
        }
        if (btnRestart.isClicked()) {
            _g.setReset(true);
            _g.setScreen("puzzle");
        }
        if (btnNew.isClicked()) {
            _g.setScreen("puzzleSetup");
        }
        if (btnExit.isClicked()) {
            _g.setQuit(true);
        }
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != "mainMenu") return;
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        graph->setColor(_colors["WHITE"]);
        graph->text("ESO-OS", Vec2i(20, 20));
        em.render(graph);
        sprBg.render(graph, WINDOW_SIZE - Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));
    }
};