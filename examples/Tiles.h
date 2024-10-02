// ESOMachina - A simple tile-based visual programming game
#include <map>

#include "../lib/s7/s7.h"

#include "../src/Imp.h"
using namespace Imp;

#define WINDOW_SIZE Vec2i(1024, 768)
#define FONT_SIZE WINDOW_SIZE.x / 32

std::map<std::string, Color> colors = {
    {"BG", Color(20,20,20)},
    {"BG2", Color(45,45,45)},
    {"BG3", Color(55,55,55)},
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
        return "????";
    }
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
        randomize();
        s7 = s7_init();
    }
    ~TestCase() {}
    void addInput(bool input) {
        inputs.push_back(input);
    }
    void setOutput(bool output) {
        this->output = output;
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

class Btn : public Entity {
public:
    Vec2i size;
    std::string text;
    Uint8 state;
    bool center;
    Btn() : Entity() {
        tag = "btn";
        state = 0;
        center = false;
        text = "Button";
        size = Vec2i(200, FONT_SIZE * 1.1f);
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
            int textWidth = graph->textWidth(textMod, FONT_SIZE);
            Vec2i textPos = pos + Vec2i((size.x - textWidth) / 2, (size.y - FONT_SIZE) / 2);
            graph->text(textMod, textPos, FONT_SIZE);
        }
        else {
            graph->text(textMod, pos, FONT_SIZE);
        }
    }
    void onMouse(bool over) override {
        if (over) {
            state = 1;
            if (Input::mouseKey(SDL_BUTTON_LEFT)) {
                state = 2;
            }
            return;
        }
        state = 0;
    }
};

class Menu : public Entity {
public:
    bool show;
    EntityManager em;
    Btn btnResume;
    Btn btnRestart;
    Btn btnNew;
    Btn btnSettings;
    Btn btnExit;
    Sprite sprBg;
    Menu() : Entity() {
        tag = "menu";
        show = false;

        btnResume.pos = Vec2i(20, 60);
        btnResume.text = "Resume";
        em.addEntity(&btnResume);

        btnRestart.pos = Vec2i(20, 60 + FONT_SIZE * 1.1f);
        btnRestart.text = "Restart";
        em.addEntity(&btnRestart);

        btnNew.pos = Vec2i(20, 60 + FONT_SIZE * 1.1f * 2);
        btnNew.text = "New";
        em.addEntity(&btnNew);

        btnSettings.pos = Vec2i(20, 60 + FONT_SIZE * 1.1f * 3);
        btnSettings.text = "Settings";
        em.addEntity(&btnSettings);

        btnExit.pos = Vec2i(20, 60 + FONT_SIZE * 1.1f * 4);
        btnExit.text = "Exit";
        em.addEntity(&btnExit);

        sprBg = Sprite(Vec2i(176, 176), Vec2i(96, 96), Vec2i(256, 256));
        
    }
    ~Menu() {}
    void process() override {
        if (!show) return;
        em.process();
        em.checkMouse();
        if (btnResume.isClicked()) {
            show = false;
        }
        if (btnRestart.isClicked()) {
            show = false;
        }
    }
    void render(Graphics* graph) override {
        if (!show) return;
        graph->setColor(colors["BG1"], 128);
        graph->rect(Vec2i(0, 0), WINDOW_SIZE);
        graph->setColor(colors["WHITE"]);
        graph->text("ESOMachina", Vec2i(20, 20), FONT_SIZE);
        em.render(graph);
        sprBg.render(graph, Vec2i(20, 60));
    }
};

