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

std::map<std::string, Color> colors = {
    {"BG", Color(20,25,20)},
    {"BG2", Color(45,50,45)},
    {"BG3", Color(55,55,60)},
    {"GREEN", Color(94,180,93)},
    {"YELLOW", Color(238,198,75)},
    {"RED", Color(255,128,128)},
    {"GRAY", Color(155,155,155)},
    {"WHITE", Color(255,255,255)},
};

enum CellType {
    CT_VOID = -1,
    CT_EMPTY = 0,
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
    Cell() {
        type = CT_EMPTY;
        parenLeft = false;
        parenRight = false;
    }
    ~Cell() {}
    void set(CellType type) {
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
            case CT_EMPTY: return "VOID";
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
    Sound toggleMenu = Sound("3.wav");
    Sound toggleTests = Sound("4.wav");
};

class Globals {
public:
    const int fontSize = WINDOW_SIZE.x / 32;
    const int cellSize = WINDOW_SIZE.x / 16;
    //
    std::string codeString = "";
    HelpItem* helpItem = nullptr;
    bool hasCodeErr = false;
    bool showMenu = false;
    bool reset = false;
    int tick = 0;
    CellType activeTile = CT_EMPTY;
    int puzzleBits = 3;
    int puzzleNum = 1;
    //
    Vec2i bottomBarSize = Vec2i(WINDOW_SIZE.x, cellSize * 3);
    Vec2i bottomBarPos = Vec2i(0, WINDOW_SIZE.y - bottomBarSize.y);
    //
    void setActiveTile(CellType type) {
        if (type == CT_IND && puzzleBits < 4) {
            return;
        }
        Sounds::changeTile.play();
        activeTile = type;
    }
    void toggleMenu() {
        Sounds::toggleMenu.play();
        showMenu = !showMenu;
    }
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
    std::map <CellType, Sprite*> cellMap = {
        {CT_EMPTY, &clear},
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

class TestCase {
public:
    std::vector<bool> inputs;
    bool output;
    bool hasError;
    bool lastCheck;
    TestCase() {
        inputs.resize(4);
        output = false;
        hasError = false;
        lastCheck = false;
        //dbg
        // randomize();
        s7 = s7_init();
    }
    ~TestCase() {}
    void set(int index, int bits, int puzzleNum) {
        inputs.resize(bits);
        for (int i = 0; i < bits; i++) {
            inputs[i] = Util::intToBits(index, bits)[i];
        }
        output = Util::intToBits(puzzleNum, Util::maxUnsignedInt(bits))[index];
    }
    void randomize() {
        for (int i = 0; i < 4; i++) {
            inputs[i] = rand() % 2;
        }
        output = rand() % 2;
    }
    bool check(std::string code) {
        if (code == "") {
            lastCheck = false;
            hasError = false;
            return lastCheck;
        };
        hasError = false;
        code = Util::toLowercase(code);
        code = Util::strReplace(code, "_", " ");
        // code = "(display \"hello\"  )";
        std::string pre = "";
        pre += "(define (xor a b) (or (and a (not b)) (and (not a) b)))";
        pre += "(define (nand a b) (not (and a b)))";
        pre += "(define (nor a b) (not (or a b)))";
        pre += "(define (xnor a b) (not (xor a b)))";
        pre += "(define a " + boolScheme(inputs[0]) + ")";
        pre += "(define b " + boolScheme(inputs[1]) + ")";
        pre += "(define c " + boolScheme(inputs[2]) + ")";
        pre += "(define d " + boolScheme(inputs[3]) + ")";
        // pre += "(define output " + boolScheme(output) + ")";
        code = "(begin " + pre + code + ")";
        std::string eval = evalScheme(code);
        if (eval != "#t" && eval != "#f") {
            hasError = true;
            lastCheck = false;
            return lastCheck;
        }
        if (eval == boolScheme(output)) lastCheck = true;
        else lastCheck = false;
        return lastCheck;
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
    Input input;
    bool show;
    int testFails;
    std::vector<TestCase> tests;
    std::string codeStringOld;
    Sprite sprTrue;
    Sprite sprFalse;
    Sprite sprPass;
    Sprite sprFail;
    TestScreen() : Entity() {
        tag = "testScreen";
        show = true;
        tests.resize(Util::maxUnsignedInt(_g.puzzleBits));
        DBG("Max unsigned int: " + std::to_string(Util::maxUnsignedInt(_g.puzzleBits)));

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

        // not dry - reset
        for (int i = 0; i < tests.size(); i++) {
            tests[i].set(i, _g.puzzleBits, _g.puzzleNum);
        }
    }
    ~TestScreen() {}
    void reset() {
        for (int i = 0; i < tests.size(); i++) {
           tests[i].set(i, _g.puzzleBits, _g.puzzleNum); 
        }
    }
    void process() {
        if (_g.showMenu) return;
        std::string cs = Util::strReplace(Util::strReplace(_g.codeString, " ", ""), "_", "");
        std::string cso = Util::strReplace(Util::strReplace(codeStringOld, " ", ""), "_", "");
        if (cs != cso) {
            _g.hasCodeErr = false;
            DBG("Retesting code");
            DBG(cs);
            testFails = 0;
            for (TestCase& test : tests) {
                if (!test.check(_g.codeString)){
                    testFails++;
                }
            }
            codeStringOld = _g.codeString;
        }
        if(input.keyDown(SDLK_SPACE)) {
            show = !show;
            Sounds::toggleTests.play();
        }
    }
    void render(Graphics* graph) {
        if (_g.showMenu) return;
        int testWinWidth = _g.fontSize * 8;
        int testWinX = WINDOW_SIZE.x - (show ? testWinWidth : _g.cellSize / 4);
        //graph->setColor(colors["BG3"]);
        //graph->rect(Vec2i(testWinX, 0), Vec2i(testWinWidth, _g.bottomBarPos.y - _g.cellSize), true);
        int y = _g.cellSize * 2;
        int ce = _g.cellSize / 2;
        for (TestCase test : tests) {
            if (test.lastCheck) sprPass.render(graph, Vec2i(testWinX - 4, y));
            else sprFail.render(graph, Vec2i(testWinX - 4 - (_g.tick/8 % 6), y));
            int x = ce;
            for (bool input : test.inputs) {
                if (input) sprTrue.render(graph, Vec2i(testWinX + x, y));
                else sprFalse.render(graph, Vec2i(testWinX + x, y));
                x += ce;
            }
            if (test.output) {
                sprTrue.render(graph, Vec2i(testWinX + ce + x, y));
            }
            else {
                sprFalse.render(graph, Vec2i(testWinX + ce + x, y));
            }
            if (test.hasError) _g.hasCodeErr = true;
            y += ce + (ce/2);
        }
        if (_g.hasCodeErr) {
            graph->setColor(colors["YELLOW"]);
            graph->text("!! ERROR", Vec2i(testWinX + 16, _g.cellSize - (_g.tick/4 % 8)), _g.fontSize);
        }
        else {
            graph->setColor(colors["GRAY"]);
            graph->text("// TESTS", Vec2i(testWinX + 16, _g.cellSize), _g.fontSize);
        }
        if (testFails == 0) {
            graph->setColor(colors["GREEN"]);
            graph->text("PASSED", Vec2i(_g.cellSize/4,_g.cellSize/4), _g.fontSize * 4);
        }
    }
};

class Btn : public Entity {
public:
    Input input;
    Vec2i size;
    std::string text;
    Uint8 state;
    bool center;
    Btn() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(200, _g.fontSize * 1.1f);
        setCollider(size);
    }
    ~Btn() {}
    bool isClicked() {
        return state == 2;
    }
    void process() override {}
    void render(Graphics* graph) override {
        Color* c = &colors["GRAY"];
        switch (state) {
            case 0: c = &colors["GRAY"]; break;
            case 1: c = &colors["YELLOW"]; break;
            case 2: c = &colors["GREEN"]; break;
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
        if (over) {
            state = 1;
            if (input.mouseKey(SDL_BUTTON_LEFT)) {
                state = 2;
            }
            return;
        }
        state = 0;
    }
};

class TileBtn : public Entity {
public:
    Input input;
    CellType type;
    Uint8 state;
    Uint8 oldState;
    Sound sndTick;
    TileBtn() : Entity() {
        state = 99; // 99 prevents play on start
        oldState = 99;
        tag = "tileBtn";
        type = CT_EMPTY;
        setCollider(Vec2i(_g.cellSize, _g.cellSize));
        sndTick.set("1.wav");
    }
    ~TileBtn() {}
    void render(Graphics* graph) override {
        if (type == CT_IND && _g.puzzleBits < 4) {
            return;
        }
        Vec2i post = pos.copy();
         
        if (_g.activeTile == type) {
            post += Vec2i(0, -_g.cellSize / 4);
        }
        else if (state > 0) {
            post += Vec2i(0, -_g.cellSize / 8);
        }
        graph->setColor(colors["GREEN"]);
        graph->rect(pos + Vec2i(_g.cellSize / 4, _g.cellSize / 8), Vec2i(_g.cellSize - (_g.cellSize / 2), _g.cellSize - _g.cellSize / 4));
        CellSprites::baseTile.render(graph, post);
        CellSprites::cellMap[type]->render(graph, post);
    }
    void onMouse(bool over) override {
        oldState = state;
        if (over) {
            state = 1;
            if (input.mouseKeyDown(SDL_BUTTON_LEFT)) {
                state = 2;
                _g.setActiveTile(type);
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
    TileBtn btnInA;
    TileBtn btnInB;
    TileBtn btnInC;
    TileBtn btnInD;
    TileBtn btnClear;
    TileBtn btnBlank;
    TileBtn btnAnd;
    TileBtn btnOr;
    TileBtn btnNot;
    TileBtn btnXor;
    TileBtn btnNand;
    TileBtn btnNor;
    TileBtn btnXnor;
    BottomBar() : Entity() {
        tag = "bottomBar";
        btnClear.type = CT_EMPTY;
        btnBlank.type = CT_BLANK;
        btnInA.type = CT_INA;
        btnInB.type = CT_INB;
        btnInC.type = CT_INC;
        btnInD.type = CT_IND;
        btnAnd.type = CT_AND;
        btnOr.type = CT_OR;
        btnNot.type = CT_NOT;
        btnXor.type = CT_XOR;
        btnNand.type = CT_NAND;
        btnNor.type = CT_NOR;
        btnXnor.type = CT_XNOR;
        
        btnClear.pos = Vec2i(1.5f * _g.cellSize, _g.bottomBarPos.y);
        btnInA.pos = Vec2i(2.5f * _g.cellSize, _g.bottomBarPos.y);
        btnInB.pos = Vec2i(3.5f * _g.cellSize, _g.bottomBarPos.y);
        btnInC.pos = Vec2i(4.5f * _g.cellSize, _g.bottomBarPos.y);
        btnInD.pos = Vec2i(5.5f * _g.cellSize, _g.bottomBarPos.y);
        btnBlank.pos = Vec2i(6.5f * _g.cellSize, _g.bottomBarPos.y);
        btnAnd.pos = Vec2i(7.5f * _g.cellSize, _g.bottomBarPos.y);
        btnOr.pos = Vec2i(8.5f * _g.cellSize, _g.bottomBarPos.y);
        btnNot.pos = Vec2i(9.5f * _g.cellSize, _g.bottomBarPos.y);
        btnXor.pos = Vec2i(10.5f * _g.cellSize, _g.bottomBarPos.y);
        btnNand.pos = Vec2i(11.5f * _g.cellSize, _g.bottomBarPos.y);
        btnNor.pos = Vec2i(12.5f * _g.cellSize, _g.bottomBarPos.y);
        btnXnor.pos = Vec2i(13.5f * _g.cellSize, _g.bottomBarPos.y);

        em.addEntity(&btnClear);
        em.addEntity(&btnInA);
        em.addEntity(&btnInB);
        em.addEntity(&btnInC);
        em.addEntity(&btnInD);
        em.addEntity(&btnBlank);
        em.addEntity(&btnAnd);
        em.addEntity(&btnOr);
        em.addEntity(&btnNot);
        em.addEntity(&btnXor);
        em.addEntity(&btnNand);
        em.addEntity(&btnNor);
        em.addEntity(&btnXnor);
    }
    ~BottomBar() {}
    void process() override {
        if (_g.showMenu) return;
        em.checkMouse();
        em.process();
        if (btnClear.state >= 1){
            _g.helpItem = &helpItems["VOID"];
        }
        else if (btnInA.state >= 1){
            _g.helpItem = &helpItems["A"];
        }
        else if (btnInB.state >= 1){
            _g.helpItem = &helpItems["B"];
        }
        else if (btnInC.state >= 1){
            _g.helpItem = &helpItems["C"];
        }
        else if (btnInD.state >= 1){
            _g.helpItem = &helpItems["D"];
        }
        else if (btnBlank.state >= 1){
            _g.helpItem = &helpItems["BLANK"];
        }
        else if (btnAnd.state >= 1){
            _g.helpItem = &helpItems["AND"];
        }
        else if (btnOr.state >= 1){
            _g.helpItem = &helpItems["OR"];
        }
        else if (btnNot.state >= 1){
            _g.helpItem = &helpItems["NOT"];
        }
        else if (btnXor.state >= 1){
            _g.helpItem = &helpItems["XOR"];
        }
        else if (btnNand.state >= 1){
            _g.helpItem = &helpItems["NAND"];
        }
        else if (btnNor.state >= 1){
            _g.helpItem = &helpItems["NOR"];
        }
        else if (btnXnor.state >= 1){
            _g.helpItem = &helpItems["XNOR"];
        }
        else {
            _g.helpItem = nullptr;
        }
    }
    void render(Graphics* graph) override {
        if (_g.showMenu) return;
        graph->setColor(colors["BG3"]);
        graph->rect(_g.bottomBarPos, _g.bottomBarSize);
        em.render(graph);

        int padX = _g.cellSize;
        int padY = _g.cellSize / 4;
        graph->setColor(colors["BG"]);
        graph->rect(Vec2i(0, _g.bottomBarPos.y + _g.cellSize), Vec2i(WINDOW_SIZE.x, _g.bottomBarSize.y - _g.cellSize), true);
        if (_g.helpItem != nullptr) {
            graph->setColor(colors["GREEN"]);
            graph->text("Name: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            int w = graph->textWidth("Name: ", _g.fontSize);
            graph->setColor(200, 200, 200);
            graph->text(_g.helpItem->title, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            w += graph->textWidth(_g.helpItem->title, _g.fontSize);
            graph->setColor(colors["GRAY"]);
            graph->text(" (" + _g.helpItem->key + ")", Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);

            graph->setColor(colors["GREEN"]);
            graph->text("Info: ", Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), _g.fontSize);
            w = graph->textWidth("Info: ", _g.fontSize);
            graph->setColor(200, 200, 200);
            graph->text(_g.helpItem->desc, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + _g.fontSize + padY), _g.fontSize);
        }
        else if (_g.codeString.length() > 0) {
            std::string codePre = _g.hasCodeErr ? "!!" :">> ";
            graph->setColor(_g.hasCodeErr ? colors["YELLOW"] : colors["GREEN"]);
            graph->text(codePre, Vec2i(padX, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            int w = graph->textWidth(codePre, _g.fontSize);
            // Split the string by _
            std::vector<std::string> split = Util::splitString(_g.codeString, "_");
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
                graph->setColor(colors["GRAY"]);
                graph->text(p1, Vec2i(padX + w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
            if (p2.length() > 0) {
                graph->setColor(colors["GREEN"]);
                graph->text(p2, Vec2i(padX + w + p1w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
            if (p3.length() > 0) {
                graph->setColor(colors["GRAY"]);
                graph->text(p3, Vec2i(padX + w + p1w + p2w, _g.bottomBarPos.y + _g.cellSize + padY), _g.fontSize);
            }
        }
    }
};

class Grid : public Entity {
public:
    Input input;
    Vec2i mousePos;
    Vec2i mousePosCell;
    Uint32 mouseBtn;
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
                cells[x][y].set(CT_EMPTY);
            }
        }

        sndTick.set("1.wav");
        // sndTick.volume = 32;
        sndAdd.set("14.wav");
        sndRemove.set("rock.ogg");
        sndParen.set("5.wav");  

        highlightCellTypeStr = "EMPTY";
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
                cells[x][y].set(CT_EMPTY);
            }
        }
    }
    void process() override {
        if (_g.showMenu) return;
        mousePos = input.mousePos();
        mouseBtn = input.getMouseBtn();
        mousePosCell = mousePos / _g.cellSize;
        // Update lastMouse array with the most recent mousePosCell value
        if (_g.tick % 2 == 0) {
            for (int i = 7; i > 0; i--) {
                lastMouse[i] = lastMouse[i - 1];
            }
            lastMouse[0] = mousePosCell;
        }
        int x = mousePosCell.x;
        int y = mousePosCell.y;
        CellType cellType = cells[x][y].get();
        bool isTile = cellType != CT_VOID && cellType != CT_EMPTY;
        highlightCellTypeStr = Cell::typeToString(cellType);
        
        if (input.keyDown(SDLK_q)) _g.setActiveTile(CT_EMPTY);
        if (input.keyDown(SDLK_BACKSPACE)) _g.setActiveTile(CT_EMPTY);
        if (input.keyDown(SDLK_w)) _g.setActiveTile(CT_BLANK);
        if (input.keyDown(SDLK_e)) _g.setActiveTile(CT_AND);
        if (input.keyDown(SDLK_r)) _g.setActiveTile(CT_OR);
        if (input.keyDown(SDLK_t)) _g.setActiveTile(CT_NOT);
        if (input.keyDown(SDLK_a)) _g.setActiveTile(CT_INA);
        if (input.keyDown(SDLK_s)) _g.setActiveTile(CT_INB);
        if (input.keyDown(SDLK_d)) _g.setActiveTile(CT_INC);
        if (input.keyDown(SDLK_f)) _g.setActiveTile(CT_IND);
        if (input.keyDown(SDLK_y)) _g.setActiveTile(CT_XOR);
        if (input.keyDown(SDLK_u)) _g.setActiveTile(CT_NAND);
        if (input.keyDown(SDLK_i)) _g.setActiveTile(CT_NOR);
        if (input.keyDown(SDLK_o)) _g.setActiveTile(CT_XNOR);

        if (input.mousePos().y < _g.bottomBarPos.y) {
            if (lastMouse[0] != lastMouse[1]) {
                int relMouse = ((float)mousePosCell.x / gridSize.x) * 255;
                sndTick.setPan(255 - relMouse, relMouse);
                sndTick.play();
            }

            CellType newCell = CT_VOID;
            if (input.mouseKeyDown(SDL_BUTTON_LEFT)) {
                newCell = _g.activeTile;
            }
            if (newCell != CT_VOID) {
                DBG("New Cell: " + std::to_string(newCell));
                if (newCell == CT_EMPTY) {
                    if (isTile) sndRemove.play();
                }
                else sndAdd.play();
                cells[x][y].set(newCell);
            }
            // Toggle connectors
            if (input.keyDown(SDLK_TAB) || input.mouseKeyDown(SDL_BUTTON_RIGHT)) {
                cells[x][y].cycleParens();
                if (isTile) sndParen.play();
            }
        }

        // Generate code string
        std::string codeStringOld = _g.codeString;
        _g.codeString = "";
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_EMPTY) {
                    bool isHovered = mousePosCell.x == x && mousePosCell.y == y;
                    if (isHovered) _g.codeString += "_";
                    if (cells[x][y].parenLeft) _g.codeString += "(";
                    if (cellType != CT_BLANK) _g.codeString += Cell::typeToString(cellType);
                    if (cells[x][y].parenRight) _g.codeString += ")";
                    if (isHovered) _g.codeString += "_";
                    else if (cellType != CT_BLANK) _g.codeString += " ";
                }
            }
        }
    }
    void render(Graphics* graph) override {
        if (_g.showMenu) return;
        // Drag grid bg
        graph->setColor(colors["BG2"]);
        for (int x = 0; x < WINDOW_SIZE.x; x += _g.cellSize) {
            graph->line(Vec2i(x, 0), Vec2i(x, WINDOW_SIZE.y));
        }
        for (int y = 0; y < WINDOW_SIZE.y; y += _g.cellSize) {
            graph->line(Vec2i(0, y), Vec2i(WINDOW_SIZE.x, y));
        }
        // Draw border
        int borderWidth = _g.hasCodeErr ? _g.cellSize/16 : _g.cellSize/4;
        graph->setColor(_g.hasCodeErr ? colors["YELLOW"] : colors["BG2"], _g.hasCodeErr ? 128 : 255);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, _g.bottomBarPos.y - borderWidth), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        graph->rect(Vec2i(WINDOW_SIZE.x - borderWidth, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        
        // Draw mouse trail
        for (int i = 0; i < 8; i++) {
            Vec2i pos = lastMouse[i];
            Vec2i cellPos = pos * _g.cellSize;
            Color c = colors["GREEN"];
            graph->setColor(c.r, c.g - (i * 8), c.b);
            graph->rect(cellPos, Vec2i(_g.cellSize, _g.cellSize), false);
        }

        // Draw cells
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_VOID && cellType != CT_EMPTY) {
                    CellSprites::baseTile.render(graph, Vec2i(_g.cellSize * x, _g.cellSize * y));
                    Sprite* spr = nullptr;
                    spr = CellSprites::cellMap[cellType];
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
                }
            }
        }

        // Highlight cell on mouse hover
        Vec2i cell = mousePos / _g.cellSize * _g.cellSize;
        graph->setColor(colors["WHITE"]);
        graph->rect(cell, Vec2i(_g.cellSize, _g.cellSize), false);

        // Draw static noise
        if (_g.tick % 2 == 0) {
            int staticSize = _g.cellSize / 32;
            graph->setColor(colors["YELLOW"], 64);
            for (int i = 0; i < WINDOW_SIZE.size2d()/(staticSize * (_g.hasCodeErr ? 128 : 1024)); i++) {
                int x = rand() % WINDOW_SIZE.x;
                int y = rand() % WINDOW_SIZE.y;
                graph->rect(Vec2i(x, y), Vec2i(staticSize, staticSize));
            }
        }

        // Draw opposite scans
        // graph->setColor(colors["GREEN"], 32);
        // for (int x = 0; x < WINDOW_SIZE.x; x += _g.cellSize) {
        //     int xx = x + sin(x + (tick/6) * 0.1) * _g.cellSize;
        //     graph->line(Vec2i(xx, 0), Vec2i(xx, WINDOW_SIZE.y));
        // }

        // Draw scanlines
        graph->setColor(colors["GREEN"],32);
        for (int y = 0; y < WINDOW_SIZE.y; y += _g.cellSize) {
            int yy = y + sin(y + (_g.tick/6) * 0.1) * _g.cellSize;
            graph->line(Vec2i(0, yy), Vec2i(WINDOW_SIZE.x, yy));
        }
    }
};

class Menu : public Entity {
public:
    EntityManager em;
    Btn btnResume;
    Btn btnRestart;
    Btn btnNew;
    Btn btnSettings;
    Btn btnExit;
    Sprite sprBg;
    Menu() : Entity() {
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
    ~Menu() {}
    void process() override {
        if (!_g.showMenu) {
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
            _g.toggleMenu();
        }
        if (btnRestart.isClicked()) {
            _g.reset = true;
            _g.showMenu = false;
        }
        if (btnNew.isClicked()) {
            int puzMax = Util::maxUnsignedInt(Util::maxUnsignedInt(_g.puzzleBits)); 
            DBG("Puzzle max: " + std::to_string(puzMax));
            _g.puzzleNum = rand() % puzMax; 
            DBG("New puzzle: " + std::to_string(_g.puzzleNum));
            _g.reset = true;
            _g.toggleMenu();
        }
    }
    void render(Graphics* graph) override {
        if (!_g.showMenu) return;
        graph->setColor(colors["BG"]);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        graph->setColor(colors["WHITE"]);
        graph->text("ESOMachina", Vec2i(20, 20), _g.fontSize);
        em.render(graph);
        sprBg.render(graph, WINDOW_SIZE - Vec2i(WINDOW_SIZE.x/2, WINDOW_SIZE.x/2));
    }
};

class Cursor : public Entity {
public:
    Input input;
    Sprite spr;
    Cursor() : Entity() {
        tag = "cursor";
        pos = Vec2i(0, 0);
        spr = Sprite(Vec2i(0, 64), Vec2i(16, 16), Vec2i(32,32));
        Util::hideMouse();
    }
    ~Cursor() {}
    void process() override {
        pos = input.mousePos();
    }
    void render(Graphics* graph) override {
        spr.render(graph, pos);
    }
};

class App : public Imp::Main { 
public:
    Input input;
    Grid grid;
    TestScreen testScreen;
    Cursor cursor;
    Menu menu;
    BottomBar bottomBar;
    App() : Imp::Main("EsoMachina (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png") { 
        clearColor = Color(colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&bottomBar);
        entityMan.addEntity(&testScreen);
        entityMan.addEntity(&menu);
        entityMan.addEntity(&cursor);
    }
    ~App() {}
    void process() override {
        // Pause
        if (input.keyDown(SDLK_ESCAPE)) {
            _g.toggleMenu();
        }

        if (_g.reset) {
            _g.reset = false;
            grid.reset();
            testScreen.reset();
        }

        _g.tick++;
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
};