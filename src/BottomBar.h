/*
 * The BottomBar class represents the bottom bar UI component in the application.
 * It contains buttons for various tile types and displays help information
 * based on user interaction.
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "BtnTile.h"
#include "g_helpItems.h"
#include "g_colors.h"
#include "g_gameMaster.h"
#include "g_fonts.h"

class BottomBar : public Entity {
public:
    EntityManager em;
    BtnTile btnInA;
    BtnTile btnInB;
    BtnTile btnInC;
    BtnTile btnInD;
    BtnTile btnClear;
    BtnTile btnBlank;
    BtnTile btnAnd;
    BtnTile btnOr;
    BtnTile btnNot;
    BtnTile btnXor;
    BtnTile btnNand;
    BtnTile btnNor;
    BtnTile btnXnor;
    BottomBar() : Entity() {
        tag = "bottomBar";

        btnClear.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["VOID"]);
        };
        btnClear.type = CT_CLEAR;
        btnClear.pos = Vec2i(g_gm.vu(1.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnClear);

        btnInA.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["A"]);
        };
        btnInA.type = CT_INA;
        btnInA.pos = Vec2i(g_gm.vu(2.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnInA);

        btnInB.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["B"]);
        };
        btnInB.type = CT_INB;
        btnInB.pos = Vec2i(g_gm.vu(3.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnInB);

        btnInC.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["C"]);
        };
        btnInC.type = CT_INC;
        btnInC.pos = Vec2i(g_gm.vu(4.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnInC);

        btnInD.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["D"]);
        };
        btnInD.type = CT_IND;
        btnInD.pos = Vec2i(g_gm.vu(5.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnInD);

        btnBlank.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["BLANK"]);
        };
        btnBlank.type = CT_BLANK;
        btnBlank.pos = Vec2i(g_gm.vu(6.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnBlank);
        
        btnAnd.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["AND"]);
        };
        btnAnd.type = CT_AND;
        btnAnd.pos = Vec2i(g_gm.vu(7.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnAnd);

        btnOr.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["OR"]);
        };
        btnOr.type = CT_OR;
        btnOr.pos = Vec2i(g_gm.vu(8.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnOr);

        btnNot.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["NOT"]);
        };
        btnNot.type = CT_NOT;
        btnNot.pos = Vec2i(g_gm.vu(9.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnNot);

        btnXor.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["XOR"]);
        };
        btnXor.type = CT_XOR;
        btnXor.pos = Vec2i(g_gm.vu(10.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnXor);

        btnNand.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["NAND"]);
        };
        btnNand.type = CT_NAND;
        btnNand.pos = Vec2i(g_gm.vu(11.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnNand);

        btnNor.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["NOR"]);
        };
        btnNor.type = CT_NOR;
        btnNor.pos = Vec2i(g_gm.vu(12.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnNor);

        btnXnor.onHover = [&](){
            g_gm.setHelpItem(&g_helpItems["XNOR"]);
        };
        btnXnor.type = CT_XNOR;        
        btnXnor.pos = Vec2i(g_gm.vu(13.5f), g_gm.bottomBarPos.y);
        em.addEntity(&btnXnor);
    }
    ~BottomBar() {}
    void process() override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        if (g_gm.getHukActive()) return;
        g_gm.setHelpItem(nullptr);
        em.checkMouse();
        em.process();
    }
    void render(Graphics* graph) override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        graph->setColor(g_colors["BG3"]);
        graph->rect(g_gm.bottomBarPos, g_gm.bottomBarSize);
        em.render(graph);

        int padX = g_gm.cellSize;
        int padY = g_gm.cellSize / 4;
        graph->setColor(g_colors["BG"]);
        graph->rect(Vec2i(0, g_gm.bottomBarPos.y + g_gm.cellSize), Vec2i(WINDOW_SIZE.x, g_gm.bottomBarSize.y - g_gm.cellSize), true);
        if (g_gm.getHelpItem() != nullptr) {
            graph->setColor(g_colors["GREEN"]);
            graph->text("Name: ", Vec2i(padX, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            int w = graph->textWidth("Name: ", &Fonts::medium);
            graph->setColor(200, 200, 200);
            graph->text(g_gm.getHelpItem()->title, Vec2i(padX + w, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            w += graph->textWidth(g_gm.getHelpItem()->title, &Fonts::medium);
            graph->setColor(g_colors["GRAY"]);
            graph->text(" (" + g_gm.getHelpItem()->key + ")", Vec2i(padX + w, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);

            graph->setColor(g_colors["GREEN"]);
            graph->text("Info: ", Vec2i(padX, g_gm.bottomBarPos.y + g_gm.cellSize + g_gm.fontSize + padY), &Fonts::medium);
            w = graph->textWidth("Info: ", &Fonts::medium);
            graph->setColor(200, 200, 200);
            graph->text(g_gm.getHelpItem()->desc, Vec2i(padX + w, g_gm.bottomBarPos.y + g_gm.cellSize + g_gm.fontSize + padY), &Fonts::medium);
        }
        else if (g_gm.hasCodeErr() && !StringTools::contains(g_gm.getCodeString(), "_")) {
            graph->setColor(g_colors["YELLOW"]);
            graph->text("!! Code Error: " + g_gm.getCodeErr(), Vec2i(padX, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
        }
        else if (g_gm.getCodeString().length() > 0) {
            std::string codePre = g_gm.hasCodeErr() ? "!!" :">> ";
            graph->setColor(g_gm.hasCodeErr() ? g_colors["YELLOW"] : g_colors["GREEN"]);
            graph->text(codePre, Vec2i(padX, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            std::string csMod = g_gm.getCodeString();
            if (g_gm.getActiveTestData() != nullptr) {
                DBG("Active test data replace");
                csMod = StringTools::replace(csMod, "AND", "XXX");// Protect AND from replacement
                std::string targets[4] = {"A", "B", "C", "D"}; 
                for (int i = 0; i < g_gm.getActiveTestData()->inputs.size(); i++) {
                    std::string rep = g_gm.getActiveTestData()->inputs[i] ? "1" : "0";
                    csMod = StringTools::replace(csMod, targets[i], rep);
                }
                csMod = StringTools::replace(csMod, "XXX", "AND");// Restore AND
            }
            // Split the string by _
            std::vector<std::string> split = StringTools::split(csMod, "_");
            std::string p1 = split[0];
            std::string p2 = "";
            std::string p3 = "";
            if (split.size() == 3) {
                p2 = split[1] + " ";
                p3 = split[2];
            }
            // 
            int p0w = graph->textWidth(codePre, &Fonts::medium);
            int p1w = graph->textWidth(p1, &Fonts::medium);
            int p2w = graph->textWidth(p2, &Fonts::medium);
            if (p1.length() > 0) {
                graph->setColor(g_colors["GRAY"]);
                graph->text(p1, Vec2i(padX + p0w, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            }
            if (p2.length() > 0) {
                graph->setColor(g_colors["GREEN"]);
                graph->text(p2, Vec2i(padX + p0w + p1w, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            }
            if (p3.length() > 0) {
                graph->setColor(g_colors["GRAY"]);
                graph->text(p3, Vec2i(padX + p0w + p1w + p2w, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
            }
        }
        else {
            graph->setColor(g_colors["GRAY"]);
            graph->text(">> No ESOP to display...", Vec2i(padX, g_gm.bottomBarPos.y + g_gm.cellSize + padY), &Fonts::medium);
        }
    }
};
