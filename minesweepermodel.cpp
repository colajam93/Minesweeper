#include "minesweepermodel.h"
#include <random>
#include <cassert>
#include <iterator>
#include <algorithm>
#include <queue>

namespace {
static std::random_device rd;
static std::mt19937 mt{rd()};

MS::CellView getCellView(int count)
{
    using CellView = MS::CellView;
    if (count == 0) {
        return CellView::Zero;
    }
    else if (count == 1) {
        return CellView::One;
    }
    else if (count == 2) {
        return CellView::Two;
    }
    else if (count == 3) {
        return CellView::Three;
    }
    else if (count == 4) {
        return CellView::Four;
    }
    else if (count == 5) {
        return CellView::Five;
    }
    else if (count == 6) {
        return CellView::Six;
    }
    else if (count == 7) {
        return CellView::Seven;
    }
    else if (count == 8) {
        return CellView::Eight;
    }
    return CellView::Zero;
}
}

namespace MS {
Cell::Cell()
    : state_(CellState::None)
    , element_(CellElement::Empty)
    , opened_(false)
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
    if (state_ == CellState::None) {
        state_ = CellState::Flag;
    }
    else if (state_ == CellState::Flag) {
        state_ = CellState::Doubt;
    }
    else if (state_ == CellState::Doubt) {
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
    : row_(row)
    , column_(column)
    , mine_(mine)
    , cells_(row * column)
    , adjacentMineCount_(row * column)
{
}

void MinesweeperModel::initialize(int clickedRow, int clickedColumn)
{
    // initialize
    for (auto&& cell : cells_) {
        cell = Cell{};
    }
    for (auto&& i : adjacentMineCount_) {
        i = 0;
    }

    // set mine
    {
        auto excludePositions =
            getAdjacentPositions({clickedRow, clickedColumn});
        excludePositions.emplace_back(clickedRow, clickedColumn);
        std::uniform_int_distribution<int> distribution{0, row_ * column_ - 1};
        auto generator = [&] { return distribution(mt); };
        for (int i = 0; i < mine_; ++i) {
            while (true) {
                auto target = generator();

                // exclude clicked and its adjacent position
                bool excludeTarget = false;
                for (auto&& exclude : excludePositions) {
                    if (target == positionToIndex(exclude)) {
                        excludeTarget = true;
                        break;
                    }
                }
                if (excludeTarget) {
                    continue;
                }

                // set mine if cell has not set mine
                auto& cell = getCell(indexToPosition(target));
                if (!cell.isMine()) {
                    cell.setMine();
                    break;
                }
            }
        }
    }

    // count adjacent mine
    for (int i = 0; i < row_ * column_; ++i) {
        const auto position = indexToPosition(i);
        if (getCell(position).isMine()) {
            continue;
        }
        auto adjacentPositions = getAdjacentPositions(position);
        for (auto&& pos : adjacentPositions) {
            if (getCell(pos).isMine()) {
                ++adjacentMineCount_[i];
            }
        }
    }

    // initialize complete
    isInitialized_ = true;
}

bool MinesweeperModel::isInitialized() const
{
    return isInitialized_;
}

bool MinesweeperModel::isSucceeded() const
{
    auto closed =
        std::count_if(std::begin(cells_), std::end(cells_),
                      [this](const Cell& cell) { return !cell.isOpened(); });
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

Cell& MinesweeperModel::getCell(const Position& position)
{
    return const_cast<Cell&>(
        static_cast<const MinesweeperModel&>(*this).getCell(position));
}

const Cell& MinesweeperModel::getCell(const Position& position) const
{
    auto index = positionToIndex(position);
    assert(0 <= index && index < row_ * column_);
    return cells_[index];
}

std::vector<Position>
MinesweeperModel::getAdjacentPositions(const Position& position)
{
    const auto row = position.row;
    const auto column = position.column;
    const bool isTopEdge = row == 0;
    const bool isBottomEdge = row == (row_ - 1);
    const bool isLeftEdge = column == 0;
    const bool isRightEdge = column == (column_ - 1);

    std::vector<Position> v;
    if (!isTopEdge && !isLeftEdge) {
        v.emplace_back(row - 1, column - 1);
    }
    if (!isTopEdge) {
        v.emplace_back(row - 1, column);
    }
    if (!isTopEdge && !isRightEdge) {
        v.emplace_back(row - 1, column + 1);
    }
    if (!isLeftEdge) {
        v.emplace_back(row, column - 1);
    }
    if (!isRightEdge) {
        v.emplace_back(row, column + 1);
    }
    if (!isBottomEdge && !isLeftEdge) {
        v.emplace_back(row + 1, column - 1);
    }
    if (!isBottomEdge) {
        v.emplace_back(row + 1, column);
    }
    if (!isBottomEdge && !isRightEdge) {
        v.emplace_back(row + 1, column + 1);
    }
    return v;
}

std::vector<CellChange> MinesweeperModel::open(int row, int column)
{
    auto& targetCell = getCell({row, column});

    // cell has already opened or flagged
    if (targetCell.isOpened() || targetCell.isFlagged()) {
        // do nothing
        return {};
    }

    // open cell
    targetCell.setOpened();

    // mine
    if (targetCell.isMine()) {
        return {std::make_pair(CellView::Mine, Position{row, column})};
    }

    auto getCellChange = [this](const Position& position) {
        return std::make_pair(
            getCellView(adjacentMineCount_[positionToIndex(position)]),
            position);
    };
    std::queue<Position> processQueue;
    std::vector<CellChange> v;
    processQueue.push({row, column});
    while (!processQueue.empty()) {
        const auto& current = processQueue.front();
        v.emplace_back(getCellChange(current));
        if (adjacentMineCount_[positionToIndex(current)] == 0) {
            for (auto&& pos : getAdjacentPositions(current)) {
                if (!getCell(pos).isOpened()) {
                    processQueue.push(pos);
                    getCell(pos).setOpened();
                }
            }
        }
        processQueue.pop();
    }

    return v;
}

std::vector<CellChange> MinesweeperModel::nextState(int row, int column)
{
    auto& targetCell = getCell({row, column});
    if (targetCell.isOpened()) {
        return {};
    }
    auto nextState = targetCell.setNextState();
    if (nextState == CellState::None) {
        return {std::make_pair(CellView::None, Position{row, column})};
    }
    else if (nextState == CellState::Flag) {
        return {std::make_pair(CellView::Flag, Position{row, column})};
    }
    else if (nextState == CellState::Doubt) {
        return {std::make_pair(CellView::Doubt, Position{row, column})};
    }
    return {};
}

std::vector<CellChange> MinesweeperModel::autoOpen(int row, int column)
{
    const auto& targetCell = getCell({row, column});
    if (!targetCell.isOpened()) {
        return {};
    }
    const auto adjacentPositions = getAdjacentPositions({row, column});
    int flaggedCount = 0;
    for (auto&& pos : adjacentPositions) {
        const auto& info = getCell(pos);
        if (info.isFlagged()) {
            ++flaggedCount;
        }
    }
    if (flaggedCount == adjacentMineCount_[positionToIndex({row, column})]) {
        std::vector<CellChange> v;
        for (auto&& pos : adjacentPositions) {
            auto part = open(pos.row, pos.column);
            v.insert(std::end(v), std::make_move_iterator(std::begin(part)),
                     std::make_move_iterator(std::end(part)));
        }
        return v;
    }
    return {};
}
} // namespace MS
