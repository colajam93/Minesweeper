#include "minesweepermodel.h"
#include <random>
#include <cassert>
#include <iterator>

namespace {
static std::random_device rd;
static std::mt19937 mt{rd()};

MS::CellView getCellView(int count)
{
    using CellView = MS::CellView;
    if(count == 0) {
        return CellView::Zero;
    } else if(count == 1) {
        return CellView::One;
    } else if(count == 2) {
        return CellView::Two;
    } else if(count == 3) {
        return CellView::Three;
    } else if(count == 4) {
        return CellView::Four;
    } else if(count == 5) {
        return CellView::Five;
    } else if(count == 6) {
        return CellView::Six;
    } else if(count == 7) {
        return CellView::Seven;
    } else if(count == 8) {
        return CellView::Eight;
    }
    return CellView::Zero;
}
}

namespace MS {
Cell::Cell()
    : state_(CellState::None), element_(CellElement::Empty), opened_(false)
{
}

void Cell::setMine()
{
    element_ = CellElement::Mine;
}

void Cell::setOpened()
{
    opened_ = true;
}

CellState Cell::setNextState()
{
    if(state_ == CellState::None) {
        state_ = CellState::Flag;
    } else if(state_ == CellState::Flag) {
        state_ = CellState::Doubt;
    } else if(state_ == CellState::Doubt) {
        state_ = CellState::None;
    }
    return state_;
}

bool Cell::isMine() const
{
    return element_ == CellElement::Mine;
}

bool Cell::isOpened() const
{
    return opened_;
}

bool Cell::isFlagged() const
{
    return state_ == CellState::Flag;
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
    : row_(row), column_(column), mine_(mine), cells_(row * column), adjacentMineCount_(row * column)
{
}

void MinesweeperModel::init(int clickedRow, int clickedColumn)
{
    const auto clickedIndex = positionToIndex({clickedRow, clickedColumn});
    std::uniform_int_distribution<int> distribution{0, row_ * column_ - 1};
    auto generator = [&] { return distribution(mt); };
    for(int i = 0; i < mine_; ++i) {
        while(true) {
            auto target = generator();
            if(target == clickedIndex) {
                continue;
            }
            if(cells_[target].isMine()) {
                continue;
            }
            cells_[target].setMine();
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
    isInitialized_ = true;
}

bool MinesweeperModel::isInitialized() const
{
    return isInitialized_;
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
    const bool isBottomEdge = row == (row_ - 1);
    const bool isLeftEdge = column == 0;
    const bool isRightEdge = column == (column_ - 1);

    std::vector<CellInfo> v;
    if(!isTopEdge && !isLeftEdge) {
        v.emplace_back(getCellInfo(row - 1, column - 1));
    }
    if(!isTopEdge) {
        v.emplace_back(getCellInfo(row - 1, column));
    }
    if(!isTopEdge && !isRightEdge) {
        v.emplace_back(getCellInfo(row - 1, column + 1));
    }
    if(!isLeftEdge) {
        v.emplace_back(getCellInfo(row, column - 1));
    }
    if(!isRightEdge) {
        v.emplace_back(getCellInfo(row, column + 1));
    }
    if(!isBottomEdge && !isLeftEdge) {
        v.emplace_back(getCellInfo(row + 1, column - 1));
    }
    if(!isBottomEdge) {
        v.emplace_back(getCellInfo(row + 1, column));
    }
    if(!isBottomEdge && !isRightEdge) {
        v.emplace_back(getCellInfo(row + 1, column + 1));
    }
    return v;
}

std::vector<CellChange> MinesweeperModel::open(int row, int column)
{
    auto targetCellInfo = getCellInfo(row, column);
    auto targetCell = targetCellInfo.first;
    if(targetCell->isOpened() || targetCell->isFlagged()) {
        return {};
    }

    targetCell->setOpened();
    if(targetCell->isMine()) {
        return {std::make_pair(CellView::Mine, Position{row, column})};
    }

    auto getCellChange = [&](const Position& position)
    {
        return std::make_pair(
            getCellView(adjacentMineCount_[positionToIndex(position)]),
            position
        );
    };

    auto targetCellPosition = targetCellInfo.second;
    std::vector<CellChange> v;
    v.emplace_back(getCellChange(targetCellPosition));

    auto adjacentCellInfos = getAdjacentCellInfos(targetCellPosition);
    for(auto&& cell: adjacentCellInfos) {
        const auto cellPointer = cell.first;
        if(cellPointer && cellPointer->isMine()) {
            return v;
        }
    }
    for(auto&& cell: adjacentCellInfos) {
        auto part = open(cell.second.row, cell.second.column);
        v.insert(std::end(v), std::make_move_iterator(std::begin(part)), std::make_move_iterator(std::end(part)));
    }
    return v;
}

std::vector<CellChange> MinesweeperModel::nextState(int row, int column)
{
    auto targetCellInfo = getCellInfo(row, column);
    if(targetCellInfo.first->isOpened()) {
        return {};
    }
    auto nextState = targetCellInfo.first->setNextState();
    if(nextState == CellState::None) {
        return {std::make_pair(CellView::None, targetCellInfo.second)};
    } else if(nextState == CellState::Flag) {
        return {std::make_pair(CellView::Flag, targetCellInfo.second)};
    } else if(nextState == CellState::Doubt) {
        return {std::make_pair(CellView::Doubt, targetCellInfo.second)};
    }
    return {};
}
} // namespace MS
