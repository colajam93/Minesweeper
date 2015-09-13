#include "minesweepermodel.h"
#include <random>
#include <cassert>
#include <iterator>
#include <algorithm>

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

MinesweeperModel::MinesweeperModel(int row, int column, int mine)
    : row_(row), column_(column), mine_(mine), cells_(row * column),
      adjacentMineCount_(row * column)
{
}

void MinesweeperModel::initialize(int clickedRow, int clickedColumn)
{
    for(auto&& cell : cells_) {
        cell = Cell{};
    }
    for(auto&& i : adjacentMineCount_) {
        i = 0;
    }

    auto excludePositions = getAdjacentPositions({clickedRow, clickedColumn});
    excludePositions.emplace_back(clickedRow, clickedColumn);
    std::uniform_int_distribution<int> distribution{0, row_ * column_ - 1};
    auto generator = [&] { return distribution(mt); };
    for(int i = 0; i < mine_; ++i) {
        while(true) {
            auto target = generator();
            bool excludeTarget = false;
            for(auto&& exclude : excludePositions) {
                if(target == positionToIndex(exclude)) {
                    excludeTarget = true;
                    break;
                }
            }
            if(excludeTarget) {
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
        auto adjacentPositions = getAdjacentPositions(position);
        for(auto&& pos : adjacentPositions) {
            const auto cell = getCellInfo(pos);
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

bool MinesweeperModel::isSucceeded() const
{
    auto closed = std::count_if(std::begin(cells_), std::end(cells_), [this](const Cell& cell)
    {
        return !cell.isOpened();
    });
    return closed == mine_;
}

int MinesweeperModel::getRow() const
{
    return row_;
}

int MinesweeperModel::getColumn() const
{
    return column_;
}

void MinesweeperModel::setUninitialized()
{
    isInitialized_ = false;
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

std::vector<Position> MinesweeperModel::getAdjacentPositions(const Position& position)
{
    const auto row = position.row;
    const auto column = position.column;
    const bool isTopEdge = row == 0;
    const bool isBottomEdge = row == (row_ - 1);
    const bool isLeftEdge = column == 0;
    const bool isRightEdge = column == (column_ - 1);

    std::vector<Position> v;
    if(!isTopEdge && !isLeftEdge) {
        v.emplace_back(row - 1, column - 1);
    }
    if(!isTopEdge) {
        v.emplace_back(row - 1, column);
    }
    if(!isTopEdge && !isRightEdge) {
        v.emplace_back(row - 1, column + 1);
    }
    if(!isLeftEdge) {
        v.emplace_back(row, column - 1);
    }
    if(!isRightEdge) {
        v.emplace_back(row, column + 1);
    }
    if(!isBottomEdge && !isLeftEdge) {
        v.emplace_back(row + 1, column - 1);
    }
    if(!isBottomEdge) {
        v.emplace_back(row + 1, column);
    }
    if(!isBottomEdge && !isRightEdge) {
        v.emplace_back(row + 1, column + 1);
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

    const auto adjacentPositions = getAdjacentPositions(targetCellPosition);
    for(auto&& pos: adjacentPositions) {
        const auto cellPointer = getCellInfo(pos).first;
        if(cellPointer && cellPointer->isMine()) {
            return v;
        }
    }
    for(auto&& pos: adjacentPositions) {
        auto part = open(pos.row, pos.column);
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

std::vector<CellChange> MinesweeperModel::autoOpen(int row, int column)
{
    auto targetCellInfo = getCellInfo(row, column);
    if(!targetCellInfo.first->isOpened()) {
        return {};
    }
    auto adjacentPositions = getAdjacentPositions({row, column});
    int flaggedCount = 0;
    for(auto&& pos : adjacentPositions) {
        auto info = getCellInfo(pos);
        if(info.first->isFlagged()) {
            ++flaggedCount;
        }
    }
    if(flaggedCount == adjacentMineCount_[positionToIndex({row, column})]) {
        std::vector<CellChange> v;
        for(auto&& pos : adjacentPositions) {
            auto part = open(pos.row, pos.column);
            v.insert(std::end(v), std::make_move_iterator(std::begin(part)), std::make_move_iterator(std::end(part)));
        }
        return v;
    }
    return {};
}
} // namespace MS