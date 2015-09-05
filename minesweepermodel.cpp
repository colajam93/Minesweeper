#include "minesweepermodel.h"
#include <random>
#include <cassert>

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

Position::Position(int row, int column)
{
    row = row;
    column = column;
}

Position::Position(std::initializer_list<int> list)
{
    assert(list.size() == 2);
    auto it = std::begin(list);
    row = *it;
    ++it;
    column = *it;
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
        if(getCellInfo(position).first->isMine()) {
            continue;
        }
        auto adjacentCellInfos = getAdjacentCellInfos(position);
        for(auto&& cell : adjacentCellInfos) {
            if(cell.first && cell.first->isMine()) {
                ++adjacentMineCount_[i];
            }
        }
    }
}

int MinesweeperModel::positionToIndex(const Position& position) const
{
    return column_ * position.row + position.column;
}

Position MinesweeperModel::indexToPosition(int index) const
{
    return {index / column_, index % column_};
}

MinesweeperModel::CellInfo MinesweeperModel::getCellInfo(const Position& position)
{
    auto index = positionToIndex(position);
    if(index < 0 || row_ * column_ <= index) {
        return std::make_pair(nullptr, Position{});
    }
    return std::make_pair(&cells_[index], position);
}

MinesweeperModel::CellInfo MinesweeperModel::getCellInfo(int row, int column)
{
    return getCellInfo({row, column});
}

std::vector<MinesweeperModel::CellInfo> MinesweeperModel::getAdjacentCellInfos(const Position& position)
{
    const auto row = position.row;
    const auto column = position.column;
    const bool isTopEdge = row == 0;
    const bool isBottomEdge = row == row_ - 1;
    const bool isLeftEdge = column == 0;
    const bool isRightEdge = column == column_ - 1;

    const CellInfo invalid = std::make_pair(nullptr, Position{});
    std::vector<CellInfo> v;
    if(!isTopEdge && !isLeftEdge) {
        v.emplace_back(getCellInfo(row - 1, column - 1));
    } else {
        v.emplace_back(invalid);
    }
    if(!isTopEdge) {
        v.emplace_back(getCellInfo(row - 1, column));
    } else {
        v.emplace_back(invalid);
    }
    if(!isTopEdge && !isRightEdge) {
        v.emplace_back(getCellInfo(row - 1, column + 1));
    } else {
        v.emplace_back(invalid);
    }
    if(!isLeftEdge) {
        v.emplace_back(getCellInfo(row, column - 1));
    } else {
        v.emplace_back(invalid);
    }
    if(!isRightEdge) {
        v.emplace_back(getCellInfo(row, column + 1));
    } else {
        v.emplace_back(invalid);
    }
    if(!isBottomEdge && !isLeftEdge) {
        v.emplace_back(getCellInfo(row + 1, column - 1));
    } else {
        v.emplace_back(invalid);
    }
    if(!isBottomEdge) {
        v.emplace_back(getCellInfo(row + 1, column));
    } else {
        v.emplace_back(invalid);
    }
    if(!isBottomEdge && !isRightEdge) {
        v.emplace_back(getCellInfo(row + 1, column + 1));
    } else {
        v.emplace_back(invalid);
    }
    return v;
}
} // namespace MS
