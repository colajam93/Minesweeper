#ifndef MINESWEEPERMODEL_H
#define MINESWEEPERMODEL_H

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
    void setElement(CellElement element);
    bool isMine() const;
};

class MinesweeperModel
{
    using Position = std::pair<int, int>;

    int row_;
    int column_;
    std::vector<Cell> cells_;
    std::vector<int> adjacentMineCount_;

    int positionToIndex(int row, int column) const;
    Position indexToPosition(int index) const;
    Cell* getCell(int row, int column);
    std::vector<Cell*> getAdjacentCells(int row, int column);

public:
    MinesweeperModel(int row, int column, int mine);
};
} // namespace MS

#endif // MINESWEEPERMODEL_H
