#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "BtnMainMenu.h"


class MainMenu : public Entity {
public:
    EntityManager em;
    BtnMainMenu btnResume;
    BtnMainMenu btnRestart;
    BtnMainMenu btnNew;
    BtnMainMenu btnSettings;
    BtnMainMenu btnExit;
    Sprite sprWiz;
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

        sprWiz = Sprite(Vec2i(0, 224), Vec2i(32,32), Vec2i(256, 256));
        // sprWiz.setAnimation({Vec2i(0, 224), Vec2i(32, 224)}, 4);
        
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
            int puzMax = Util::maxUnsignedInt(Util::maxUnsignedInt(_g.getPuzzleBits())); 
            DBG("Puzzle max: " + std::to_string(puzMax));
            _g.setPuzzleNum(rand() % puzMax); 
            DBG("New puzzle: " + std::to_string(_g.getPuzzleNum()));
            _g.setReset(true);
            _g.setScreen("puzzle");
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
        graph->text("ESOMachina", Vec2i(20, 20), _g.fontSize);
        em.render(graph);
        sprBg.render(graph, WINDOW_SIZE - Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));
        sprWiz.render(graph, Vec2i(0, WINDOW_SIZE.y - 256));
    }
};