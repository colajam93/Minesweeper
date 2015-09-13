#ifndef MINESWEEPERMODEL_H
#define MINESWEEPERMODEL_H

#include "minesweepercommon.h"

#include <vector>
#include <utility>

namespace MS {
enum class CellState {
    None, Flag, Doubt
};

enum class CellElement {
    Empty, Mine
};

class Cell {
    CellState state_;
    CellElement element_;
    bool opened_;

public:
    Cell();
    void setMine();
    void setOpened();
    CellState setNextState();
    bool isMine() const;
    bool isOpened() const;
    bool isFlagged() const;
};

class MinesweeperModel
{
    using CellInfo = std::pair<Cell*, Position>;

    const int row_;
    const int column_;
    const int mine_;
    std::vector<Cell> cells_;
    std::vector<int> adjacentMineCount_;
    bool isInitialized_ = false;

    int positionToIndex(const Position& position) const;
    Position indexToPosition(int index) const;
    CellInfo getCellInfo(const Position& position);
    CellInfo getCellInfo(int row, int column);
    std::vector<Position> getAdjacentPositions(const Position& position);

public:
    MinesweeperModel(int row, int column, int mine);
    void initialize(int clickedRow, int clickedColumn);
    std::vector<CellChange> open(int row, int column);
    std::vector<CellChange> nextState(int row, int column);
    bool isInitialized() const;
    bool isSucceeded() const;
    int getRow() const;
    int getColumn() const;
    void setUninitialized();
};
} // namespace MS

#endif // MINESWEEPERMODEL_H
