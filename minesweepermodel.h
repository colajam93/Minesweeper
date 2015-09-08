#ifndef MINESWEEPERMODEL_H
#define MINESWEEPERMODEL_H

#include <vector>
#include <utility>
#include <initializer_list>

namespace MS {
enum class CellState {
    None, Flag, Doubt
};

enum class CellElement {
    Empty, Mine
};

enum class CellView {
    Mine, Zero, One, Two, Three, Four, Five, Six, Seven, Eight, None, Flag, Doubt
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

struct Position {
    int row = 0;
    int column = 0;
    Position() = default;
    Position(int row, int column);
    Position(std::initializer_list<int> list);
};

using CellChange = std::pair<CellView, Position>;

class MinesweeperModel
{
    using CellInfo = std::pair<Cell*, Position>;

    int row_;
    int column_;
    int mine_;
    std::vector<Cell> cells_;
    std::vector<int> adjacentMineCount_;
    bool isInitialized_ = false;

    int positionToIndex(const Position& position) const;
    Position indexToPosition(int index) const;
    CellInfo getCellInfo(const Position& position);
    CellInfo getCellInfo(int row, int column);
    std::vector<CellInfo> getAdjacentCellInfos(const Position& position);

public:
    MinesweeperModel(int row, int column, int mine);
    void init(int clickedRow, int clickedColumn);
    std::vector<CellChange> open(int row, int column);
    std::vector<CellChange> nextState(int row, int column);
    bool isInitialized() const;
};
} // namespace MS

#endif // MINESWEEPERMODEL_H
