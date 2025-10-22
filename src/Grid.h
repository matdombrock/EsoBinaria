/*
* Grid class for the logic gate puzzle game
* Manages the grid of cells, user interactions, and rendering
* on the grid
*/
#pragma once
#include "Imp.h"
using namespace Imp;
#include "Cell.h"
#include "CellSprites.h"
#include "g_colors.h"
#include "g_gameMaster.h"

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
        Vec2i gridBounds = Vec2i(WINDOW_SIZE.x, WINDOW_SIZE.y - g_gm.bottomBarSize.y);
        gridSize = gridBounds / g_gm.cellSize;
        // Resize the 2d array to the grid size
        cells.resize(gridSize.x);
        // Fill cells with 0
        for (int x = 0; x < gridSize.x; x++) {
            cells[x].resize(gridSize.y);
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_VOID);
            }
        }

        sndTick.set("tick.wav");
        // sndTick.volume = 32;
        sndAdd.set("add.wav");
        sndRemove.set("rock.ogg");
        sndParen.set("paren.wav");  

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
    void translate(Vec2i pos) {
        std::vector<std::vector<Cell>> newCells(gridSize.x, std::vector<Cell>(gridSize.y));
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
            int newX = (x + pos.x + gridSize.x) % gridSize.x;
            int newY = (y + pos.y + gridSize.y) % gridSize.y;
            newCells[newX][newY] = cells[x][y];
            }
        }
        cells = newCells;
    }
    void reset() {
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                cells[x][y].set(CT_VOID);
            }
        }
        // Puzzle specific
        // Fill tutorial puzzles
        // 0 place B
        // 1 place NOT
        if (g_gm.getPuzzleNum() == 1 && g_gm.getPuzzleChallenge() == 'e') {
            cells[6][4].set(CT_INA);
        }
        // 2 remove tiles
        if (g_gm.getPuzzleNum() == 2 && g_gm.getPuzzleChallenge() == 'e') {
            cells[6][4].set(CT_NOT);
            cells[2][2].set(CT_INC);
            cells[3][4].set(CT_INB);
            cells[4][7].set(CT_BLANK);
        }
        // 3 solo

        //
        DBG("Sending load msg");
        g_gm.sendMessage("load");
    }
    void process() override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        if (g_gm.getHukActive()) return;
        if (g_gm.getModalActive()) return;
        mousePos = GInput.mousePos();
        mousePosCell = mousePos / g_gm.cellSize;
        // Update lastMouse array with the most recent mousePosCell value
        if (g_gm.getTick() % 2 == 0) {
            for (int i = 7; i > 0; i--) {
                lastMouse[i] = lastMouse[i - 1];
            }
            lastMouse[0] = mousePosCell;
        }
        int x = mousePosCell.x;
        int y = mousePosCell.y;
        CellType cellType = CT_VOID;
        if (x >= 0 && x < gridSize.x && y >= 0 && y < gridSize.y) {
            cellType = cells[x][y].get();
        }
        bool isTile = cellType != CT_VOID && cellType != CT_CLEAR;
        highlightCellTypeStr = Cell::typeToString(cellType);
        
        if (GInput.keyOnce(SDLK_q)) g_gm.setActiveTile(CT_CLEAR);
        if (GInput.keyOnce(SDLK_BACKSPACE)) g_gm.setActiveTile(CT_CLEAR);
        if (GInput.keyOnce(SDLK_w)) g_gm.setActiveTile(CT_BLANK);
        if (GInput.keyOnce(SDLK_e)) g_gm.setActiveTile(CT_AND);
        if (GInput.keyOnce(SDLK_r)) g_gm.setActiveTile(CT_OR);
        if (GInput.keyOnce(SDLK_t)) g_gm.setActiveTile(CT_NOT);
        if (GInput.keyOnce(SDLK_a)) g_gm.setActiveTile(CT_INA);
        if (GInput.keyOnce(SDLK_s)) g_gm.setActiveTile(CT_INB);
        if (GInput.keyOnce(SDLK_d)) g_gm.setActiveTile(CT_INC);
        if (GInput.keyOnce(SDLK_f)) g_gm.setActiveTile(CT_IND);
        if (GInput.keyOnce(SDLK_y)) g_gm.setActiveTile(CT_XOR);
        if (GInput.keyOnce(SDLK_u)) g_gm.setActiveTile(CT_NAND);
        if (GInput.keyOnce(SDLK_i)) g_gm.setActiveTile(CT_NOR);
        if (GInput.keyOnce(SDLK_o)) g_gm.setActiveTile(CT_XNOR);

        if (GInput.mousePos().y < g_gm.bottomBarPos.y) {
            if (lastMouse[0] != lastMouse[1]) {
                int relMouse = ((float)mousePosCell.x / gridSize.x) * 255;
                if(g_gm.getActiveTestData() == nullptr) {
                    sndTick.setPan(255 - relMouse, relMouse);
                    sndTick.play();
                }
            }

            CellType newCell = CT_VOID;
            if (GInput.mouseKeyOnce(SDL_BUTTON_LEFT)) {
                CellType active = g_gm.getActiveTile();
                // Grab tile if nothing is active
                if (active == CT_VOID && cellType != CT_VOID) {
                    DBG("Grabbed cell type: " + Cell::typeToString(cellType));
                    g_gm.setActiveTile(cellType, true);
                    newCell = CT_CLEAR;
                }
                else {
                    newCell = active;
                }
            }
            // Drop tile
            if (newCell != CT_VOID && x >= 0 && x < gridSize.x && y >= 0 && y < gridSize.y) {
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
                if (newCell != CT_CLEAR && !GInput.key(SDLK_LCTRL)) {
                    g_gm.setActiveTile(CT_VOID, false);
                }
            }
            // Remove held tile or right click
            if (GInput.mouseKeyOnce(SDL_BUTTON_RIGHT) || GInput.mouseKeyOnce(4)) {
                if (g_gm.getActiveTile() != CT_VOID) {
                    g_gm.setActiveTile(CT_VOID);
                    return;
                }
            }
            // Toggle connectors
            if ((x >= 0 && x < gridSize.x && y >= 0 && y < gridSize.y) && 
                (GInput.keyOnce(SDLK_TAB) || GInput.mouseKeyOnce(SDL_BUTTON_RIGHT) || GInput.mouseKeyOnce(4))) {
                cells[x][y].cycleParens();
                if (isTile) sndParen.play();
            }
            // Toggle comments
            if ((x >= 0 && x < gridSize.x && y >= 0 && y < gridSize.y) && 
                (GInput.keyOnce(SDLK_c) || GInput.mouseKeyOnce(SDL_BUTTON_MIDDLE))) {
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
        std::string codeStringOld = g_gm.getCodeString();
        std::string codeStringNew = "";
        g_gm.setCodeString("");
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
        g_gm.setCodeString(codeStringNew);
    }
    void render(Graphics* graph) override {
        if (g_gm.getScreen() != SCN_PUZZLE) return;
        // Drag grid bg
        graph->setColor(g_colors["BG2"]);
        for (int x = 0; x < gridSize.x; x++) {
            graph->line(Vec2i(x * g_gm.cellSize, 0), Vec2i(x * g_gm.cellSize, gridSize.y * g_gm.cellSize));
        }
        for (int y = 0; y < gridSize.y; y++) {
            graph->line(Vec2i(0, y * g_gm.cellSize), Vec2i(gridSize.x * g_gm.cellSize, y * g_gm.cellSize));
        }
        // Draw border
        int borderWidth = g_gm.hasCodeErr() ? g_gm.cellSize/16 : g_gm.cellSize/4;
        graph->setColor(g_gm.hasCodeErr() ? g_colors["YELLOW"] : g_colors["BG2"], g_gm.hasCodeErr() ? 128 : 255);
        graph->rect(Vec2i(0, 0), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, g_gm.bottomBarPos.y - borderWidth), Vec2i(WINDOW_SIZE.x, borderWidth));
        graph->rect(Vec2i(0, 0), Vec2i(borderWidth, WINDOW_SIZE.y));
        graph->rect(Vec2i(WINDOW_SIZE.x - borderWidth, pos.y), Vec2i(borderWidth, WINDOW_SIZE.y));
        
        // Draw mouse trail
        for (int i = 0; i < 8; i++) {
            Vec2i pos = lastMouse[i];
            Vec2i cellPos = pos * g_gm.cellSize;
            Color c = g_colors["GREEN"];
            graph->setColor(c.r, c.g - (i * 8), c.b);
            graph->rect(cellPos, Vec2i(g_gm.cellSize, g_gm.cellSize), false);
        }

        // Draw cells
        for (int x = 0; x < gridSize.x; x++) {
            for (int y = 0; y < gridSize.y; y++) {
                CellType cellType = cells[x][y].get();
                if (cellType != CT_VOID && cellType != CT_CLEAR) {
                    CellSprites::baseTile.render(graph, Vec2i(g_gm.cellSize * x, g_gm.cellSize * y));
                    Sprite* spr = CellSprites::cellMap[cellType];
                    // Draw test highlights
                    if (g_gm.getActiveTestData() != nullptr) {
                        if (cellType == CT_INA) {
                            spr = g_gm.getActiveTestData()->inputs[0] ? &CellSprites::inATrueTile : &CellSprites::inAFalseTile;
                        }
                        if (cellType == CT_INB) {
                            spr = g_gm.getActiveTestData()->inputs[1] ? &CellSprites::inBTrueTile : &CellSprites::inBFalseTile;
                        }
                        if (cellType == CT_INC) {
                            spr = g_gm.getActiveTestData()->inputs[2] ? &CellSprites::inCTrueTile : &CellSprites::inCFalseTile;
                        }
                        if (cellType == CT_IND) {
                            spr = g_gm.getActiveTestData()->inputs[3] ? &CellSprites::inDTrueTile : &CellSprites::inDFalseTile;
                        }
                    }
                    if (spr != nullptr) {
                        spr->render(graph, Vec2i(g_gm.cellSize * x, g_gm.cellSize * y));
                    }
                    else {
                        DBG("Unknown cell type: " + std::to_string(cellType));
                    }
                    // Draw parenthesis
                    if (cells[x][y].parenLeft) {
                        CellSprites::parenR.render(graph, Vec2i(g_gm.cellSize * x, g_gm.cellSize * y), true);
                    }
                    if (cells[x][y].parenRight) {
                        CellSprites::parenR.render(graph, Vec2i(g_gm.cellSize * x, g_gm.cellSize * y));
                    }
                    // Comment
                    if (cells[x][y].isComment) {
                        graph->setColor(g_colors["GRAY"], 128 + (std::sin(g_gm.getTick()/32.0f) * 16));
                        graph->rect(Vec2i(g_gm.cellSize * x, g_gm.cellSize * y), Vec2i(g_gm.cellSize, g_gm.cellSize));
                    }
                }
            }
        }

        // Highlight cell on mouse hover
        if (g_gm.getActiveTestData() == nullptr && mousePos.y < g_gm.bottomBarPos.y) {
            Vec2i cell = mousePos / g_gm.cellSize * g_gm.cellSize;
            graph->setColor(g_colors["WHITE"]);
            graph->rect(cell, Vec2i(g_gm.cellSize, g_gm.cellSize), false); 
            if (g_gm.getActiveTile() != CT_VOID) {
                CellSprites::baseTile.render(graph, cell - Vec2i(0, g_gm.vu(0.25f)));
                CellSprites::cellMap[g_gm.getActiveTile()]->render(graph, cell - Vec2i(0, g_gm.vu(0.25f)));
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
    std::string cellsToString() {
        std::string str = "";
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                Cell cell = cells[x][y];
                std::string cellString = "";
                cellString += std::to_string(cell.get()) + ",";
                cellString += cell.parenLeft ? "1," : "0,";
                cellString += cell.parenRight ? "1," : "0,";
                cellString += cell.isComment ? "1," : "0,";
                str += cellString;
            }
        }
        return str;
    }
    void setCellsFromString(std::string str) {
        std::vector<std::string> split = Imp::StringTools::split(str, ",");
        for (int y = 0; y < gridSize.y; y++) {
            for (int x = 0; x < gridSize.x; x++) {
                Cell cell = cells[x][y];
                int i = (y * gridSize.x + x) * 4;
                CellType cellType = (CellType)std::stoi(split[i]);
                cell.set(cellType);
                cell.parenLeft = std::stoi(split[i + 1]);
                cell.parenRight = std::stoi(split[i + 2]);
                cell.isComment = std::stoi(split[i + 3]);
                cells[x][y] = cell;
            }
        }
    }
};
