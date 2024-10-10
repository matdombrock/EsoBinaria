#pragma once

#include "../src/Imp.h"
using namespace Imp;
#include "TestData.h"
#include "../lib/s7/s7.h"
#include "_gameMaster.h"

class TestCase : public Entity {
public:
    TestData data;
    bool isHovered;
    bool show;
    int ce;
    Sprite sprTrue;
    Sprite sprFalse;
    Sprite sprPass;
    Sprite sprFail;
    TestCase() : Entity() {
        data.inputs.resize(4);
        data.output = false;
        data.err = "";
        data.lastCheck = false;
        isHovered = false;
        show = true;
        ce = _g.cellSize / 2;
        setCollider(Vec2i(_g.cellSize * 2, _g.cellSize / 2));
        s7 = s7_init();

        sprTrue = Sprite(Vec2i(0, 80), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
        std::vector<Vec2i> trueFrames = {
            Vec2i(0, 80), 
            Vec2i(16, 80), 
            Vec2i(32, 80), 
            Vec2i(48, 80), 
            Vec2i(64, 80), 
        };
        sprTrue.setAnimation(trueFrames, 4, true);
        sprFalse = Sprite(Vec2i(0, 96), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
        std::vector<Vec2i> falseFrames = {
            Vec2i(0, 96), 
            Vec2i(16, 96), 
            Vec2i(32, 96), 
            Vec2i(48, 96), 
            Vec2i(64, 96), 
        };
        sprFalse.setAnimation(falseFrames, 4, true);
        sprPass = Sprite(Vec2i(0, 112), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
        sprFail = Sprite(Vec2i(16, 112), Vec2i(16, 16), Vec2i(_g.cellSize/2, _g.cellSize/2));
    }
    ~TestCase() {}
    void set(int index, int bits, int puzzleNum) {
        this->data.index = index;
        data.inputs.resize(bits);
        for (int i = 0; i < bits; i++) {
            data.inputs[i] = Util::intToBits(index, bits)[i];
        }
        data.output = Util::intToBits(puzzleNum, Util::maxUnsignedInt(bits))[index];
    }
    void randomize() {
        for (int i = 0; i < 4; i++) {
            data.inputs[i] = rand() % 2;
        }
        data.output = rand() % 2;
    }
    bool check(std::string code) { 
        data.err = "";
        code = Util::toLowercase(code);
        code = Util::strReplace(code, "_", " ");
        DBG("Code Check:");
        DBG(code);
        if ((!Util::stringContains(code, "a") 
            && !Util::stringContains(code, "b") 
            && !Util::stringContains(code, "c") 
            && !Util::stringContains(code, "d"))
            || code == "")
        {
            data.lastCheck = false;
            data.err = "";
            DBG("Ignoring empty code");
            return data.lastCheck;
        }
        // code = "(display \"hello\"  )";
        std::string pre = "";
        pre += "(define (xor a b) (or (and a (not b)) (and (not a) b)))";
        pre += "(define (nand a b) (not (and a b)))";
        pre += "(define (nor a b) (not (or a b)))";
        pre += "(define (xnor a b) (not (xor a b)))";
        pre += "(define a " + boolScheme(data.inputs[0]) + ")";
        pre += "(define b " + boolScheme(data.inputs[1]) + ")";
        pre += "(define c " + boolScheme(data.inputs[2]) + ")";
        pre += "(define d " + boolScheme(data.inputs[3]) + ")";
        // pre += "(define output " + boolScheme(output) + ")";
        code = "(begin " + pre + code + ")";
        std::string eval = evalScheme(code);
        if (eval != "#t" && eval != "#f") {
            data.err = eval;
            data.lastCheck = false;
            return data.lastCheck;
        }
        std::string evalTarget = boolScheme(data.output);
        DBG("Eval: " + eval);
        DBG("Target: " + evalTarget);
        if (eval == evalTarget) data.lastCheck = true;
        else data.lastCheck = false;
        return data.lastCheck;
    }
    void process() override {

    }
    void render(Graphics* graph) override {
        if (!show) return;
        int hoverMod = isHovered ? _g.vu(0.1f) : 0;
        if (data.lastCheck) sprPass.render(graph, pos + Vec2i(hoverMod, 0));
        else sprFail.render(graph, Vec2i(pos.x - _g.vu(0.1f) - (_g.getTick()/8 % 6) + hoverMod, pos.y));
        int x = ce;
        for (bool input : data.inputs) {
            if (input) sprTrue.render(graph, Vec2i(pos.x + x, pos.y));
            else sprFalse.render(graph, Vec2i(pos.x + x, pos.y));
            x += ce;
        }
        if (data.output) {
            sprTrue.render(graph, Vec2i(pos.x + ce + x + hoverMod, pos.y));
        }
        else {
            sprFalse.render(graph, Vec2i(pos.x + ce + x + hoverMod, pos.y));
        }
        if (data.err != "") _g.setCodeErr(data.err);
    }
    void onMouse(bool over) override {
        if (!show) return;
        if (over) {
            DBG("Over test case " + std::to_string(data.index));
            isHovered = true;
            _g.setActiveTestData(&data);
            return;
        }
        if (_g.getActiveTestData() == &data) {
            _g.setActiveTestData(nullptr);
        }
        isHovered = false;
    }
private:
    s7_scheme* s7;
    std::string evalScheme(const std::string& expr) {
        s7_pointer result = s7_eval_c_string(s7, expr.c_str());
        return s7_object_to_c_string(s7, result);
    }
    bool schemeBool(std::string sb) {
        return sb == "#t";
    }
    std::string boolScheme(bool b) {
        return b ? "#t" : "#f";
    }
};