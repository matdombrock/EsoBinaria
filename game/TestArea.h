#pragma once

#include "../src/Imp.h"
using namespace Imp;
#include "TestCase.h"
#include "_colors.h"
#include "_gameMaster.h"

class TestArea : public Entity {
public:
    EntityManager em;
    int testCount;
    int testFails;
    int testWinWidth;
    std::vector<TestCase> tests;
    std::string codeStringOld;
    TestArea() : Entity() {
        tag = "testScreen";
        
        DBG("Max unsigned int: " + std::to_string(Util::maxUnsignedInt(_g.getPuzzleBits())));

        tests.resize(256);
        reset();
        testWinWidth = _g.fontSize * 8;
        pos.y = _g.cellSize;
        for (int i = 0; i < tests.size(); i++) {
            em.addEntity(&tests[i]);
        }
    }
    ~TestArea() {}
    int check() {
        testFails = 0;
        for (int i = 0; i < testCount; i++) {
            if (!tests[i].check(_g.getCodeString())){
                testFails++;
            }
        }
        return testFails;
    }
    void reset() {
        switch (_g.getPuzzleChallenge()) {
            case 'e': testCount = Util::maxUnsignedInt(_g.getPuzzleBits()) / 4; break;
            case 'm': testCount = Util::maxUnsignedInt(_g.getPuzzleBits()) / 2; break;
            case 'h': testCount = Util::maxUnsignedInt(_g.getPuzzleBits()); break;
            default: testCount = Util::maxUnsignedInt(_g.getPuzzleBits()); break;
        }
        for (int i = 0; i < tests.size(); i++) {
           tests[i].set(i, _g.getPuzzleBits(), _g.getPuzzleNum()); 
        }
        check();
    }
    void process() {
        if (_g.getScreen() != "puzzle") return;
        em.checkMouse();
        em.process();
        std::string cs = StringTools::replace(StringTools::replace(_g.getCodeString(), " ", ""), "_", "");
        std::string cso = StringTools::replace(StringTools::replace(codeStringOld, " ", ""), "_", "");
        if (cs != cso) {
            _g.setCodeErr("");
            DBG("Retesting code");
            DBG(cs);
            int testFails = check();
            codeStringOld = _g.getCodeString();
        }
        if(_input.keyOnce(SDLK_SPACE)) {
            _g.toggleTests();
        }
        // Update positions
        // XXX - Maybe slow
        pos.x = WINDOW_SIZE.x - (_g.getShowTests() ? testWinWidth : _g.cellSize / 2);
        for (int i = 0; i < tests.size(); i++) {
            tests[i].pos = Vec2i(_g.vu(0.5f), _g.cellSize + (i * _g.cellSize /2 ));
            tests[i].pos += pos;
            tests[i].show = true;
            // DBG("testCount");
            // DBG(testCount);
            if (i >= testCount) {
                tests[i].show = false;
            }
        }
    }
    void render(Graphics* graph) {
        if (_g.getScreen() != "puzzle") return;
        em.render(graph);
        if (_g.hasCodeErr()) {
            graph->setColor(_colors["YELLOW"]);
            graph->text("!! ERROR", Vec2i(pos.x + _g.vu(0.5f), pos.y - (_g.getTick()/4 % 8)));
        }
        else {
            graph->setColor(_colors["GRAY"]);
            graph->text(_g.getPuzzleString(), Vec2i(pos.x + _g.vu(0.5f), pos.y));
        }
        if (testFails == 0) {
            graph->setColor(_colors["GREEN"]);
            graph->text("PASSED", Vec2i(_g.cellSize,_g.cellSize));
        }
    }
};