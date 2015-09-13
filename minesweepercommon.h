#ifndef MINESWEEPERCOMMON_H
#define MINESWEEPERCOMMON_H

#include <utility>

namespace MS {
enum class CellView {
    Mine, Zero, One, Two, Three, Four, Five, Six, Seven, Eight, None, Flag, Doubt
};

struct Position {
    int row = 0;
    int column = 0;
    Position() = default;
    Position(int row, int column)
    {
        this->row = row;
        this->column = column;
    }
};

using CellChange = std::pair<CellView, Position>;

enum class ClickType {
    Open, NextState, AutoOpen
};
} // namespace MS

#endif // MINESWEEPERCOMMON_H
