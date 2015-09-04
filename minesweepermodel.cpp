#include "minesweepermodel.h"
#include <random>

namespace {
static std::random_device rd;
static std::mt19937 mt{rd()};
}

namespace MS {
Cell::Cell()
    : state_(CellState::None), element_(CellElement::Empty), opened_(false)
{
}

void Cell::setElement(CellElement element)
{
    element_ = element;
}

bool Cell::isMine() const
{
    return element_ == CellElement::Mine;
}

MinesweeperModel::MinesweeperModel(int row, int column, int mine)
    : row_(row), column_(column), cells_(row * column), adjacentMineCount_(row * column)
{
    const int cellCount = row * column;
    std::uniform_int_distribution<int> distribution{0, cellCount - 1};
    auto generator = [&] { return distribution(mt); };
    for(int i = 0; i < mine; ++i) {
        while(true) {
            auto target = generator();
            if(cells_[target].isMine()) {
                continue;
            }
            cells_[target].setElement(CellElement::Mine);
            break;
        }
    }

    for(size_t i = 0; i < cells_.size(); ++i) {
        const auto position = indexToPosition(i);
        const auto row = position.first;
        const auto column = position.second;
        if(getCell(row, column)->isMine()) {
            continue;
        }
        auto adjacentCells = getAdjacentCells(row, column);
        for(auto&& cell : adjacentCells) {
            if(cell && cell->isMine()) {
                ++adjacentMineCount_[i];
            }
        }
    }
}

int MinesweeperModel::positionToIndex(int row, int column) const
{
    return column_ * row + column;
}

MinesweeperModel::Position MinesweeperModel::indexToPosition(int index) const
{
    return std::make_pair(index / column_, index % column_);
}

Cell* MinesweeperModel::getCell(int row, int column)
{
    auto index = positionToIndex(row, column);
    if(index < 0 || row_ * column_ <= index) {
        return nullptr;
    }
    return &cells_[index];
}

std::vector<Cell*> MinesweeperModel::getAdjacentCells(int row, int column)
{
    const bool isTopEdge = row == 0;
    const bool isBottomEdge = row == row_ - 1;
    const bool isLeftEdge = column == 0;
    const bool isRightEdge = column == column_ - 1;

    std::vector<Cell*> v;
    if(!isTopEdge && !isLeftEdge) {
        v.push_back(getCell(row - 1, column - 1));
    } else {
        v.push_back(nullptr);
    }
    if(!isTopEdge) {
        v.push_back(getCell(row - 1, column));
    } else {
        v.push_back(nullptr);
    }
    if(!isTopEdge && !isRightEdge) {
        v.push_back(getCell(row - 1, column + 1));
    } else {
        v.push_back(nullptr);
    }
    if(!isLeftEdge) {
        v.push_back(getCell(row, column - 1));
    } else {
        v.push_back(nullptr);
    }
    if(!isRightEdge) {
        v.push_back(getCell(row, column + 1));
    } else {
        v.push_back(nullptr);
    }
    if(!isBottomEdge && !isLeftEdge) {
        v.push_back(getCell(row + 1, column - 1));
    } else {
        v.push_back(nullptr);
    }
    if(!isBottomEdge) {
        v.push_back(getCell(row + 1, column));
    } else {
        v.push_back(nullptr);
    }
    if(!isBottomEdge && !isRightEdge) {
        v.push_back(getCell(row + 1, column + 1));
    } else {
        v.push_back(nullptr);
    }
    return v;
}
} // namespace MS
