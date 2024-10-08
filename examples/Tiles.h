// ESOMachina - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "../src/Imp.h"
using namespace Imp;

#define WINDOW_SIZE Vec2i(1024, 768)

class HelpItem {
public:
    std::string title;
    std::string desc;
    std::string key;
};

std::map<std::string, HelpItem> helpItems = {
    {"A", {"IN_A", "Input A", "A"}},
    {"B", {"IN_B", "Input B", "S"}},
    {"C", {"IN_C", "Input C", "D"}},
    {"D", {"IN_D", "Input D", "F"}},
    {"VOID", {"CLEAR", "Clear the cell", "q"}},
    {"BLANK", {"BLANK", "Blank cell", "w"}},
    {"AND", {"AND", "Outputs true if both inputs are true", "e"}},
    {"OR", {"OR", "Outputs true if either input is true", "r"}},
    {"NOT", {"NOT", "Outputs true if input is false", "t"}},
    {"XOR", {"XOR", "Outputs true if inputs are different", "y"}},
    {"NAND", {"NAND", "Outputs false if both inputs are true", "u"}},
    {"NOR", {"NOR", "Outputs false if either input is true", "i"}},
    {"XNOR", {"XNOR", "Outputs false if inputs are different", "o"}},
};

std::map<std::string, Color> _colors = {
    {"BG", Color(20,25,20)},
    {"BG2", Color(45,50,45)},
    {"BG3", Color(55,55,60)},
    {"GREEN", Color(94,180,93)},
    {"YELLOW", Color(238,198,75)},
    {"RED", Color(186,89,90)},
    {"GRAY", Color(155,155,155)},
    {"WHITE", Color(255,255,255)},
};

enum CellType {
    CT_VOID = 999,
    CT_CLEAR = 0,
    CT_INA = 1,
    CT_INB = 2,
    CT_INC = 3,
    CT_IND = 4,
    CT_BLANK = 5,
    CT_AND = 6,
    CT_OR = 7,
    CT_NOT = 8,
    CT_XOR = 9,
    CT_NAND = 10,
    CT_NOR = 11,
    CT_XNOR = 12
};

class Cell {
public:
    CellType type;
    bool parenLeft;
    bool parenRight;
    bool isComment;
    Cell() {
        type = CT_CLEAR;
        parenLeft = false;
        parenRight = false;
        isComment = false;
    }
    ~Cell() {}
    void set(CellType type) {
        if (type == CT_CLEAR) {
            parenLeft = false;
            parenRight = false;
            isComment = false;
        }
        this->type = type;
    }
    CellType get() {
        return type;
    }
    void cycleParens() {
        if (parenLeft) {
            parenLeft = false;
            parenRight = true;
        }
        else if (parenRight) {
            parenRight = false;
            parenLeft = false;
        }
        else {
            parenLeft = true;
            parenRight = false;
        }
    }
    static std::string typeToString(CellType type) {
        switch (type) {
            case CT_VOID: return "VOID";
            case CT_CLEAR: return "VOID";
            case CT_INA: return "A";
            case CT_INB: return "B";
            case CT_INC: return "C";
            case CT_IND: return "D";
            case CT_BLANK: return "BLANK";
            case CT_AND: return "AND";
            case CT_OR: return "OR";
            case CT_NOT: return "NOT";
            case CT_XOR: return "XOR";
            case CT_NAND: return "NAND";
            case CT_NOR: return "NOR";
            case CT_XNOR: return "XNOR";
        }
        DBG("CellType not found " + std::to_string(type));
        return "????";
    }
};

namespace Sounds {
    Sound menuTrack = Sound("menu.ogg");
    Sound changeTile = Sound("2.wav");
    Sound toggleMainMenu = Sound("3.wav");
    Sound toggleTests = Sound("4.wav");
};

class TestData {
public:
    int index;
    std::vector<bool> inputs;
    bool output;
    std::string err;
    bool lastCheck;
};

