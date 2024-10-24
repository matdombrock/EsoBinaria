#pragma once

#include <string>
#include <map>

#include "../src/Imp.h"
using namespace Imp;

#include "_sounds.h"
#include "Cell.h"
#include "TestData.h"
#include "_helpItems.h"

#define WINDOW_SIZE Vec2i(1024, 768)

class GameMaster {
public:
    const int fontSize = WINDOW_SIZE.x / 32;
    const int cellSize = WINDOW_SIZE.x / 16;
    const Vec2i bottomBarSize = Vec2i(WINDOW_SIZE.x, cellSize * 3);
    const Vec2i bottomBarPos = Vec2i(0, WINDOW_SIZE.y - bottomBarSize.y);
    int vu(float val) {
        return (int)(val * cellSize);
    }
    void setReset(bool reset) {
        this->reset = reset;
        DBG("Reset: " + std::to_string(reset));
    }
    bool getReset() {
        return reset;
    }
    void setQuit(bool quit) {
        this->quit = quit;
    }
    bool getQuit() {
        return quit;
    }
    void setCodeString(std::string codeString) {
        this->codeString = codeString;
    }
    std::string getCodeString() {
        return codeString;
    }
    void setHelpItem(HelpItem* helpItem) {
        this->helpItem = helpItem;
    }
    HelpItem* getHelpItem() {
        return helpItem;
    }
    void setCodeErr(std::string err) {
        codeErr = err;
    }
    bool hasCodeErr() {
        return codeErr != "";
    }
    std::string getCodeErr() {
        return codeErr;
    }
    void incTick() {
        tick++;
    }
    int getTick() {
        return tick;
    }
    void setScreen(std::string screen) {
        activeScreen = screen;
    }
    std::string getScreen() {
        return activeScreen;
    }
    void setPuzzleBits(int bits) {
        puzzleBits = bits;
    }
    int getPuzzleBits() {
        return puzzleBits;
    }
    void toggleTests() {
        Sounds::toggleTests.play();
        this->showTests = !this->showTests;
    }
    int getShowTests() {
        return showTests;
    }
    void setPuzzleNum(int num) {
        puzzleNum = num;
    }
    int getPuzzleNum() {
        return puzzleNum;
    }
    void setPuzzleChallenge(char challenge) {
        puzzleChallenge = challenge;
    }
    char getPuzzleChallenge() {
        return puzzleChallenge;
    }
    void setActiveTile(CellType type, bool playSound = true, bool autoVoid = true) {
        if (type == CT_IND && puzzleBits < 4) {
            return;
        }
        if (autoVoid && type == activeTile) {
            type = CT_VOID;
        }
        if (playSound) Sounds::changeTile.play();
        activeTile = type;
    }
    CellType getActiveTile() {
        return activeTile;
    }
    void setActiveTestData(TestData* data) {
        activeTestData = data;
    }
    TestData* getActiveTestData() {
        return activeTestData;
    }
    std::string getPuzzleString() {
        return std::to_string(puzzleBits) + "." + std::to_string(puzzleNum) + puzzleChallenge;
    }
private:
    CellType activeTile = CT_VOID;
    TestData* activeTestData = nullptr;
    int tick = 0;
    int puzzleBits = 3;
    int puzzleNum = 255;
    char puzzleChallenge = 'e';
    std::string activeScreen = "puzzleSetup";
    bool showTests = true;
    bool reset = false;
    bool quit = false;
    std::string codeErr = "";
    std::string codeString = "";
    HelpItem* helpItem = nullptr;
};