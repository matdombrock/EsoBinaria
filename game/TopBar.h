#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "BtnTopMenu.h"
#include "Modal.h"

class TopBar : public Entity {
public:
    BtnTopMenu btnHome;
    BtnTopMenu btnFile;
    BtnTopMenu btnTools;
    BtnTopMenu btnEdit;
    // File menu
    BtnTopMenu btnNew;
    BtnTopMenu btnReset;
    BtnTopMenu btnSave;
    BtnTopMenu btnLoad;
    // Tools menu
    BtnTopMenu btnTests;
    //
    EntityManager em;
    Modal modal;
    int height;
    std::string activeTopMenu;
    TopBar() : Entity() {
        tag = "topBar";
        height = _g.vu(0.5f);

        btnHome.isHomeBtn = true;
        btnHome.onClick = []() { _g.setScreen("mainMenu"); };
        btnHome.available = true;
        btnHome.pos = Vec2i(_g.vu(0.25f), 4);
        btnHome.setCollider(Vec2i(_g.vu(0.25f), height));
        em.addEntity(&btnHome);

        btnFile.onClick = [this]() {
            activeTopMenu = "btnFile";
            btnNew.available = true;
            btnReset.available = true;
            btnSave.available = true;
            btnLoad.available = true;
        };
        btnFile.tag = "btnFile";
        btnFile.fontSize = _g.fontSize * 0.75f;
        btnFile.available = true;
        btnFile.text = "FILE";
        btnFile.pos = Vec2i(_g.vu(0.15f) + _g.vu(0.75f), 4);
        em.addEntity(&btnFile);

        btnTools.onClick = [this]() {
            activeTopMenu = "btnTools";
            btnTests.available = true;
        };
        btnTools.tag = "btnTools";
        btnTools.fontSize = _g.fontSize * 0.75f;
        btnTools.available = true;
        btnTools.text = "TOOL";
        btnTools.pos = Vec2i(_g.vu(0.15f) + _g.vu(2.25f), 4);
        em.addEntity(&btnTools);

        btnEdit.tag = "btnEdit";
        btnEdit.fontSize = _g.fontSize * 0.75f;
        btnEdit.available = true;
        btnEdit.text = "EDIT";
        btnEdit.pos = Vec2i(_g.vu(0.15f) + _g.vu(3.75f), 4);
        em.addEntity(&btnEdit);

        // File menu
        btnNew.tag = "btnNew";
        btnNew.onClick = [this]() {
            modal.onOk = [&]() {
                _g.setScreen("puzzleSetup");
            };
            modal.title = "New Puzzle?";
            modal.show = true;
        };
        btnNew.available = false;
        btnNew.text = "NEW";
        btnNew.fontSize = _g.fontSize * 0.75f;
        btnNew.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(0.5f));
        em.addEntity(&btnNew);

        btnReset.onClick = [this]() { 
            modal.onOk = [&]() {
                _g.setReset(true); 
            };
            modal.title = "Reset?";
            modal.show = true;
        };
        btnReset.available = false;
        btnReset.text = "RESET";
        btnReset.fontSize = _g.fontSize * 0.75f;
        btnReset.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(1));
        em.addEntity(&btnReset);

        btnSave.onClick = [this]() { 
            modal.onOk = [&]() {
                DBG("Save");
            };
            modal.title = "Saved";
            modal.showCancel = false;
            modal.show = true; 
        };
        btnSave.available = false;
        btnSave.text = "SAVE";
        btnSave.fontSize = _g.fontSize * 0.75f;
        btnSave.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(1.5f));
        em.addEntity(&btnSave);

        btnLoad.onClick = [this]() { 
            modal.onOk = [&]() {
                _g.setReset(true); 
            };
            modal.title = "Load?";
            modal.show = true;
        };
        btnLoad.available = false;
        btnLoad.text = "LOAD";
        btnLoad.fontSize = _g.fontSize * 0.75f;
        btnLoad.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(2));
        em.addEntity(&btnLoad);

        // Tools menu
        btnTests.onClick = []() { DBG("TESTS"); _g.toggleTests(); };
        btnTests.available = false;
        btnTests.text = "TESTS";
        btnTests.fontSize = _g.fontSize * 0.75f;
        btnTests.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + _g.vu(0.5f));
        em.addEntity(&btnTests);

        em.addEntity(&modal);
    }
    ~TopBar() {}
    void process() override {
        if (_g.getScreen() != "puzzle") return;
        // Clear if nothing is clicked
        if (_input.mouseKeyOnce(SDL_BUTTON_LEFT)) {
            activeTopMenu = "";
        }
        em.checkMouse();
        em.process();
        if (activeTopMenu != "btnFile") {
            btnNew.available = false;
            btnReset.available = false;
            btnSave.available = false;
            btnLoad.available = false;
        }
        if (activeTopMenu != "btnTools") {
            btnTests.available = false;
        }
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != "puzzle") return;
        graph->setColor(_colors["BG3"]);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, height));
        
        graph->setColor(_colors["BG2"]);
        if (activeTopMenu == "btnFile") {
            graph->rect(Vec2i(btnFile.pos.x, height), Vec2i(129, 120));
        }
        if (activeTopMenu == "btnTools") {
            graph->rect(Vec2i(btnTools.pos.x, height), Vec2i(120, 120));
        }
        
        em.render(graph);
    }
};