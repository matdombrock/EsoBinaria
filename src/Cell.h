/*
* This class represents a cell on the grid in the game.
* It holds information about the cell type, parentheses, and comments.
*/
#pragma once
#include "Imp.h"
using namespace Imp;

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
        type = CT_VOID;
        parenLeft = false;
        parenRight = false;
        isComment = false;
    }
    ~Cell() {}
    void set(CellType type) {
        if (type == CT_VOID) {
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
        // Non OP tiles can only have right side parens
        if (type == CT_BLANK || type == CT_INA || type == CT_INB || type == CT_INC || type == CT_IND) {
            parenRight = !parenRight;
        }
        // All OP tiles are locked to left paren
    }
    static std::string typeToString(CellType type) {
        switch (type) {
            case CT_VOID: return "VOID";
            case CT_CLEAR: return "CLEAR";
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
