#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "BtnNub.h"
#include "_colors.h"
#include "_gameMaster.h"

class SetupScreen : public Entity {
public:
    EntityManager em;
    BtnNub btnsLvl[256];
    BtnText btnEasy;
    BtnText btnMedium;
    BtnText btnHard;
    BtnText btnStart;
    SetupScreen() : Entity() {
        tag = "setupScreen";
        for (int i = 0; i < 256; i++) {
            int x = i % 16;
            int y = i / 16;
            x *= _g.vu(0.5f);
            y *= _g.vu(0.5f);
            btnsLvl[i].pos = Vec2i(_g.vu(2), _g.vu(2)) + Vec2i(x,y);
            btnsLvl[i].onClick = [i](){
                DBG("Clicked: " + std::to_string(i));
                _g.setPuzzleNum(i);
            };
            btnsLvl[i].index = i;
            em.addEntity(&btnsLvl[i]);
        }

        btnEasy.pos = Vec2i(_g.vu(10), _g.vu(2));
        btnEasy.text = "IMP";
        btnEasy.onClick = [](){
            _g.setPuzzleChallenge('e');
        };
        em.addEntity(&btnEasy);

        btnMedium.pos = Vec2i(_g.vu(10), _g.vu(2) + _g.vu(1));
        btnMedium.text = "MAGE";
        btnMedium.onClick = [](){
            _g.setPuzzleChallenge('m');
        };
        em.addEntity(&btnMedium);

        btnHard.pos = Vec2i(_g.vu(10), _g.vu(2) + _g.vu(2));
        btnHard.text = "ARCH";
        btnHard.onClick = [](){
            _g.setPuzzleChallenge('h');
        };
        em.addEntity(&btnHard);

        btnStart.pos = Vec2i(_g.vu(10), _g.vu(2) + _g.vu(3));
        btnStart.text = "START";
        btnStart.onClick = [](){
            _g.setReset(true);
            _g.setScreen("puzzle");
        };
        em.addEntity(&btnStart);
    }
    ~SetupScreen() {}
    void process() override {
        if (_g.getScreen() != "puzzleSetup") return;
        int maxLevel = _g.getPuzzleChallenge() == 'e' ? 4 : _g.getPuzzleChallenge() == 'm' ? 64 : 256;
        for (int i = 0; i < 256; i++) {
            btnsLvl[i].available = true;
            if (btnsLvl[i].index >= maxLevel) {
                btnsLvl[i].available = false;
            }
        }
        if (_g.getPuzzleNum() >= maxLevel) {
            _g.setPuzzleNum(maxLevel - 1);
        }
        em.process();
        em.checkMouse();
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != "puzzleSetup") return;
        graph->setColor(_colors["BG3"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        graph->setColor(_colors["WHITE"]);
        graph->text(_g.getPuzzleString(), Vec2i(_g.vu(2), _g.vu(1)), _g.fontSize);

        // Draw cursor lines
        // graph->setColor(_colors["GREEN"], 128);
        // graph->line(_input.mousePos(), Vec2i(0,0));
        // graph->line(_input.mousePos(), Vec2i(WINDOW_SIZE.x,0));
        // graph->line(_input.mousePos(), Vec2i(WINDOW_SIZE.x,WINDOW_SIZE.y));
        // graph->line(_input.mousePos(), Vec2i(0,WINDOW_SIZE.y));

        graph->setColor(_colors["YELLOW"], 128);
        if (_g.getPuzzleChallenge() == 'e') {
            graph->rect(btnEasy.pos, btnEasy.size);
        }
        if (_g.getPuzzleChallenge() == 'm') {
            graph->rect(btnMedium.pos, btnMedium.size);
        }
        if (_g.getPuzzleChallenge() == 'h') {
            graph->rect(btnHard.pos, btnHard.size);
        }
        em.render(graph);

        // Draw static noise
        if (_g.getTick() % 2 == 0) {
            int staticSize = _g.cellSize / 32;
            graph->setColor(_colors["YELLOW"],128);
            for (int i = 0; i < WINDOW_SIZE.size2d()/(staticSize * 1024); i++) {
                int x = rand() % WINDOW_SIZE.x;
                int y = rand() % WINDOW_SIZE.y;
                graph->rect(Vec2i(x, y), Vec2i(staticSize, staticSize));
            }
        }
    }
};