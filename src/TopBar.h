/*
* The TopBar class implements a top navigation bar with buttons for file, edit, and tools menus.
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "Modal.h"
#include "_colors.h"
#include "_gameMaster.h"
#include "_fonts.h"

class BtnTopBar : public BtnText {
public:
    bool isHomeBtn = false;
    BtnTopBar() : BtnText() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(80, g_gm.vu(0.45f));
        setCollider(size);
    }
    ~BtnTopBar() {}
    void render(Graphics* graph) override {
        if (!available) return;
        Color* c = &g_colors["GRAY"];
        switch (state) {
            case 0: c = &g_colors["GRAY"]; break;
            case 1: c = &g_colors["YELLOW"]; break;
            case 2: c = &g_colors["GREEN"]; break;
        }
        graph->setColor(*c);
        if (isHomeBtn) {
            int sz = g_gm.vu(0.25f);
            // draw a diamond with triangles
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(sz, sz/2), pos + Vec2i(sz/2, sz));
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(0, sz/2), pos + Vec2i(sz/2, sz));

            return;
        }
        if (center) {
            int textWidth = graph->textWidth(text, &Fonts::small);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - g_gm.vu(0.5f)) / 2);
            graph->text(text, textPos, &Fonts::small);
        }
        else {
            graph->text(" " + text + " ", pos, &Fonts::small);
        }
    }
};

class TopBar : public Entity {
public:
    BtnTopBar btnHome;
    BtnTopBar btnFile;
    BtnTopBar btnTools;
    BtnTopBar btnEdit;
    // File menu
    BtnTopBar btnNew;
    BtnTopBar btnSave;
    BtnTopBar btnLoad;
    // Tools menu
    BtnTopBar btnTests;
    // Edit menu
    BtnTopBar btnUndo;
    BtnTopBar btnRedo;
    BtnTopBar btnReset;
    BtnTopBar btnMVDown;
    BtnTopBar btnMVUp;
    BtnTopBar btnMVLeft;
    BtnTopBar btnMVRight;
    EntityManager em;
    Modal modal;
    int height;
    std::string activeTopBar;
    TopBar() : Entity() {
        tag = "TopBar";
        height = g_gm.vu(0.5f);

        btnHome.isHomeBtn = true;
        btnHome.onClick = []() { g_gm.setScreen(SCN_MAIN_MENU); };
        btnHome.available = true;
        btnHome.pos = Vec2i(g_gm.vu(0.25f), 4);
        btnHome.setCollider(Vec2i(g_gm.vu(0.25f), height));
        em.addEntity(&btnHome);

        btnFile.onClick = [this]() {
            activeTopBar = "btnFile";
            btnNew.available = true;
            btnSave.available = true;
            btnLoad.available = true;
        };
        btnFile.tag = "btnFile";
        btnFile.available = true;
        btnFile.text = "FILE";
        btnFile.pos = Vec2i(g_gm.vu(0.15f) + g_gm.vu(0.75f), 4);
        em.addEntity(&btnFile);

        btnEdit.onClick = [this]() {
            activeTopBar = "btnEdit";
            btnUndo.available = true;
            btnRedo.available = true;
            btnReset.available = true;
        };
        btnEdit.tag = "btnEdit";
        btnEdit.available = true;
        btnEdit.text = "EDIT";
        btnEdit.pos = Vec2i(g_gm.vu(0.15f) + g_gm.vu(2.25f), 4);
        em.addEntity(&btnEdit);

        btnTools.onClick = [this]() {
            activeTopBar = "btnTools";
            btnTests.available = true;
            btnMVDown.available = true;
            btnMVUp.available = true;
            btnMVLeft.available = true;
            btnMVRight.available = true;
        };
        btnTools.tag = "btnTools";
        btnTools.available = true;
        btnTools.text = "TOOL";
        btnTools.pos = Vec2i(g_gm.vu(0.15f) + g_gm.vu(3.75f), 4);
        em.addEntity(&btnTools);


        // File menu
        btnNew.tag = "btnNew";
        btnNew.onClick = [this]() {
            modal.onOk = [&]() {
                g_gm.setScreen(SCN_PUZZLE_SETUP);
            };
            modal.title = "Change Enigma?";
            modal.show = true;
        };
        btnNew.available = false;
        btnNew.text = "OPEN";
        btnNew.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + g_gm.vu(0.5f));
        em.addEntity(&btnNew);

        btnSave.onClick = [this]() { 
            modal.onOk = [&]() {
                g_gm.sendMessage("save");
            };
            modal.title = "Save?";
            modal.show = true; 
        };
        btnSave.available = false;
        btnSave.text = "SAVE";
        btnSave.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + g_gm.vu(1));
        em.addEntity(&btnSave);

        btnLoad.onClick = [this]() { 
            modal.onOk = [&]() {
                g_gm.sendMessage("load");
            };
            modal.title = "Load?";
            modal.show = true;
        };
        btnLoad.available = false;
        btnLoad.text = "LOAD";
        btnLoad.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + g_gm.vu(1.5f));
        em.addEntity(&btnLoad);

        // Tools menu
        btnTests.onClick = []() { DBG("TESTS"); g_gm.toggleTests(); };
        btnTests.available = false;
        btnTests.text = "SUBENIGMAS";
        btnTests.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + g_gm.vu(0.5f));
        em.addEntity(&btnTests);

        btnMVDown.onClick = [this]() { 
            g_gm.sendMessage("mv_down");
        };
        btnMVDown.available = false;
        btnMVDown.text = "DOWN";
        btnMVDown.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + g_gm.vu(1.0f));
        em.addEntity(&btnMVDown);

        btnMVUp.onClick = [this]() { 
            g_gm.sendMessage("mv_up");
        };
        btnMVUp.available = false;
        btnMVUp.text = "UP";
        btnMVUp.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + g_gm.vu(1.5f));
        em.addEntity(&btnMVUp);

        btnMVLeft.onClick = [this]() { 
            g_gm.sendMessage("mv_left");
        };
        btnMVLeft.available = false;
        btnMVLeft.text = "LEFT";
        btnMVLeft.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + g_gm.vu(2.0f));
        em.addEntity(&btnMVLeft);

        btnMVRight.onClick = [this]() { 
            g_gm.sendMessage("mv_right");
        };
        btnMVRight.available = false;
        btnMVRight.text = "RIGHT";
        btnMVRight.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + g_gm.vu(2.5f));
        em.addEntity(&btnMVRight);

        // Edit menu
        btnUndo.onClick = [this]() { 
            
        };
        btnUndo.available = false;
        btnUndo.text = "UNDO";
        btnUndo.pos = Vec2i(btnEdit.pos.x, btnEdit.pos.y + g_gm.vu(0.5f));
        em.addEntity(&btnUndo);

        btnRedo.onClick = [this]() { 
            
        };
        btnRedo.available = false;
        btnRedo.text = "REDO";
        btnRedo.pos = Vec2i(btnEdit.pos.x, btnEdit.pos.y + g_gm.vu(1.0f));
        em.addEntity(&btnRedo);
        
        btnReset.onClick = [this]() { 
            modal.onOk = [&]() {
                // _g.setReset(true); 
                g_gm.sendMessage("reset");
            };
            modal.title = "Reset?";
            modal.show = true;
        };
        btnReset.available = false;
        btnReset.text = "RESET";
        btnReset.pos = Vec2i(btnEdit.pos.x, btnEdit.pos.y + g_gm.vu(1.5f));
        em.addEntity(&btnReset);


        em.addEntity(&modal);
    }
    ~TopBar() {}
    void process() override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        if (g_gm.getHukActive()) return;
        // Clear if nothing is clicked
        if (GInput.mouseKeyOnce(SDL_BUTTON_LEFT)) {
            activeTopBar = "";
        }
        em.checkMouse();
        em.process();
        if (activeTopBar != "btnFile") {
            btnNew.available = false;
            btnSave.available = false;
            btnLoad.available = false;
        }
        if (activeTopBar != "btnTools") {
            btnTests.available = false;
            btnMVDown.available = false;
            btnMVUp.available = false;
            btnMVLeft.available = false;
            btnMVRight.available = false;
        }
        if (activeTopBar != "btnEdit") {
            btnUndo.available = false;
            btnRedo.available = false;
            btnReset.available = false;
        }
    }
    void render(Graphics* graph) override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        graph->setColor(g_colors["BG3"]);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, height));
        
        graph->setColor(g_colors["BG2"]);
        if (activeTopBar == "btnFile") {
            graph->rect(Vec2i(btnFile.pos.x, height), Vec2i(129, 120));
        }
        if (activeTopBar == "btnTools") {
            graph->rect(Vec2i(btnTools.pos.x, height), Vec2i(164, 180));
        }
        if (activeTopBar == "btnEdit") {
            graph->rect(Vec2i(btnEdit.pos.x, height), Vec2i(120, 120));
        }
        
        em.render(graph);
    }
};
