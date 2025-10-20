#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "BtnTile.h"
#include "_helpItems.h"
#include "_colors.h"
#include "_gameMaster.h"
#include "_fonts.h"

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
            _g.setHelpItem(&helpItems["VOID"]);
        };
        btnClear.type = CT_CLEAR;
        btnClear.pos = Vec2i(_g.vu(1.5f), _g.bottomBarPos.y);
        em.addEntity(&btnClear);

        btnInA.onHover = [&](){
            _g.setHelpItem(&helpItems["A"]);
        };
        btnInA.type = CT_INA;
        btnInA.pos = Vec2i(_g.vu(2.5f), _g.bottomBarPos.y);
        em.addEntity(&btnInA);

        btnInB.onHover = [&](){
            _g.setHelpItem(&helpItems["B"]);
        };
        btnInB.type = CT_INB;
        btnInB.pos = Vec2i(_g.vu(3.5f), _g.bottomBarPos.y);
        em.addEntity(&btnInB);

        btnInC.onHover = [&](){
            _g.setHelpItem(&helpItems["C"]);
        };
        btnInC.type = CT_INC;
        btnInC.pos = Vec2i(_g.vu(4.5f), _g.bottomBarPos.y);
        em.addEntity(&btnInC);

        btnInD.onHover = [&](){
            _g.setHelpItem(&helpItems["D"]);
        };
        btnInD.type = CT_IND;
        btnInD.pos = Vec2i(_g.vu(5.5f), _g.bottomBarPos.y);
        em.addEntity(&btnInD);

        btnBlank.onHover = [&](){
            _g.setHelpItem(&helpItems["BLANK"]);
        };
        btnBlank.type = CT_BLANK;
        btnBlank.pos = Vec2i(_g.vu(6.5f), _g.bottomBarPos.y);
        em.addEntity(&btnBlank);
        
        btnAnd.onHover = [&](){
            _g.setHelpItem(&helpItems["AND"]);
        };
        btnAnd.type = CT_AND;
        btnAnd.pos = Vec2i(_g.vu(7.5f), _g.bottomBarPos.y);
        em.addEntity(&btnAnd);

        btnOr.onHover = [&](){
            _g.setHelpItem(&helpItems["OR"]);
        };
        btnOr.type = CT_OR;
        btnOr.pos = Vec2i(_g.vu(8.5f), _g.bottomBarPos.y);
        em.addEntity(&btnOr);

        btnNot.onHover = [&](){
            _g.setHelpItem(&helpItems["NOT"]);
        };
        btnNot.type = CT_NOT;
        btnNot.pos = Vec2i(_g.vu(9.5f), _g.bottomBarPos.y);
        em.addEntity(&btnNot);

        btnXor.onHover = [&](){
            _g.setHelpItem(&helpItems["XOR"]);
        };
        btnXor.type = CT_XOR;
        btnXor.pos = Vec2i(_g.vu(10.5f), _g.bottomBarPos.y);
        em.addEntity(&btnXor);

        btnNand.onHover = [&](){
            _g.setHelpItem(&helpItems["NAND"]);
        };
        btnNand.type = CT_NAND;
        btnNand.pos = Vec2i(_g.vu(11.5f), _g.bottomBarPos.y);
        em.addEntity(&btnNand);

        btnNor.onHover = [&](){
            _g.setHelpItem(&helpItems["NOR"]);
        };
        btnNor.type = CT_NOR;
        btnNor.pos = Vec2i(_g.vu(12.5f), _g.bottomBarPos.y);
        em.addEntity(&btnNor);

        btnXnor.onHover = [&](){
            _g.setHelpItem(&helpItems["XNOR"]);
        };
        btnXnor.type = CT_XNOR;        
        btnXnor.pos = Vec2i(_g.vu(13.5f), _g.bottomBarPos.y);
        em.addEntity(&btnXnor);
    }
    ~BottomBar() {}
    void process() override {
        if (_g.getScreen() != SCN_PUZZLE) return;
        if (_g.getHukActive()) return;
        _g.setHelpItem(nullptr);
        em.checkMouse();
        em.process();
    }
    void render(Graphics* graph) override {
        if (_g.getScreen() != SCN_PUZZLE) return;
        graph->setColor(_colors["BG3"]);
        graph->rect(_g.bottomBarPos, _g.bottomBarSize);
        em.render(graph);

        int padX = _g.cellSize;
        int padY = _g.cellSize / 4;
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, _g.bottomBarPos.y + _g.cellSize), Vec2i(WINDOW_SIZE.x, _g.bottomBarSize.y - _g.cellSize), true);
        if (_g.getHelpItem() != nullptr) {
            graph->setColor(_colors["GREEN"]);
            graph->text("Name: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            int w = graph->textWidth("Name: ", &Fonts::medium);
            graph->setColor(200, 200, 200);
            graph->text(_g.getHelpItem()->title, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            w += graph->textWidth(_g.getHelpItem()->title, &Fonts::medium);
            graph->setColor(_colors["GRAY"]);
            graph->text(" (" + _g.getHelpItem()->key + ")", Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);

            graph->setColor(_colors["GREEN"]);
            graph->text("Info: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), &Fonts::medium);
            w = graph->textWidth("Info: ", &Fonts::medium);
            graph->setColor(200, 200, 200);
            graph->text(_g.getHelpItem()->desc, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), &Fonts::medium);
        }
        else if (_g.hasCodeErr() && !StringTools::contains(_g.getCodeString(), "_")) {
            graph->setColor(_colors["YELLOW"]);
            graph->text("!! Code Error: " + _g.getCodeErr(), Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
        }
        else if (_g.getCodeString().length() > 0) {
            std::string codePre = _g.hasCodeErr() ? "!!" :">> ";
            graph->setColor(_g.hasCodeErr() ? _colors["YELLOW"] : _colors["GREEN"]);
            graph->text(codePre, Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            std::string csMod = _g.getCodeString();
            if (_g.getActiveTestData() != nullptr) {
                DBG("Active test data replace");
                csMod = StringTools::replace(csMod, "AND", "XXX");// Protect AND from replacement
                std::string targets[4] = {"A", "B", "C", "D"}; 
                for (int i = 0; i < _g.getActiveTestData()->inputs.size(); i++) {
                    std::string rep = _g.getActiveTestData()->inputs[i] ? "1" : "0";
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
                graph->setColor(_colors["GRAY"]);
                graph->text(p1, Vec2i(padX + p0w, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            }
            if (p2.length() > 0) {
                graph->setColor(_colors["GREEN"]);
                graph->text(p2, Vec2i(padX + p0w + p1w, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            }
            if (p3.length() > 0) {
                graph->setColor(_colors["GRAY"]);
                graph->text(p3, Vec2i(padX + p0w + p1w + p2w, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
            }
        }
        else {
            graph->setColor(_colors["GRAY"]);
            graph->text(">> No ESOP to display...", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), &Fonts::medium);
        }
    }
};