class Grid : public Entity {
public:
    bool paused;
    int tick;
    bool showTests;
    std::vector<TestCase> tests;
    bool codeErr;
    Vec2i mousePos;
    Vec2i mousePosCell;
    Uint32 mouseBtn;
    int cellSize;
    Vec2i gridSize;
    std::string highlightCellTypeStr;
    std::string codeString;
    std::string helpString;
    std::map<CellType, std::string> helpStrings;
    std::vector<std::vector<Cell>> cells;
    Vec2i lastMouse[8];
    Vec2i bottomTilePos[13];
    int bottomSize;
    int bottomY;
    std::map<CellType, Sprite*> cellSprites;
    Sprite sprBaseTile;
    Sprite sprBlankTile;
    Sprite sprAndTile;
    Sprite sprOrTile;
    Sprite sprNotTile;
    Sprite sprXOrTile;
    Sprite sprNAndTile;
    Sprite sprNOrTile;
    Sprite sprXNorTile;
    Sprite sprParenR;
    Sprite sprInputA;
    Sprite sprInputB;
    Sprite sprInputC;
    Sprite sprInputD;
    Sprite sprVoid;
    Sprite sprTrue;
    Sprite sprFalse;
    Sprite sprPass;
    Sprite sprFail;
    Sound sndTick;
    Sound sndAdd;
    Sound sndRemove;
    Sound sndParen;
    Sound sndTest;
    Grid() : Entity() {
        tag = "grid";
        cellSize = WINDOW_SIZE.x / 16;
        gridSize = WINDOW_SIZE / cellSize;
        // Resize the 2d array to the grid size
        cells.resize(gridSize.x);
        // Fill cells with 0
        for (int x = 0; x < gridSize.x; x++) {
            cells[x].resize(gridSize.y);
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_EMPTY);
            }
        }
        // Sprite tiles
        sprBaseTile = Sprite(Vec2i(0, 0), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprBlankTile = Sprite(Vec2i(0, 32), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprAndTile  = Sprite(Vec2i(0, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprOrTile = Sprite(Vec2i(16, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprNotTile = Sprite(Vec2i(32, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprXOrTile = Sprite(Vec2i(48, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprNAndTile = Sprite(Vec2i(64, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprNOrTile = Sprite(Vec2i(80, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprXNorTile = Sprite(Vec2i(96, 16), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprParenR = Sprite(Vec2i(32, 0), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprInputA = Sprite(Vec2i(0, 48), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprInputB = Sprite(Vec2i(16, 48), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprInputC = Sprite(Vec2i(32, 48), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprInputD = Sprite(Vec2i(48, 48), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        sprVoid = Sprite(Vec2i(16, 32), Vec2i(16, 16), Vec2i(cellSize, cellSize));
        
        // Sprite tests
        sprTrue = Sprite(Vec2i(0, 80), Vec2i(16, 16), Vec2i(cellSize/2, cellSize/2));
        std::vector<Vec2i> trueFrames = {
            Vec2i(0, 80), 
            Vec2i(16, 80), 
            Vec2i(32, 80), 
            Vec2i(48, 80), 
            Vec2i(64, 80), 
        };
        sprTrue.setAnimation(trueFrames, 4, true);
        sprFalse = Sprite(Vec2i(0, 96), Vec2i(16, 16), Vec2i(cellSize/2, cellSize/2));
        std::vector<Vec2i> falseFrames = {
            Vec2i(0, 96), 
            Vec2i(16, 96), 
            Vec2i(32, 96), 
            Vec2i(48, 96), 
            Vec2i(64, 96), 
        };
        sprFalse.setAnimation(falseFrames, 4, true);
        sprPass = Sprite(Vec2i(0, 112), Vec2i(16, 16), Vec2i(cellSize/2, cellSize/2));
        sprFail = Sprite(Vec2i(16, 112), Vec2i(16, 16), Vec2i(cellSize/2, cellSize/2));

        cellSprites[CT_INA] = &sprInputA;
        cellSprites[CT_INB] = &sprInputB;
        cellSprites[CT_INC] = &sprInputC;
        cellSprites[CT_IND] = &sprInputD;
        cellSprites[CT_BLANK] = &sprBlankTile;
        cellSprites[CT_AND] = &sprAndTile;
        cellSprites[CT_OR] = &sprOrTile;
        cellSprites[CT_NOT] = &sprNotTile;
        cellSprites[CT_XOR] = &sprXOrTile;
        cellSprites[CT_NAND] = &sprNAndTile;
        cellSprites[CT_NOR] = &sprNOrTile;
        cellSprites[CT_XNOR] = &sprXNorTile;

        helpStrings[CT_EMPTY] = "VOID (Q) - Clear cell";
        helpStrings[CT_INA] = "A (A) - Input A";
        helpStrings[CT_INB] = "B (S) - Input B";
        helpStrings[CT_INC] = "C (D) - Input C";
        helpStrings[CT_IND] = "D (F) - Input D";
        helpStrings[CT_BLANK] = "BLANK (Q) - Blank cell";
        helpStrings[CT_AND] = "AND (E) - AND gate";
        helpStrings[CT_OR] = "OR (R) - OR gate";
        helpStrings[CT_NOT] = "NOT (T) - NOT gate";
        helpStrings[CT_XOR] = "XOR (Y) - XOR gate";
        helpStrings[CT_NAND] = "NAND (U) - NAND gate";
        helpStrings[CT_NOR] = "NOR (I) - NOR gate";
        helpStrings[CT_XNOR] = "XNOR (O) - XNOR gate";

        sndTick.set("1.wav");
        sndTick.volume = 32;
        sndAdd.set("14.wav");
        sndRemove.set("rock.ogg");
        sndParen.set("5.wav");
        sndTest.set("4.wav");

        bottomSize = cellSize * 3;
        bottomY = WINDOW_SIZE.y - bottomSize;
        for (int i = 0; i < 13; i++) {
            bottomTilePos[i] = Vec2i(i * cellSize, bottomY);
        }

        // DBG basic test
        cells[2][2].set(CT_AND);
        cells[2][2].parenLeft = true;
        cells[3][2].set(CT_INA);
        cells[4][2].set(CT_INB);
        cells[4][2].parenRight = true;
        
        cells[3][4].set(CT_OR);
        cells[3][4].parenLeft = true;
        cells[4][4].set(CT_INC);
        cells[5][4].set(CT_IND);
        cells[5][4].parenRight = true;

        showTests = true;
        highlightCellTypeStr = "EMPTY";
        codeString = "";
        helpString = "";
        tick = 0;
        codeErr = false;
        paused = false;
        tests.resize(8);
    }
    ~Grid() {}
    void process() override {
        if (paused) return;
        mousePos = Input::mousePos();
        mouseBtn = Input::getMouseBtn();
        mousePosCell = mousePos / cellSize;
        // Update lastMouse array with the most recent mousePosCell value
        if (tick % 2 == 0) {
            for (int i = 7; i > 0; i--) {
                lastMouse[i] = lastMouse[i - 1];
            }
            lastMouse[0] = mousePosCell;
        }
        if (lastMouse[0] != lastMouse[1]) {
            int relMouse = ((float)mousePosCell.x / gridSize.x) * 255;
            sndTick.setPan(255 - relMouse, relMouse);
            sndTick.play(true);
        }
        int x = mousePosCell.x;
        int y = mousePosCell.y;
        CellType cellType = cells[x][y].get();
        bool isTile = cellType != CT_VOID && cellType != CT_EMPTY;
        highlightCellTypeStr = Cell::typeToString(cellType);
        // Overwrite highlightCellTypeStr if mouse is over bottom area
        helpString = "";
        for (int i = 0; i < 13; i++) {
            Vec2i btp = bottomTilePos[i];
            btp = btp / cellSize;
            if (btp.x == x && btp.y == y) {
                highlightCellTypeStr = Cell::typeToString((CellType)i);
                helpString = helpStrings[(CellType)i];
            }
        }
        // Modify cells
        CellType newCell = CT_VOID;
        if (Input::keyDown(SDLK_q)) newCell = CT_EMPTY;
        if (Input::keyDown(SDLK_w)) newCell = CT_BLANK;
        if (Input::keyDown(SDLK_e)) newCell = CT_AND;
        if (Input::keyDown(SDLK_r)) newCell = CT_OR;
        if (Input::keyDown(SDLK_t)) newCell = CT_NOT;
        if (Input::keyDown(SDLK_a)) newCell = CT_INA;
        if (Input::keyDown(SDLK_s)) newCell = CT_INB;
        if (Input::keyDown(SDLK_d)) newCell = CT_INC;
        if (Input::keyDown(SDLK_f)) newCell = CT_IND;
        if (Input::keyDown(SDLK_y)) newCell = CT_XOR;
        if (Input::keyDown(SDLK_u)) newCell = CT_NAND;
        if (Input::keyDown(SDLK_i)) newCell = CT_NOR;
        if (Input::keyDown(SDLK_o)) newCell = CT_XNOR;
        if (newCell != CT_VOID) {
            DBG("New Cell: " + std::to_string(newCell));
            if (newCell == CT_EMPTY) {
                if (isTile) sndRemove.play();
            }
            else sndAdd.play();
            cells[x][y].set(newCell);
        }
        // Toggle connectors
        if (Input::keyDown(SDLK_TAB)) {
            cells[x][y].cycleParens();
            if (isTile) sndParen.play();
        }
        // Show tests
        if (Input::keyDown(SDLK_SPACE)) {
            showTests = !showTests;
            sndTest.play();
        }

        // Generate code string
        std::string codeStringOld = codeString;
        codeString = "";
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_EMPTY) {
                    bool isHovered = mousePosCell.x == x && mousePosCell.y == y;
                    if (isHovered) codeString += "_";
                    if (cells[x][y].parenLeft) codeString += "(";
                    if (cellType != CT_BLANK) codeString += Cell::typeToString(cellType);
                    if (cells[x][y].parenRight) codeString += ")";
                    if (isHovered) codeString += "_";
                    else if (cellType != CT_BLANK) codeString += " ";
                }
            }
        }
        std::string cs = Util::strReplace(Util::strReplace(codeString, " ", ""), "_", "");
        std::string cso = Util::strReplace(Util::strReplace(codeStringOld, " ", ""), "_", "");
        if (cs != cso) {
            DBG("Retesting code");
            DBG(cs);
            for (TestCase& test : tests) {
                test.check(codeString);
            }
        }
    }
    void render(Graphics* graph) override {
        if (paused) return;
        // Drag grid bg
        graph->setColor(colors["BG2"]);
        for (int x = 0; x < WINDOW_SIZE.x; x += cellSize) {
            graph->line(Vec2i(x, 0), Vec2i(x, WINDOW_SIZE.y));
        }
        for (int y = 0; y < WINDOW_SIZE.y; y += cellSize) {
            graph->line(Vec2i(0, y), Vec2i(WINDOW_SIZE.x, y));
        }
        // Draw border
        int borderWidth = codeErr ? cellSize/16 : cellSize/4;
        graph->setColor(codeErr ? colors["YELLOW"] : colors["BG2"], codeErr ? 128 : 255);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, bottomY - borderWidth), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        graph->rect(Vec2i(WINDOW_SIZE.x - borderWidth, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        
        // Draw mouse trail
        for (int i = 0; i < 8; i++) {
            Vec2i pos = lastMouse[i];
            Vec2i cellPos = pos * cellSize;
            Color c = colors["GREEN"];
            graph->setColor(c.r, c.g - (i * 8), c.b);
            graph->rect(cellPos, Vec2i(cellSize, cellSize), false);
        }

        // Draw cells
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_VOID && cellType != CT_EMPTY) {
                    sprBaseTile.render(graph, Vec2i(cellSize * x, cellSize * y));
                    Sprite* spr = nullptr;
                    spr = cellSprites[cellType];
                    if (spr != nullptr) {
                        spr->render(graph, Vec2i(cellSize * x, cellSize * y));
                    }
                    else {
                        DBG("Unknown cell type: " + std::to_string(cellType));
                    }
                    // Draw parenthesis
                    if (cells[x][y].parenLeft) {
                        sprParenR.render(graph, Vec2i(cellSize * x, cellSize * y), true);
                    }
                    if (cells[x][y].parenRight) {
                        sprParenR.render(graph, Vec2i(cellSize * x, cellSize * y));
                    }
                }
            }
        }

        // Highlight cell on mouse hover
        Vec2i cell = mousePos / cellSize * cellSize;
        graph->setColor(colors["WHITE"]);
        graph->rect(cell, Vec2i(cellSize, cellSize), false);

        // Draw bottom area
        graph->setColor(colors["BG2"]);
        graph->rect(Vec2i(0, bottomY), Vec2i(WINDOW_SIZE.x, bottomSize));

        sprBaseTile.render(graph, bottomTilePos[CT_EMPTY]);
        sprVoid.render(graph, bottomTilePos[CT_EMPTY]);
        sprBaseTile.render(graph, bottomTilePos[CT_INA]);
        cellSprites[CT_INA]->render(graph, bottomTilePos[CT_INA]);
        sprBaseTile.render(graph, bottomTilePos[CT_INB]); 
        cellSprites[CT_INB]->render(graph, bottomTilePos[CT_INB]);
        sprBaseTile.render(graph, bottomTilePos[CT_INC]); 
        cellSprites[CT_INC]->render(graph, bottomTilePos[CT_INC]);
        sprBaseTile.render(graph, bottomTilePos[CT_IND]); 
        cellSprites[CT_IND]->render(graph, bottomTilePos[CT_IND]);
        sprBaseTile.render(graph, bottomTilePos[CT_BLANK]); 
        cellSprites[CT_BLANK]->render(graph, bottomTilePos[CT_BLANK]);
        sprBaseTile.render(graph, bottomTilePos[CT_AND]); 
        cellSprites[CT_AND]->render(graph, bottomTilePos[CT_AND]);
        sprBaseTile.render(graph, bottomTilePos[CT_OR]); 
        cellSprites[CT_OR]->render(graph, bottomTilePos[CT_OR]);
        sprBaseTile.render(graph, bottomTilePos[CT_NOT]);
        cellSprites[CT_NOT]->render(graph, bottomTilePos[CT_NOT]);
        sprBaseTile.render(graph, bottomTilePos[CT_XOR]);
        cellSprites[CT_XOR]->render(graph, bottomTilePos[CT_XOR]);
        sprBaseTile.render(graph, bottomTilePos[CT_NAND]);
        cellSprites[CT_NAND]->render(graph, bottomTilePos[CT_NAND]);
        sprBaseTile.render(graph, bottomTilePos[CT_NOR]);
        cellSprites[CT_NOR]->render(graph, bottomTilePos[CT_NOR]);
        sprBaseTile.render(graph, bottomTilePos[CT_XNOR]);
        cellSprites[CT_XNOR]->render(graph, bottomTilePos[CT_XNOR]);

        graph->setColor(colors["YELLOW"]);
        graph->text(mousePosCell.toString(), Vec2i(WINDOW_SIZE.x - cellSize * 2, bottomY + 4), FONT_SIZE);
        // graph->text(highlightCellTypeStr, Vec2i(WINDOW_SIZE.x - cellSize * 2, bottomY + 4), FONT_SIZE);
        // graph->text(highlightCellTypeStr, mousePos + Vec2i(-cellSize/2, cellSize));

        // Tests
        int testWinWidth = FONT_SIZE * 8;
        int testWinX = WINDOW_SIZE.x - (showTests ? testWinWidth : cellSize / 4);
        //graph->setColor(colors["BG3"]);
        //graph->rect(Vec2i(testWinX, 0), Vec2i(testWinWidth, bottomY - cellSize), true);
        int y = cellSize * 2;
        int ce = cellSize / 2;
        codeErr = false;
        for (TestCase test : tests) {
            // DBG("Test: " + std::to_string(test.lastCheck));
            if (test.lastCheck) sprPass.render(graph, Vec2i(testWinX - 4, y));
            else sprFail.render(graph, Vec2i(testWinX - 4 - (tick/8 % 6), y));
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
            if (test.hasError) codeErr = true;
            y += ce + (ce/2);
        }
        if (codeErr) {
            graph->setColor(colors["YELLOW"]);
            graph->text("!! ERROR", Vec2i(testWinX + 16, cellSize - (tick/4 % 8)), FONT_SIZE);
        }
        else {
            graph->setColor(colors["GRAY"]);
            graph->text("// TESTS", Vec2i(testWinX + 16, cellSize), FONT_SIZE);
        }

        if (helpString.length() > 0) {
            graph->setColor(colors["GREEN"]);
            graph->text("Info: ", Vec2i(10, bottomY + cellSize + 8), FONT_SIZE);
            int w = graph->textWidth("Info: ", FONT_SIZE);
            graph->setColor(200, 200, 200);
            graph->text(helpString, Vec2i(10 + w, bottomY + cellSize + 8), FONT_SIZE);
        }
        else if (codeString.length() > 0) {
            std::string codePre = codeErr ? "!!" :">> ";
            graph->setColor(codeErr ? colors["YELLOW"] : colors["GREEN"]);
            graph->text(codePre, Vec2i(10, bottomY + cellSize + 8), FONT_SIZE);
            int w = graph->textWidth(codePre, FONT_SIZE);
            // Split the string by _
            std::vector<std::string> split = Util::splitString(codeString, "_");
            std::string p1 = split[0];
            std::string p2 = "";
            std::string p3 = "";
            if (split.size() == 3) {
                p2 = split[1] + " ";
                p3 = split[2];
            }
            // 
            int p1w = graph->textWidth(p1, FONT_SIZE);
            int p2w = graph->textWidth(p2, FONT_SIZE);
            if (p1.length() > 0) {
                graph->setColor(colors["GRAY"]);
                graph->text(p1, Vec2i(10 + w, bottomY + cellSize + 8), FONT_SIZE);
            }
            if (p2.length() > 0) {
                graph->setColor(colors["GREEN"]);
                graph->text(p2, Vec2i(10 + w + p1w, bottomY + cellSize + 8), FONT_SIZE);
            }
            if (p3.length() > 0) {
                graph->setColor(colors["GRAY"]);
                graph->text(p3, Vec2i(10 + w + p1w + p2w, bottomY + cellSize + 8), FONT_SIZE);
            }
        }

        // Draw static noise
        if (tick % 2 == 0) {
            int staticSize = cellSize / 32;
            graph->setColor(colors["YELLOW"], 64);
            for (int i = 0; i < WINDOW_SIZE.size2d()/(staticSize * (codeErr ? 128 : 1024)); i++) {
                int x = rand() % WINDOW_SIZE.x;
                int y = rand() % WINDOW_SIZE.y;
                graph->rect(Vec2i(x, y), Vec2i(staticSize, staticSize));
            }
        }

        // Draw opposite scans
        // graph->setColor(colors["GREEN"], 32);
        // for (int x = 0; x < WINDOW_SIZE.x; x += cellSize) {
        //     int xx = x + sin(x + (tick/6) * 0.1) * cellSize;
        //     graph->line(Vec2i(xx, 0), Vec2i(xx, WINDOW_SIZE.y));
        // }

        // Draw scanlines
        graph->setColor(colors["GREEN"],32);
        for (int y = 0; y < WINDOW_SIZE.y; y += cellSize) {
            int yy = y + sin(y + (tick/6) * 0.1) * cellSize;
            graph->line(Vec2i(0, yy), Vec2i(WINDOW_SIZE.x, yy));
        }

        tick++;
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
        pos = Input::mousePos();
    }
    void render(Graphics* graph) override {
        spr.render(graph, pos);
    }
};

class App : public Imp::Main { 
public:
    Grid grid;
    Cursor cursor;
    Menu menu;
    App() : Imp::Main("EsoMachina (v0.1-alpha)", WINDOW_SIZE, 60, "tiles.png") { 
        clearColor = Color(colors["BG"]);
        entityMan.addEntity(&grid);
        entityMan.addEntity(&menu);
        entityMan.addEntity(&cursor);
    }
    ~App() {}
    void process() override {
        // Pause
        if (Input::keyDown(SDLK_ESCAPE)) {
            menu.show = !menu.show;
            DBG("Toggle menu " + std::to_string(grid.paused));
        }
        // Pause the grid when menu is showing
        // Outside of cond because can be triggered other ways
        grid.paused = menu.show;
    }
    void onLostFocus() override {
        pauseRenderer = true;
    }
    void onGainedFocus() override {
        pauseRenderer = false;
    }
};