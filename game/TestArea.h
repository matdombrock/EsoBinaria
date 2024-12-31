#pragma once

#include "../src/Imp.h"
using namespace Imp;
#include "TestCase.h"
#include "Modal.h"
#include "_colors.h"
#include "_gameMaster.h"
#include "_fonts.h"

class TestArea : public Entity {
public:
    EntityManager em;
    int testCount;
    int testFails;
    int testWinWidth;
    std::vector<TestCase> tests;
    std::string codeStringOld;
    Font font = Font("HomeVideo.ttf", _g.fontSize);
    Modal modal;
    bool shownPassModal = false;
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

        em.addEntity(&modal);
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
        if (_g.getScreen() != SCN_PUZZLE) return;
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
        pos.x = WINDOW_SIZE.x - (_g.getShowTests() ? testWinWidth : _g.cellSize * 0.75f);
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

        if (testFails == 0) {
            if (!hasPassed){
                _g.store.setBool("completed_lvl_" + _g.getPuzzleString(), true);
                _g.sendMessage("save");
                hasPassed = true;
            }
        }
        else hasPassed = false;
    }
    void render(Graphics* graph) {
        if (_g.getScreen() != SCN_PUZZLE) return;
        em.render(graph);
        if (_g.hasCodeErr()) {
            graph->setColor(_colors["YELLOW"]);
            graph->text("!! ERROR", Vec2i(pos.x + _g.vu(0.5f), pos.y - (_g.getTick()/4 % 8)), &Fonts::medium);
        }
        else {
            graph->setColor(_colors["GRAY"]);
            graph->text(_g.getPuzzleString(), Vec2i(pos.x + _g.vu(1.1f), pos.y + _g.vu(0.25f)), &Fonts::medium);
        }
        if (testFails == 0) {
            graph->setColor(_colors["GREEN"], 128);
            graph->text("PASSED", Vec2i(_g.cellSize,_g.cellSize), &Fonts::large);
            if (!shownPassModal) {
                modal.onOk = [&]() {
                    int bits = _g.getPuzzleBits();
                    char challenge = _g.getPuzzleChallenge();
                    int num = _g.getPuzzleNum();
                    num++;
                    if (challenge == 'e') {
                        if (num >= 4) {
                            num = 0;
                            challenge = 'm';
                        }
                    }
                    else if (challenge == 'm') {
                        if (num >= 64) {
                            num = 0;
                            challenge = 'h';
                        }
                    }
                    else if (challenge == 'h') {
                        _g.setScreen(SCN_PUZZLE_SETUP);
                        return;
                    }
                    _g.setPuzzleData(bits, challenge, num);
                    // _g.setReset(true);
                    _g.sendMessage("reset");
                    _g.setScreen(SCN_PUZZLE);
                };
                modal.title = "Passed!";
                modal.setOkText("Next");
                modal.setCancelText("Stay");
                modal.show = true;
                shownPassModal = true;
            }
        }
        else shownPassModal = false;
    }
private:
    bool hasPassed = false;
};