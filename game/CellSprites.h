#pragma once
#include <map>
#include "../src/Imp.h"
using namespace Imp;
#include "Cell.h"
#include "_gameMaster.h"

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