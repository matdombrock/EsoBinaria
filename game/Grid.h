#pragma once
#include "../src/Imp.h"
using namespace Imp;
#include "Cell.h"
#include "CellSprites.h"
#include "_colors.h"
#include "_gameMaster.h"

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
                cells[x][y].set(CT_VOID);
            }
        }

        sndTick.set("1.wav");
        // sndTick.volume = 32;
        sndAdd.set("14.wav");
        sndRemove.set("rock.ogg");
        sndParen.set("5.wav");  

        highlightCellTypeStr = "CLEAR";
        helpString = "";

        reset();

        DBG("Grid started");

        // DBG basic test
        // cells[3][3].set(CT_NOT);
        // cells[3][3].parenLeft = true;
        // cells[4][3].set(CT_OR);
        // cells[4][3].parenLeft = true;
        // cells[5][3].set(CT_INA);
        // cells[6][3].set(CT_INB);
        // cells[7][3].set(CT_INC);
        // cells[7][3].parenRight = true;
        // cells[8][3].set(CT_BLANK);
        // cells[8][3].parenRight = true;
    }
    ~Grid() {}
    void reset() {
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_VOID);
            }
        }
        // 0 place B
        // 1 place NOT
        if (_g.getPuzzleNum() == 1 && _g.getPuzzleChallenge() == 'e') {
            cells[6][4].set(CT_INA);
        }
        // 2 place A
        // 3 place NOT
        if (_g.getPuzzleNum() == 2 && _g.getPuzzleChallenge() == 'e') {
            cells[6][4].set(CT_NOT);
        }
    }
    void process() override {
        if (_g.getScreen() != SCN_PUZZLE) return;
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
        
        if (_input.keyOnce(SDLK_q)) _g.setActiveTile(CT_CLEAR);
        if (_input.keyOnce(SDLK_BACKSPACE)) _g.setActiveTile(CT_CLEAR);
        if (_input.keyOnce(SDLK_w)) _g.setActiveTile(CT_BLANK);
        if (_input.keyOnce(SDLK_e)) _g.setActiveTile(CT_AND);
        if (_input.keyOnce(SDLK_r)) _g.setActiveTile(CT_OR);
        if (_input.keyOnce(SDLK_t)) _g.setActiveTile(CT_NOT);
        if (_input.keyOnce(SDLK_a)) _g.setActiveTile(CT_INA);
        if (_input.keyOnce(SDLK_s)) _g.setActiveTile(CT_INB);
        if (_input.keyOnce(SDLK_d)) _g.setActiveTile(CT_INC);
        if (_input.keyOnce(SDLK_f)) _g.setActiveTile(CT_IND);
        if (_input.keyOnce(SDLK_y)) _g.setActiveTile(CT_XOR);
        if (_input.keyOnce(SDLK_u)) _g.setActiveTile(CT_NAND);
        if (_input.keyOnce(SDLK_i)) _g.setActiveTile(CT_NOR);
        if (_input.keyOnce(SDLK_o)) _g.setActiveTile(CT_XNOR);

        if (_input.mousePos().y < _g.bottomBarPos.y) {
            if (lastMouse[0] != lastMouse[1]) {
                int relMouse = ((float)mousePosCell.x / gridSize.x) * 255;
                if(_g.getActiveTestData() == nullptr) {
                    sndTick.setPan(255 - relMouse, relMouse);
                    sndTick.play();
                }
            }

            CellType newCell = CT_VOID;
            if (_input.mouseKeyOnce(SDL_BUTTON_LEFT)) {
                CellType active = _g.getActiveTile();
                // Grab tile if nothing is active
                if (active == CT_VOID && cellType != CT_VOID) {
                    DBG("Grabbed cell type: " + Cell::typeToString(cellType));
                    _g.setActiveTile(cellType, true);
                    newCell = CT_CLEAR;
                }
                else {
                    newCell = active;
                }
            }
            // Drop tile
            if (newCell != CT_VOID) {
                DBG("New Cell: " + std::to_string(newCell));
                if (newCell == CT_CLEAR) {
                    if (isTile) sndRemove.play();
                    // Do not ever place the CT_CLEAR tile
                    cells[x][y].set(CT_VOID);
                }
                else {
                    sndAdd.play();
                    cells[x][y].set(newCell);
                }
                // clear active Tile
                if (newCell != CT_CLEAR && !_input.key(SDLK_LCTRL)) {
                    _g.setActiveTile(CT_VOID, false);
                }
            }
            // Remove held tile or right click
            if (_input.mouseKeyOnce(SDL_BUTTON_RIGHT) || _input.mouseKeyOnce(4)) {
                if (_g.getActiveTile() != CT_VOID) {
                    _g.setActiveTile(CT_VOID);
                    return;
                }
            }
            // Toggle connectors
            if (_input.keyOnce(SDLK_TAB) || _input.mouseKeyOnce(SDL_BUTTON_RIGHT) || _input.mouseKeyOnce(4)) {
                cells[x][y].cycleParens();
                if (isTile) sndParen.play();
            }
            // Toggle comments
            if (_input.keyOnce(SDLK_c) || _input.mouseKeyOnce(SDL_BUTTON_MIDDLE)) {
                cells[x][y].isComment = !cells[x][y].isComment;
            }
        }

        // Mod tiles
        CellType cellTypeLast = CT_VOID;
        int xOld = 0;
        int yOld = 0;
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                CellType cellType = cells[x][y].get();
                // Auto parenthesize NOT
                if (cellTypeLast == CT_NOT && cellType != CT_VOID) {
                    cells[x][y].parenRight = true;
                    cells[xOld][yOld].parenLeft = true;
                }
                // Auto left parenthesize OPs
                if (cellType != CT_VOID 
                    && cellType != CT_BLANK 
                    && cellType != CT_INA 
                    && cellType != CT_INB 
                    && cellType != CT_INC 
                    && cellType != CT_IND) 
                {
                    cells[x][y].parenLeft = true;
                    cells[x][y].parenRight = false;
                }
                // Update last if not void
                if (cellType != CT_VOID) {
                    cellTypeLast = cellType;
                    xOld = x;
                    yOld = y;
                }

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
                if (cellType != CT_VOID) {
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
        if (_g.getScreen() != SCN_PUZZLE) return;
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
                    Sprite* spr = CellSprites::cellMap[cellType];
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
        // if (_g.getTick() % 2 == 0) {
        //     int staticSize = _g.cellSize / 32;
        //     graph->setColor(_colors["YELLOW"], 64);
        //     for (int i = 0; i < WINDOW_SIZE.size2d()/(staticSize * (_g.hasCodeErr() ? 128 : 1024)); i++) {
        //         int x = rand() % WINDOW_SIZE.x;
        //         int y = rand() % WINDOW_SIZE.y;
        //         graph->rect(Vec2i(x, y), Vec2i(staticSize, staticSize));
        //     }
        // }

        // Draw opposite scans
        // graph->setColor(_colors["GREEN"], 32);
        // for (int x = 0; x < WINDOW_SIZE.x; x += _g.cellSize) {
        //     int xx = x + sin(x + (tick/6) * 0.1) * _g.cellSize;
        //     graph->line(Vec2i(xx, 0), Vec2i(xx, WINDOW_SIZE.y));
        // }

        // Draw scanlines
        // graph->setColor(_colors["GREEN"],32);
        // for (int y = 0; y < WINDOW_SIZE.y; y += _g.cellSize) {
        //     int yy = y + sin(y + (_g.getTick()/6) * 0.1) * _g.cellSize;
        //     graph->line(Vec2i(0, yy), Vec2i(WINDOW_SIZE.x, yy));
        // }
    }
};