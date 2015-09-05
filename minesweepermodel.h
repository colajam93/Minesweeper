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

class Cell {
    CellState state_;
    CellElement element_;
    bool opened_;

public:
    Cell();
    void setElement(CellElement element);
    bool isMine() const;
};

struct Position {
    int row = 0;
    int column = 0;
    Position() = default;
    Position(int row, int column);
    Position(std::initializer_list<int> list);
};

class MinesweeperModel
{
    using CellInfo = std::pair<Cell*, Position>;

    int row_;
    int column_;
    std::vector<Cell> cells_;
    std::vector<int> adjacentMineCount_;

    int positionToIndex(const Position& position) const;
    Position indexToPosition(int index) const;
    CellInfo getCellInfo(const Position& position);
    CellInfo getCellInfo(int row, int column);
    std::vector<CellInfo> getAdjacentCellInfos(const Position& position);

public:
    MinesweeperModel(int row, int column, int mine);
};
} // namespace MS

#endif // MINESWEEPERMODEL_H
