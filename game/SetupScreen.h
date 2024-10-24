#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "_colors.h"
#include "_gameMaster.h"

class BtnNub : public Btn {
public:
    int index;
    BtnNub() : Btn() {
        tag = "nub";
        index = 0;
        size = Vec2i(_g.vu(0.25f), _g.vu(0.25f));
        setCollider(Vec2i(_g.cellSize / 2, _g.cellSize / 2));
    }
    ~BtnNub() {}
    void render(Graphics* graph) override {
        if (!available) return;
        graph->setColor(state ? _colors["GREEN"] : _colors["GRAY"]);
        if (_g.getPuzzleNum() == index) {
            graph->setColor(_colors["YELLOW"]);
        }
        // graph->rect(pos, size, true);
        graph->tri(pos, pos + Vec2i(size.x, 0), pos + Vec2i(0, size.y), _g.getTick() * (index/256.0f) / 512.0f);
        // graph->setColor(_colors["BG"]);
        graph->tri(pos + Vec2i(size.x, size.y), pos + Vec2i(size.x, 0), pos + Vec2i(0, size.y), (_g.getTick() * (index / 256.0f) / 512.0f));
    }
};

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
            btnsLvl[i].pos = Vec2i(_g.vu(2), _g.vu(3)) + Vec2i(x,y);
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
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        
        graph->setColor(_colors["YELLOW"],180);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, _g.fontSize * 4));
        // Draw tri in top left
        graph->setColor(_colors["BG"]);
        graph->tri(Vec2i(0, 0), Vec2i(_g.vu(2), 0), Vec2i(0, _g.vu(2)));
        
        graph->setColor(_colors["WHITE"]);
        graph->setFont("HomeVideo.ttf", _g.fontSize * 4);
        graph->text("   EsoBinaria", Vec2i(_g.vu(0), _g.vu(0)));
        graph->setColor(_colors["WHITE"]);
        graph->setFont("HomeVideo.ttf", _g.fontSize);
        graph->text(_g.getPuzzleString(), Vec2i(_g.vu(2), _g.vu(2)));

        // Draw cursor lines
        // graph->setColor(_colors["GREEN"], 128);
        // graph->line(_input.mousePos(), Vec2i(0,0));
        // graph->line(_input.mousePos(), Vec2i(WINDOW_SIZE.x,0));
        // graph->line(_input.mousePos(), Vec2i(WINDOW_SIZE.x,WINDOW_SIZE.y));
        // graph->line(_input.mousePos(), Vec2i(0,WINDOW_SIZE.y));

        if (_g.getPuzzleChallenge() == 'e') {
            graph->setColor(_colors["GREEN"], 64);
            graph->rect(btnEasy.pos, Vec2i(_g.vu(10), _g.vu(10)));
        }
        if (_g.getPuzzleChallenge() == 'm') {
            graph->setColor(_colors["YELLOW"], 128);
            graph->rect(btnMedium.pos, Vec2i(_g.vu(10), _g.vu(10)));
        }
        if (_g.getPuzzleChallenge() == 'h') {
            graph->setColor(_colors["RED"], 128);
            graph->rect(btnHard.pos, Vec2i(_g.vu(10), _g.vu(10)));
        }
        
        // Draw behind start
        graph->setColor(_colors["RED"], 200);
        graph->rect(btnStart.pos, Vec2i(_g.vu(10), _g.vu(10)));

        // Draw behind levels
        graph->setColor(_colors["BG2"]);
        graph->rect(Vec2i(_g.vu(1.8f), _g.vu(2.8f)), Vec2i(16 * _g.vu(0.5f) + _g.vu(0.2f), 16 * _g.vu(0.5f) + _g.vu(0.2f)));

        em.render(graph);

        // Draw triangle in bottom right corner
        graph->setColor(_colors["BG"]);
        graph->tri(Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y), Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y - _g.vu(6)), Vec2i(WINDOW_SIZE.x - _g.vu(6), WINDOW_SIZE.y));

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