/*
* A namespace containing all cell sprites
*/
#pragma once
#include "Imp.h"
#include <map>
#include "Cell.h"
#include "g_gameMaster.h"

using namespace Imp;

namespace CellSprites {
inline Sprite baseTile =
    Sprite(Vec2i(0, 0), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite blankTile =
    Sprite(Vec2i(0, 32), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite andTile =
    Sprite(Vec2i(0, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite orTile =
    Sprite(Vec2i(16, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite notTile =
    Sprite(Vec2i(32, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite xorTile =
    Sprite(Vec2i(48, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite nandTile =
    Sprite(Vec2i(64, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite norTile =
    Sprite(Vec2i(80, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite xnorTile =
    Sprite(Vec2i(96, 16), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite parenR =
    Sprite(Vec2i(32, 0), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inATile =
    Sprite(Vec2i(0, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inBTile =
    Sprite(Vec2i(16, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inCTile =
    Sprite(Vec2i(32, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inDTile =
    Sprite(Vec2i(48, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite clear =
    Sprite(Vec2i(16, 32), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite specVoid = Sprite(Vec2i(256, 256), Vec2i(0, 0), Vec2i(0, 0));
inline Sprite inATrueTile =
    Sprite(Vec2i(64, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inBTrueTile =
    Sprite(Vec2i(80, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inCTrueTile =
    Sprite(Vec2i(96, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inDTrueTile =
    Sprite(Vec2i(112, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inAFalseTile =
    Sprite(Vec2i(128, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inBFalseTile =
    Sprite(Vec2i(144, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inCFalseTile =
    Sprite(Vec2i(160, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline Sprite inDFalseTile =
    Sprite(Vec2i(176, 48), Vec2i(16, 16), Vec2i(g_gm.cellSize, g_gm.cellSize));
inline std::map<CellType, Sprite *> cellMap = {
    {CT_VOID, &specVoid},   {CT_CLEAR, &clear}, {CT_AND, &andTile},
    {CT_OR, &orTile},       {CT_NOT, &notTile}, {CT_XOR, &xorTile},
    {CT_NAND, &nandTile},   {CT_NOR, &norTile}, {CT_XNOR, &xnorTile},
    {CT_BLANK, &blankTile}, {CT_INA, &inATile}, {CT_INB, &inBTile},
    {CT_INC, &inCTile},     {CT_IND, &inDTile},
};
}; // namespace CellSprites
