#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "Cell.h"
#include "CellSprites.h"
#include "_colors.h"
#include "_gameMaster.h"

class BtnTile : public Entity {
public:
    CellType type;
    Uint8 state;
    Uint8 oldState;
    Sound sndTick;
    std::function<void()> onClick;
    std::function<void()> onHover;
    BtnTile() : Entity() {
        state = 99; // 99 prevents play on start
        oldState = 99;
        tag = "tileBtn";
        type = CT_CLEAR;
        setCollider(Vec2i(_g.cellSize, _g.cellSize));
        sndTick.set("1.wav");
    }
    ~BtnTile() {}
    void render(Graphics* graph) override {
            if (type == CT_IND && _g.getPuzzleBits() < 4) {
                return;
            }
            Vec2i post = pos.copy();
            
            if (_g.getActiveTile() == type) {
                post += Vec2i(0, -_g.cellSize / 4);
            }
            else if (state > 0) {
            post += Vec2i(0, -_g.cellSize / 8);
        }
        graph->setColor(_colors["GREEN"]);
        graph->rect(pos + Vec2i(_g.cellSize / 4, _g.cellSize / 8), Vec2i(_g.cellSize - (_g.cellSize / 2), _g.cellSize - _g.cellSize / 4));
        CellSprites::baseTile.render(graph, post);
        CellSprites::cellMap[type]->render(graph, post);
    }
    void onMouse(bool over) override {
        oldState = state;
        if (over) {
            state = 1;
            if (_input.mouseKeyOnce(SDL_BUTTON_LEFT)) {
                state = 2;
                _g.setActiveTile(type);
            }
            if (state > 0) {
                if (onHover != nullptr) onHover();
            }
            if (state == 1 && oldState == 0) {
                DBG("play ");
                sndTick.play();
            }
            return;
        }
        state = 0;
    }
};