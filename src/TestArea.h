/*
* The TestArea class manages and displays test cases for puzzle validation.
* It checks the user's code against predefined test cases and shows pass/fail status.
* This holds all of the test cases for the current puzzle.
*/
#pragma once

#include "Imp.h"
using namespace Imp;
#include "Modal.h"
#include "TestCase.h"
#include "_colors.h"
#include "_fonts.h"
#include "_gameMaster.h"

class TestArea : public Entity {
public:
  EntityManager em;
  int testCount;
  int testFails;
  int testWinWidth;
  std::vector<TestCase> tests = {};
  std::string codeStringOld;
  Font font = Font("HomeVideo.ttf", g_gm.fontSize);
  Modal modal;
  bool shownPassModal = false;
  TestArea() : Entity() {
    tag = "testScreen";

    DBG("Max unsigned int: " +
        std::to_string(Util::maxUnsignedInt(g_gm.getPuzzleBits())));
    tests.resize(256);
    testWinWidth = g_gm.fontSize * 8;
    pos.y = g_gm.cellSize;
    for (int i = 0; i < tests.size(); i++) {
      em.addEntity(&tests[i]);
    }

    em.addEntity(&modal);
  }
  ~TestArea() {}
  int check() {
    testFails = 0;
    for (int i = 0; i < testCount; i++) {
      if (!tests[i].check(g_gm.getCodeString())) {
        testFails++;
      }
    }
    return testFails;
  }
  void reset() {
    DBG("Resetting tests");
    switch (g_gm.getPuzzleChallenge()) {
    case 'e':
      testCount = Util::maxUnsignedInt(g_gm.getPuzzleBits()) / 4;
      break;
    case 'm':
      testCount = Util::maxUnsignedInt(g_gm.getPuzzleBits()) / 2;
      break;
    case 'h':
      testCount = Util::maxUnsignedInt(g_gm.getPuzzleBits());
      break;
    default:
      testCount = Util::maxUnsignedInt(g_gm.getPuzzleBits());
      break;
    }
    for (int i = 0; i < tests.size(); i++) {
      tests[i].set(i, g_gm.getPuzzleBits(), g_gm.getPuzzleNum());
    }
    check();
  }
  void process() {
    if (g_gm.getScreen() != SCN_PUZZLE)
      return;
    em.checkMouse();
    em.process();
    std::string cs = StringTools::replace(
        StringTools::replace(g_gm.getCodeString(), " ", ""), "_", "");
    std::string cso = StringTools::replace(
        StringTools::replace(codeStringOld, " ", ""), "_", "");
    if (cs != cso) {
      g_gm.setCodeErr("");
      DBG("Retesting code");
      DBG(cs);
      int testFails = check();
      codeStringOld = g_gm.getCodeString();
    }
    if (GInput.keyOnce(SDLK_SPACE)) {
      g_gm.toggleTests();
    }
    // Update positions
    // XXX - Maybe slow
    pos.x = WINDOW_SIZE.x -
            (g_gm.getShowTests() ? testWinWidth : g_gm.cellSize * 0.75f);
    for (int i = 0; i < tests.size(); i++) {
      tests[i].pos = Vec2i(g_gm.vu(0.5f), g_gm.cellSize + (i * g_gm.cellSize / 2));
      tests[i].pos += pos;
      tests[i].show = true;
      // DBG("testCount");
      // DBG(testCount);
      if (i >= testCount) {
        tests[i].show = false;
      }
    }

    if (testFails == 0) {
      if (!hasPassed) {
        g_gm.store.setBool("completed_lvl_" + g_gm.getPuzzleString(), true);
        g_gm.sendMessage("save");
        hasPassed = true;
      }
    } else
      hasPassed = false;
  }
  void render(Graphics *graph) {
    if (g_gm.getScreen() != SCN_PUZZLE)
      return;
    em.render(graph);
    if (g_gm.hasCodeErr()) {
      graph->setColor(g_colors["YELLOW"]);
      graph->text("!! ERROR",
                  Vec2i(pos.x + g_gm.vu(0.5f), pos.y - (g_gm.getTick() / 4 % 8)),
                  &Fonts::medium);
    } else {
      graph->setColor(g_colors["GRAY"]);
      graph->text(g_gm.getPuzzleString(),
                  Vec2i(pos.x + g_gm.vu(1.1f), pos.y + g_gm.vu(0.25f)),
                  &Fonts::medium);
    }
    if (testFails == 0) {
      graph->setColor(g_colors["GREEN"], 128);
      graph->text("PASSED", Vec2i(g_gm.cellSize, g_gm.cellSize), &Fonts::large);
      if (!shownPassModal) {
        modal.onOk = [&]() {
          int bits = g_gm.getPuzzleBits();
          char challenge = g_gm.getPuzzleChallenge();
          int num = g_gm.getPuzzleNum();
          num++;
          if (challenge == 'e') {
            if (num >= 4) {
              num = 0;
              challenge = 'm';
            }
          } else if (challenge == 'm') {
            if (num >= 64) {
              num = 0;
              challenge = 'h';
            }
          } else if (challenge == 'h') {
            g_gm.setScreen(SCN_PUZZLE_SETUP);
            return;
          }
          g_gm.setPuzzleData(bits, challenge, num);
          g_gm.sendMessage("reset");
          g_gm.setScreen(SCN_PUZZLE);
        };
        modal.title = "Passed!";
        modal.setOkText("Next");
        modal.setCancelText("Stay");
        modal.show = true;
        shownPassModal = true;
      }
    } else
      shownPassModal = false;
  }

private:
  bool hasPassed = false;
};