class Globals {
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
    void toggleMainMenu() {
        Sounds::toggleMainMenu.play();
        showMainMenu = !showMainMenu;
    }
    void setShowMainMenu(bool show) {
        showMainMenu = show;
    }
    bool getShowMainMenu() {
        return showMainMenu;
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
private:
    CellType activeTile = CT_VOID;
    TestData* activeTestData = nullptr;
    int tick = 0;
    int puzzleBits = 3;
    int puzzleNum = 255;
    bool showMainMenu = false;
    bool showTests = true;
    bool reset = false;
    bool quit = false;
    std::string codeErr = "";
    std::string codeString = "";
    HelpItem* helpItem = nullptr;
};
Globals _g;

namespace CellSprites {
    Sprite baseTile = Sprite(Vec2i(0, 0), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite blankTile = Sprite(Vec2i(0, 32), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite andTile  = Sprite(Vec2i(0, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite orTile = Sprite(Vec2i(16, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite notTile = Sprite(Vec2i(32, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite xorTile = Sprite(Vec2i(48, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite nandTile = Sprite(Vec2i(64, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite norTile = Sprite(Vec2i(80, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite xnorTile = Sprite(Vec2i(96, 16), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite parenR = Sprite(Vec2i(32, 0), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inATile = Sprite(Vec2i(0, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inBTile = Sprite(Vec2i(16, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inCTile = Sprite(Vec2i(32, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inDTile = Sprite(Vec2i(48, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite clear = Sprite(Vec2i(16, 32), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite specVoid = Sprite(Vec2i(256,256), Vec2i(0,0), Vec2i(0,0)); 
    // Special
    Sprite inATrueTile = Sprite(Vec2i(64, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inBTrueTile = Sprite(Vec2i(80, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inCTrueTile = Sprite(Vec2i(96, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inDTrueTile = Sprite(Vec2i(112, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inAFalseTile = Sprite(Vec2i(128, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inBFalseTile = Sprite(Vec2i(144, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inCFalseTile = Sprite(Vec2i(160, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    Sprite inDFalseTile = Sprite(Vec2i(176, 48), Vec2i(16, 16), Vec2i(_g.cellSize, _g.cellSize));
    std::map <CellType, Sprite*> cellMap = {
        {CT_VOID, &specVoid},
        {CT_CLEAR, &clear},
        {CT_AND, &andTile},
        {CT_OR, &orTile},
        {CT_NOT, &notTile},
        {CT_XOR, &xorTile},
        {CT_NAND, &nandTile},
        {CT_NOR, &norTile},
        {CT_XNOR, &xnorTile},
        {CT_BLANK, &blankTile},
        {CT_INA, &inATile},
        {CT_INB, &inBTile},
        {CT_INC, &inCTile},
        {CT_IND, &inDTile},
    };
};

class TestCase : public Entity {
public:
    TestData data;
    bool isHovered;
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

class TestScreen : public Entity {
public:
    EntityManager em;
    int testFails;
    int testWinWidth;
    std::vector<TestCase> tests;
    std::string codeStringOld;
    TestScreen() : Entity() {
        tag = "testScreen";
        tests.resize(Util::maxUnsignedInt(_g.getPuzzleBits()));
        DBG("Max unsigned int: " + std::to_string(Util::maxUnsignedInt(_g.getPuzzleBits())));

        // not dry - reset
        for (int i = 0; i < tests.size(); i++) {
            tests[i].set(i, _g.getPuzzleBits(), _g.getPuzzleNum());
        }
        testWinWidth = _g.fontSize * 8;
        pos.y = _g.cellSize;
        for (int i = 0; i < tests.size(); i++) {
            em.addEntity(&tests[i]);
        }
    }
    ~TestScreen() {}
    void reset() {
        for (int i = 0; i < tests.size(); i++) {
           tests[i].set(i, _g.getPuzzleBits(), _g.getPuzzleNum()); 
        }
    }
    void process() {
        if (_g.getShowMainMenu()) return;
        em.checkMouse();
        em.process();
        std::string cs = Util::strReplace(Util::strReplace(_g.getCodeString(), " ", ""), "_", "");
        std::string cso = Util::strReplace(Util::strReplace(codeStringOld, " ", ""), "_", "");
        if (cs != cso) {
            _g.setCodeErr("");
            DBG("Retesting code");
            DBG(cs);
            testFails = 0;
            for (TestCase& test : tests) {
                if (!test.check(_g.getCodeString())){
                    testFails++;
                }
            }
            codeStringOld = _g.getCodeString();
        }
        if(_input.keyDown(SDLK_SPACE)) {
            _g.toggleTests();
        }
        // Update positions
        // XXX - Maybe slow
        pos.x = WINDOW_SIZE.x - (_g.getShowTests() ? testWinWidth : _g.cellSize / 2);
        for (int i = 0; i < tests.size(); i++) {
            tests[i].pos = Vec2i(_g.vu(0.5f), _g.cellSize + (i * _g.cellSize /2 ));
            tests[i].pos += pos;
        }
    }
    void render(Graphics* graph) {
        if (_g.getShowMainMenu()) return;
        em.render(graph);
        //graph->setColor(_colors["BG3"]);
        //graph->rect(Vec2i(pos.x, 0), Vec2i(testWinWidth, _g.bottomBarPos.y - _g.cellSize), true);
        if (_g.hasCodeErr()) {
            graph->setColor(_colors["YELLOW"]);
            graph->text("!! ERROR", Vec2i(pos.x + _g.vu(0.5f), pos.y - (_g.getTick()/4 % 8)), _g.fontSize);
        }
        else {
            graph->setColor(_colors["GRAY"]);
            graph->text("3." + std::to_string(_g.getPuzzleNum()), Vec2i(pos.x + _g.vu(0.5f), pos.y), _g.fontSize);
        }
        if (testFails == 0) {
            graph->setColor(_colors["GREEN"]);
            graph->text("PASSED", Vec2i(_g.cellSize/4,_g.cellSize/4), _g.fontSize * 4);
        }
    }
};

class BtnMainMenu : public Entity {
public:
    Vec2i size;
    std::string text;
    Uint8 state;
    bool center;
    BtnMainMenu() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(200, _g.fontSize * 1.1f);
        setCollider(size);
    }
    ~BtnMainMenu() {}
    bool isClicked() {
        return state == 2;
    }
    void process() override {}
    void render(Graphics* graph) override {
        Color* c = &_colors["GRAY"];
        switch (state) {
            case 0: c = &_colors["GRAY"]; break;
            case 1: c = &_colors["YELLOW"]; break;
            case 2: c = &_colors["GREEN"]; break;
        }
        std::string textMod = (state > 0 ? ">> " : "") + text;
        graph->setColor(*c);
        if (center) {
            int textWidth = graph->textWidth(textMod, _g.fontSize);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - _g.fontSize) / 2);
            graph->text(textMod, textPos, _g.fontSize);
        }
        else {
            graph->text(textMod, pos, _g.fontSize);
        }
    }
    void onMouse(bool over) override {
        if (_input.mouseKeyDown(SDL_BUTTON_LEFT) && over) {
            state = 2;
        }
        else if (over) {
            state = 1;
        }
        else state = 0;
    }
};


class BtnTopMenu : public Entity {
public:
    Vec2i size;
    int fontSize;
    std::string text;
    Uint8 state;
    bool center;
    bool show;
    bool isHomeBtn;
    std::function<void()> onClick;
    BtnTopMenu() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        show = false;
        text = "Button";
        fontSize = _g.fontSize * 0.75f;
        size = Vec2i(80, fontSize * 1.1f);
        isHomeBtn = false;
        setCollider(size);
    }
    ~BtnTopMenu() {}
    bool isHovered() {
        return state == 1;
    }
    bool isClicked() {
        return state == 2;
    }
    void process() override {
        timer > 0 ? timer-- : timer = 0;
    }
    void render(Graphics* graph) override {
        if (!show) return;
        // graph->setColor(_colors["BG2"]);
        // graph->rect(pos, size, true);
        Color* c = &_colors["GRAY"];
        switch (state) {
            case 0: c = &_colors["GRAY"]; break;
            case 1: c = &_colors["YELLOW"]; break;
            case 2: c = &_colors["GREEN"]; break;
        }
        graph->setColor(*c);
        if (isHomeBtn) {
            int sz = _g.vu(0.25f);
            //graph->circle(pos + Vec2i(sz/2, sz/2), sz/2, true);
            // draw a diamond with triangles
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(sz, sz/2), pos + Vec2i(sz/2, sz));
            graph->tri(pos + Vec2i(sz/2, 0), pos + Vec2i(0, sz/2), pos + Vec2i(sz/2, sz));

            return;
        }
        if (center) {
            int textWidth = graph->textWidth(text, fontSize);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - fontSize) / 2);
            graph->text(text, textPos, fontSize);
        }
        else {
            graph->text(" " + text + " ", pos, fontSize);
        }
    }
    void onMouse(bool over) override {
        if (!show) return;
        if (timer == 0) {
            if (_input.mouseKeyDown(SDL_BUTTON_LEFT) && over) {
                state = 2;
                if (onClick != nullptr) onClick();
                // reset timer
                timer = 16;
            }
            else if (over) {
                state = 1;
            }
            else state = 0;
        }
    }
private:
    int timer = 0;
};

class BtnTile : public Entity {
public:
    CellType type;
    Uint8 state;
    Uint8 oldState;
    Sound sndTick;
    std::function<void()> onClick;
    std::function<void()> onHover;
    BtnTile() : Entity() {
        state = 99; // 99 prevents play on start
        oldState = 99;
        tag = "tileBtn";
        type = CT_CLEAR;
        setCollider(Vec2i(_g.cellSize, _g.cellSize));
        sndTick.set("1.wav");
    }
    ~BtnTile() {}
    void render(Graphics* graph) override {
            if (type == CT_IND && _g.getPuzzleBits() < 4) {
                return;
            }
            Vec2i post = pos.copy();
            
            if (_g.getActiveTile() == type) {
                post += Vec2i(0, -_g.cellSize / 4);
            }
            else if (state > 0) {
            post += Vec2i(0, -_g.cellSize / 8);
        }
        graph->setColor(_colors["GREEN"]);
        graph->rect(pos + Vec2i(_g.cellSize / 4, _g.cellSize / 8), Vec2i(_g.cellSize - (_g.cellSize / 2), _g.cellSize - _g.cellSize / 4));
        CellSprites::baseTile.render(graph, post);
        CellSprites::cellMap[type]->render(graph, post);
    }
    void onMouse(bool over) override {
        oldState = state;
        if (over) {
            state = 1;
            if (_input.mouseKeyDown(SDL_BUTTON_LEFT)) {
                state = 2;
                _g.setActiveTile(type);
            }
            if (state > 0) {
                if (onHover != nullptr) onHover();
            }
            if (state == 1 && oldState == 0) {
                DBG("play ");
                sndTick.play();
            }
            return;
        }
        state = 0;
    }
};

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
        if (_g.getShowMainMenu()) return;
        _g.setHelpItem(nullptr);
        em.checkMouse();
        em.process();
    }
    void render(Graphics* graph) override {
        if (_g.getShowMainMenu()) return;
        graph->setColor(_colors["BG3"]);
        graph->rect(_g.bottomBarPos, _g.bottomBarSize);
        em.render(graph);

        int padX = _g.cellSize;
        int padY = _g.cellSize / 4;
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, _g.bottomBarPos.y + _g.cellSize), Vec2i(WINDOW_SIZE.x, _g.bottomBarSize.y - _g.cellSize), true);
        if (_g.getHelpItem() != nullptr) {
            graph->setColor(_colors["GREEN"]);
            graph->text("Name: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            int w = graph->textWidth("Name: ", _g.fontSize);
            graph->setColor(200, 200, 200);
            graph->text(_g.getHelpItem()->title, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            w += graph->textWidth(_g.getHelpItem()->title, _g.fontSize);
            graph->setColor(_colors["GRAY"]);
            graph->text(" (" + _g.getHelpItem()->key + ")", Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);

            graph->setColor(_colors["GREEN"]);
            graph->text("Info: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), _g.fontSize);
            w = graph->textWidth("Info: ", _g.fontSize);
            graph->setColor(200, 200, 200);
            graph->text(_g.getHelpItem()->desc, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), _g.fontSize);
        }
        else if (_g.getCodeString().length() > 0) {
            std::string codePre = _g.hasCodeErr() ? "!!" :">> ";
            graph->setColor(_g.hasCodeErr() ? _colors["YELLOW"] : _colors["GREEN"]);
            graph->text(codePre, Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            int w = graph->textWidth(codePre, _g.fontSize);
            //
            // if (_g.hasCodeErr()) {
            //     graph->setColor(_colors["YELLOW"]);
            //     graph->text(_g.getCodeErr(), Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            //     return;
            // }
            std::string csMod = _g.getCodeString();
            if (_g.getActiveTestData() != nullptr) {
                DBG("Active test data replace");
                csMod = Util::strReplace(csMod, "AND", "XXX");// Protect AND from replacement
                std::string targets[4] = {"A", "B", "C", "D"}; 
                for (int i = 0; i < _g.getActiveTestData()->inputs.size(); i++) {
                    std::string rep = _g.getActiveTestData()->inputs[i] ? "1" : "0";
                    csMod = Util::strReplace(csMod, targets[i], rep);
                }
                csMod = Util::strReplace(csMod, "XXX", "AND");// Restore AND
            }
            // Split the string by _
            std::vector<std::string> split = Util::splitString(csMod, "_");
            std::string p1 = split[0];
            std::string p2 = "";
            std::string p3 = "";
            if (split.size() == 3) {
                p2 = split[1] + " ";
                p3 = split[2];
            }
            // 
            int p1w = graph->textWidth(p1, _g.fontSize);
            int p2w = graph->textWidth(p2, _g.fontSize);
            if (p1.length() > 0) {
                graph->setColor(_colors["GRAY"]);
                graph->text(p1, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
            if (p2.length() > 0) {
                graph->setColor(_colors["GREEN"]);
                graph->text(p2, Vec2i(padX + w + p1w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
            if (p3.length() > 0) {
                graph->setColor(_colors["GRAY"]);
                graph->text(p3, Vec2i(padX + w + p1w + p2w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
        }
    }
};

class Grid : public Entity {
public:
    Vec2i mousePos;
    Vec2i mousePosCell;
    Vec2i gridSize;
    std::string highlightCellTypeStr;
    std::string helpString;
    std::vector<std::vector<Cell>> cells;
    Vec2i lastMouse[8];
    Sound sndTick;
    Sound sndAdd;
    Sound sndRemove;
    Sound sndParen;
    Grid() : Entity() {
        DBG("Starting grid");
        tag = "grid";
        Vec2i gridBounds = Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y - _g.bottomBarSize.y);
        gridSize = gridBounds / _g.cellSize;
        // Resize the 2d array to the grid size
        cells.resize(gridSize.x);
        // Fill cells with 0
        for (int x = 0; x < gridSize.x; x++) {
            cells[x].resize(gridSize.y);
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_CLEAR);
            }
        }

        sndTick.set("1.wav");
        // sndTick.volume = 32;
        sndAdd.set("14.wav");
        sndRemove.set("rock.ogg");
        sndParen.set("5.wav");  

        highlightCellTypeStr = "CLEAR";
        helpString = "";
        DBG("Grid started");

        // DBG basic test
        cells[3][3].set(CT_NOT);
        cells[3][3].parenLeft = true;
        cells[4][3].set(CT_OR);
        cells[4][3].parenLeft = true;
        cells[5][3].set(CT_INA);
        cells[6][3].set(CT_INB);
        cells[7][3].set(CT_INC);
        cells[7][3].parenRight = true;
        cells[8][3].set(CT_BLANK);
        cells[8][3].parenRight = true;
    }
    ~Grid() {}
    void reset() {
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_CLEAR);
            }
        }
    }
    void process() override {
        if (_g.getShowMainMenu()) return;
        mousePos = _input.mousePos();
        mousePosCell = mousePos / _g.cellSize;
        // Update lastMouse array with the most recent mousePosCell value
        if (_g.getTick() % 2 == 0) {
            for (int i = 7; i > 0; i--) {
                lastMouse[i] = lastMouse[i - 1];
            }
            lastMouse[0] = mousePosCell;
        }
        int x = mousePosCell.x;
        int y = mousePosCell.y;
        CellType cellType = cells[x][y].get();
        bool isTile = cellType != CT_VOID && cellType != CT_CLEAR;
        highlightCellTypeStr = Cell::typeToString(cellType);
        
        if (_input.keyDown(SDLK_q)) _g.setActiveTile(CT_CLEAR);
        if (_input.keyDown(SDLK_BACKSPACE)) _g.setActiveTile(CT_CLEAR);
        if (_input.keyDown(SDLK_w)) _g.setActiveTile(CT_BLANK);
        if (_input.keyDown(SDLK_e)) _g.setActiveTile(CT_AND);
        if (_input.keyDown(SDLK_r)) _g.setActiveTile(CT_OR);
        if (_input.keyDown(SDLK_t)) _g.setActiveTile(CT_NOT);
        if (_input.keyDown(SDLK_a)) _g.setActiveTile(CT_INA);
        if (_input.keyDown(SDLK_s)) _g.setActiveTile(CT_INB);
        if (_input.keyDown(SDLK_d)) _g.setActiveTile(CT_INC);
        if (_input.keyDown(SDLK_f)) _g.setActiveTile(CT_IND);
        if (_input.keyDown(SDLK_y)) _g.setActiveTile(CT_XOR);
        if (_input.keyDown(SDLK_u)) _g.setActiveTile(CT_NAND);
        if (_input.keyDown(SDLK_i)) _g.setActiveTile(CT_NOR);
        if (_input.keyDown(SDLK_o)) _g.setActiveTile(CT_XNOR);

        if (_input.mousePos().y < _g.bottomBarPos.y) {
            if (lastMouse[0] != lastMouse[1]) {
                int relMouse = ((float)mousePosCell.x / gridSize.x) * 255;
                if(_g.getActiveTestData() == nullptr) {
                    sndTick.setPan(255 - relMouse, relMouse);
                    sndTick.play();
                }
            }

            CellType newCell = CT_VOID;
            if (_input.mouseKeyDown(SDL_BUTTON_LEFT)) {
                CellType active = _g.getActiveTile();
                // Grab tile if nothing is active
                if (active == CT_VOID && cellType != CT_CLEAR) {
                    DBG("Grabbed cell type: " + Cell::typeToString(cellType));
                    _g.setActiveTile(cellType, true);
                    newCell = CT_CLEAR;
                }
                else {
                    newCell = active;
                }
            }
            if (newCell != CT_VOID) {
                DBG("New Cell: " + std::to_string(newCell));
                if (newCell == CT_CLEAR) {
                    if (isTile) sndRemove.play();
                }
                else sndAdd.play();
                cells[x][y].set(newCell);
                // clear actie Tile
                if (newCell != CT_CLEAR) {
                    _g.setActiveTile(CT_VOID, false);   
                }
            }
            // Toggle connectors
            if (_input.keyDown(SDLK_TAB) || _input.mouseKeyDown(SDL_BUTTON_RIGHT)) {
                cells[x][y].cycleParens();
                if (isTile) sndParen.play();
            }
            // Toggle comments
            if (_input.keyDown(SDLK_c) || _input.mouseKeyDown(SDL_BUTTON_MIDDLE)) {
                cells[x][y].isComment = !cells[x][y].isComment;
            }
        }

        // Generate code string
        std::string codeStringOld = _g.getCodeString();
        std::string codeStringNew = "";
        _g.setCodeString("");
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                if (cells[x][y].isComment) continue;
                CellType cellType = cells[x][y].get();
                if (cellType != CT_CLEAR) {
                    bool isHovered = mousePosCell.x == x && mousePosCell.y == y;
                    if (isHovered) codeStringNew += "_";
                    if (cells[x][y].parenLeft) codeStringNew += "(";
                    if (cellType != CT_BLANK) codeStringNew += Cell::typeToString(cellType);
                    if (cells[x][y].parenRight) codeStringNew += ")";
                    if (isHovered) codeStringNew += "_";
                    else if (cellType != CT_BLANK) codeStringNew += " ";
                }
            }
        }
        _g.setCodeString(codeStringNew);
    }
    void render(Graphics* graph) override {
        if (_g.getShowMainMenu()) return;
        // Drag grid bg
        graph->setColor(_colors["BG2"]);
        for (int x = 0; x < WINDOW_SIZE.x; x += _g.cellSize) {
            graph->line(Vec2i(x, 0), Vec2i(x, WINDOW_SIZE.y));
        }
        for (int y = 0; y < WINDOW_SIZE.y; y += _g.cellSize) {
            graph->line(Vec2i(0, y), Vec2i(WINDOW_SIZE.x, y));
        }
        // Draw border
        int borderWidth = _g.hasCodeErr() ? _g.cellSize/16 : _g.cellSize/4;
        graph->setColor(_g.hasCodeErr() ? _colors["YELLOW"] : _colors["BG2"], _g.hasCodeErr() ? 128 : 255);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, _g.bottomBarPos.y - borderWidth), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        graph->rect(Vec2i(WINDOW_SIZE.x - borderWidth, pos.y), Vec2i(borderWidth, WINDOW_SIZE.y));
        
        // Draw mouse trail
        for (int i = 0; i < 8; i++) {
            Vec2i pos = lastMouse[i];
            Vec2i cellPos = pos * _g.cellSize;
            Color c = _colors["GREEN"];
            graph->setColor(c.r, c.g - (i * 8), c.b);
            graph->rect(cellPos, Vec2i(_g.cellSize, _g.cellSize), false);
        }

        // Draw cells
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_VOID && cellType != CT_CLEAR) {
                    CellSprites::baseTile.render(graph, Vec2i(_g.cellSize * x, _g.cellSize * y));
                    Sprite* spr = nullptr;
                    // Draw test highlights
                    if (_g.getActiveTestData() != nullptr) {
                        if (cellType == CT_INA) {
                            spr = _g.getActiveTestData()->inputs[0] ? &CellSprites::inATrueTile : &CellSprites::inAFalseTile;
                        }
                        if (cellType == CT_INB) {
                            spr = _g.getActiveTestData()->inputs[1] ? &CellSprites::inBTrueTile : &CellSprites::inBFalseTile;
                        }
                        if (cellType == CT_INC) {
                            spr = _g.getActiveTestData()->inputs[2] ? &CellSprites::inCTrueTile : &CellSprites::inCFalseTile;
                        }
                        if (cellType == CT_IND) {
                            spr = _g.getActiveTestData()->inputs[3] ? &CellSprites::inDTrueTile : &CellSprites::inDFalseTile;
                        }
                    }
                    else {
                        spr = CellSprites::cellMap[cellType];
                    }
                    if (spr != nullptr) {
                        spr->render(graph, Vec2i(_g.cellSize * x, _g.cellSize * y));
                    }
                    else {
                        DBG("Unknown cell type: " + std::to_string(cellType));
                    }
                    // Draw parenthesis
                    if (cells[x][y].parenLeft) {
                        CellSprites::parenR.render(graph, Vec2i(_g.cellSize * x, _g.cellSize * y), true);
                    }
                    if (cells[x][y].parenRight) {
                        CellSprites::parenR.render(graph, Vec2i(_g.cellSize * x, _g.cellSize * y));
                    }
                    // Comment
                    if (cells[x][y].isComment) {
                        graph->setColor(_colors["GRAY"], 128 + (std::sin(_g.getTick()/32.0f) * 16));
                        graph->rect(Vec2i(_g.cellSize * x, _g.cellSize * y), Vec2i(_g.cellSize, _g.cellSize));
                    }
                }
            }
        }

        // Highlight cell on mouse hover
        if (_g.getActiveTestData() == nullptr && mousePos.y < _g.bottomBarPos.y) {
            Vec2i cell = mousePos / _g.cellSize * _g.cellSize;
            graph->setColor(_colors["WHITE"]);
            graph->rect(cell, Vec2i(_g.cellSize, _g.cellSize), false); 
            if (_g.getActiveTile() != CT_VOID) {
                CellSprites::baseTile.render(graph, cell - Vec2i(0, _g.vu(0.25f)));
                CellSprites::cellMap[_g.getActiveTile()]->render(graph, cell - Vec2i(0, _g.vu(0.25f)));
            }
        }

        // Draw static noise
        if (_g.getTick() % 2 == 0) {
            int staticSize = _g.cellSize / 32;
            graph->setColor(_colors["YELLOW"], 64);
            for (int i = 0; i < WINDOW_SIZE.size2d()/(staticSize * (_g.hasCodeErr() ? 128 : 1024)); i++) {
                int x = rand() % WINDOW_SIZE.x;
                int y = rand() % WINDOW_SIZE.y;
                graph->rect(Vec2i(x, y), Vec2i(staticSize, staticSize));
            }
        }

        // Draw opposite scans
        // graph->setColor(_colors["GREEN"], 32);
        // for (int x = 0; x < WINDOW_SIZE.x; x += _g.cellSize) {
        //     int xx = x + sin(x + (tick/6) * 0.1) * _g.cellSize;
        //     graph->line(Vec2i(xx, 0), Vec2i(xx, WINDOW_SIZE.y));
        // }

        // Draw scanlines
        graph->setColor(_colors["GREEN"],32);
        for (int y = 0; y < WINDOW_SIZE.y; y += _g.cellSize) {
            int yy = y + sin(y + (_g.getTick()/6) * 0.1) * _g.cellSize;
            graph->line(Vec2i(0, yy), Vec2i(WINDOW_SIZE.x, yy));
        }
    }
};

class MainMenu : public Entity {
public:
    EntityManager em;
    BtnMainMenu btnResume;
    BtnMainMenu btnRestart;
    BtnMainMenu btnNew;
    BtnMainMenu btnSettings;
    BtnMainMenu btnExit;
    Sprite sprBg;
    MainMenu() : Entity() {
        tag = "menu";

        btnResume.pos = Vec2i(20, 60);
        btnResume.text = "Resume";
        em.addEntity(&btnResume);

        btnRestart.pos = Vec2i(20, 60 + _g.fontSize * 1.1f);
        btnRestart.text = "Restart";
        em.addEntity(&btnRestart);

        btnNew.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 2);
        btnNew.text = "New";
        em.addEntity(&btnNew);

        btnSettings.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 3);
        btnSettings.text = "Settings";
        em.addEntity(&btnSettings);

        btnExit.pos = Vec2i(20, 60 + _g.fontSize * 1.1f * 4);
        btnExit.text = "Exit";
        em.addEntity(&btnExit);

        sprBg = Sprite(Vec2i(176, 176), Vec2i(96, 96), Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));
        
    }
    ~MainMenu() {}
    void process() override {
        if (!_g.getShowMainMenu()) {
            Sounds::menuTrack.volDown(4, 8);
            // Sounds::menuTrack.stop();
            return;
        }
        // Sounds::menuTrack.volume = 255;
        if (!Sounds::menuTrack.isPlaying()) Sounds::menuTrack.volume = 0;
        Sounds::menuTrack.volUp(4, 128);
        Sounds::menuTrack.play(true);
        em.process();
        em.checkMouse();
        if (btnResume.isClicked()) {
            _g.toggleMainMenu();
        }
        if (btnRestart.isClicked()) {
            _g.setReset(true);
            _g.setShowMainMenu(false);
        }
        if (btnNew.isClicked()) {
            int puzMax = Util::maxUnsignedInt(Util::maxUnsignedInt(_g.getPuzzleBits())); 
            DBG("Puzzle max: " + std::to_string(puzMax));
            _g.setPuzzleNum(rand() % puzMax); 
            DBG("New puzzle: " + std::to_string(_g.getPuzzleNum()));
            _g.setReset(true);
            _g.toggleMainMenu();
        }
        if (btnExit.isClicked()) {
            _g.setQuit(true);
        }
    }
    void render(Graphics* graph) override {
        if (!_g.getShowMainMenu()) return;
        graph->setColor(_colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        graph->setColor(_colors["WHITE"]);
        graph->text("ESOMachina", Vec2i(20, 20), _g.fontSize);
        em.render(graph);
        sprBg.render(graph, WINDOW_SIZE - Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));
    }
};

class TopBar : public Entity {
public:
    BtnTopMenu btnHome;
    BtnTopMenu btnFile;
    BtnTopMenu btnTools;
    BtnTopMenu btnEdit;
    // File menu
    BtnTopMenu btnReset;
    BtnTopMenu btnSave;
    BtnTopMenu btnLoad;
    // Tools menu
    BtnTopMenu btnTests;
    //
    EntityManager em;
    int height;
    std::string activeTopMenu;
    TopBar() : Entity() {
        tag = "topBar";
        height = _g.vu(0.5f);

        btnHome.isHomeBtn = true;
        btnHome.onClick = []() { _g.toggleMainMenu(); };
        btnHome.show = true;
        btnHome.pos = Vec2i(_g.vu(0.25f), 4);
        btnHome.setCollider(Vec2i(_g.vu(0.25f), height));
        em.addEntity(&btnHome);

        btnFile.onClick = [this]() {
            activeTopMenu = "btnFile";
            btnReset.show = true;
            btnSave.show = true;
            btnLoad.show = true;
        };
        btnFile.tag = "btnFile";
        btnFile.show = true;
        btnFile.text = "FILE";
        btnFile.pos = Vec2i(_g.vu(0.15f) + _g.vu(0.75f), 4);
        em.addEntity(&btnFile);

        btnTools.onClick = [this]() {
            activeTopMenu = "btnTools";
            btnTests.show = true;
        };
        btnTools.tag = "btnTools";
        btnTools.show = true;
        btnTools.text = "TOOL";
        btnTools.pos = Vec2i(_g.vu(0.15f) + _g.vu(2.25f), 4);
        em.addEntity(&btnTools);

        btnEdit.tag = "btnEdit";
        btnEdit.show = true;
        btnEdit.text = "EDIT";
        btnEdit.pos = Vec2i(_g.vu(0.15f) + _g.vu(3.75f), 4);
        em.addEntity(&btnEdit);

        // File menu
        btnReset.onClick = []() { _g.setReset(true); };
        btnReset.show = false;
        btnReset.text = "RESET";
        btnReset.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(0.5f));
        em.addEntity(&btnReset);

        btnSave.onClick = []() { DBG("Save"); };
        btnSave.show = false;
        btnSave.text = "SAVE";
        btnSave.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(1));
        em.addEntity(&btnSave);

        btnLoad.onClick = []() { DBG("Load"); };
        btnLoad.show = false;
        btnLoad.text = "LOAD";
        btnLoad.pos = Vec2i(btnFile.pos.x, btnFile.pos.y + _g.vu(1.5f));
        em.addEntity(&btnLoad);

        // Tools menu
        btnTests.onClick = []() { DBG("TESTS"); _g.toggleTests(); };
        btnTests.show = false;
        btnTests.text = "TESTS";
        btnTests.pos = Vec2i(btnTools.pos.x, btnTools.pos.y + _g.vu(0.5f));
        em.addEntity(&btnTests);

    }
    ~TopBar() {}
    void process() override {
        // Clear if nothing is clicked
        if (_input.mouseKeyDown(SDL_BUTTON_LEFT)) {
            activeTopMenu = "";
        }
        em.checkMouse();
        em.process();
        if (activeTopMenu != "btnFile") {
            btnReset.show = false;
            btnSave.show = false;
            btnLoad.show = false;
        }
        if (activeTopMenu != "btnTools") {
            btnTests.show = false;
        }
    }
    void render(Graphics* graph) override {
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

class Cursor : public Entity {
public:
    Sprite spr;
    Cursor() : Entity() {
        tag = "cursor";
        pos = Vec2i(0, 0);
        spr = Sprite(Vec2i(0, 64), Vec2i(16, 16), Vec2i(32,32));
        Util::hideMouse();
    }
    ~Cursor() {}
    void process() override {
        pos = _input.mousePos();
    }
    void render(Graphics* graph) override {
        spr.render(graph, pos);
    }
};

class App : public Imp::Main { 
public:
    Grid grid;
    TestScreen testScreen;
    Cursor cursor;
    MainMenu mainMenu;
    TopBar topBar;
    BottomBar bottomBar;
    App() : Imp::Main("EsoMachina (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png") { 
        clearColor = Color(_colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&topBar);
        entityMan.addEntity(&testScreen);
        entityMan.addEntity(&mainMenu);
        entityMan.addEntity(&cursor);
    }
    ~App() {}
    void process() override {
        _input.poll();

        // Pause
        if (_input.keyDown(SDLK_ESCAPE)) {
            _g.toggleMainMenu();
        }

        if (_g.getReset()) {
            _g.setReset(false);
            grid.reset();
            testScreen.reset();
        }
        if (_g.getQuit()) {
            exit(0);
        }

        _g.incTick();
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
};