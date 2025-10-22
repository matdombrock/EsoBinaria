#pragma once

#include <map>
#include <string>

#include "Imp.h"
using namespace Imp;

#include "Cell.h"
#include "TestData.h"
#include "_helpItems.h"

#define WINDOW_SIZE Vec2i(1024, 768)
#define WINDOW_FPS 60
#define VERSION_STRING "0.8"

enum ScreenName {
  SCN_BUMPER,
  SCN_PUZZLE_SETUP,
  SCN_PUZZLE,
  SCN_HELP,
  SCN_MAIN_MENU,
  SCN_SETTINGS
};

class GameMaster {
public:
  const int fontSize = WINDOW_SIZE.x / 32;
  const int cellSize = WINDOW_SIZE.x / 16;
  const Vec2i bottomBarSize = Vec2i(WINDOW_SIZE.x, cellSize * 3);
  const Vec2i bottomBarPos = Vec2i(0, WINDOW_SIZE.y - bottomBarSize.y);
  Store store;
  Sound sndToggleTests;
  Sound sndChangeTile;
  Input input;
  GameMaster() {}
  ~GameMaster() {}
  void init() {

    sndToggleTests.set("toggle.wav");
    sndChangeTile.set("chtile.wav");

    std::string puzzleChallengeString = store.getString("puzzle_challenge");
    puzzleChallengeString.length() > 0
        ? puzzleChallenge = puzzleChallengeString[0]
        : puzzleChallenge = 'e';
    puzzleNum = store.getInt("puzzle_num");
    DBG("Loaded Current Puzzle: " + getPuzzleString());
  }
  int vu(float val) { return (int)(val * cellSize); }
  void setCodeString(std::string codeString) { this->codeString = codeString; }
  std::string getCodeString() { return codeString; }
  void setHelpItem(HelpItem *helpItem) { this->helpItem = helpItem; }
  HelpItem *getHelpItem() { return helpItem; }
  void setCodeErr(std::string err) { codeErr = err; }
  bool hasCodeErr() { return codeErr != ""; }
  std::string getCodeErr() { return codeErr; }
  void incTick() { tick++; }
  int getTick() { return tick; }
  void setScreen(ScreenName screenName) {
    DBG("Switching to screen: " + std::to_string(screenName));
    activeScreen = screenName;
  }
  ScreenName getScreen() { return activeScreen; }
  void setPuzzleBits(int bits) { puzzleBits = bits; }
  int getPuzzleBits() { return puzzleBits; }
  void toggleTests() {
    sndToggleTests.play();
    this->showTests = !this->showTests;
  }
  int getShowTests() { return showTests; }
  void setPuzzleNum(int num) {
    puzzleNum = num;
    store.setInt("puzzle_num", num);
  }
  int getPuzzleNum() { return puzzleNum; }
  void setPuzzleChallenge(char challenge) {
    puzzleChallenge = challenge;
    store.setString("puzzle_challenge", std::string(1, challenge));
  }
  char getPuzzleChallenge() { return puzzleChallenge; }
  void setPuzzleData(int bits, char challenge, int num) {
    puzzleBits = bits;
    setPuzzleChallenge(challenge);
    setPuzzleNum(num);
  }
  void setActiveTile(CellType type, bool playSound = true,
                     bool autoVoid = true) {
    if (type == CT_IND && puzzleBits < 4) {
      return;
    }
    if (autoVoid && type == activeTile) {
      type = CT_VOID;
    }
    if (playSound)
      sndChangeTile.play();
    activeTile = type;
  }
  CellType getActiveTile() { return activeTile; }
  void setActiveTestData(TestData *data) { activeTestData = data; }
  TestData *getActiveTestData() { return activeTestData; }
  std::string getPuzzleString() {
    return std::to_string(puzzleBits) + "." + puzzleChallenge +
           std::to_string(puzzleNum);
  }
  std::string getPuzzleStringDynamic(int puzzleBits, char puzzleChallenge,
                                     int puzzleNum) {
    return std::to_string(puzzleBits) + "." + puzzleChallenge +
           std::to_string(puzzleNum);
  }
  void setHukActive(bool active) { hukActive = active; }
  bool getHukActive() { return hukActive; }
  void setModalActive(bool active) { 
    modalActive = active; }
  bool getModalActive() { 
    return modalActive; }
  void sendMessage(std::string message) { messages.push_back(message); }
  std::vector<std::string> getMessages() { return messages; }
  void clearMessages() { messages.clear(); }

private:
  CellType activeTile = CT_VOID;
  TestData *activeTestData = nullptr;
  int tick = 0;
  int puzzleBits = 3;
  int puzzleNum = 0;
  char puzzleChallenge = 'e';
  ScreenName activeScreen = SCN_PUZZLE_SETUP;
  bool showTests = true;
  // bool reset = false;
  // bool quit = false;
  bool hukActive = false;
  bool modalActive = false;
  std::string codeErr = "";
  std::string codeString = "";
  HelpItem *helpItem = nullptr;
  std::vector<std::string> messages;
};
