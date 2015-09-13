#include "minesweepermodelqt.h"

#include <QTimer>

namespace {
QTimer& getTimer()
{
    static QTimer timer;
    return timer;
}
}

namespace MS {
MinesweeperModelQt::MinesweeperModelQt(QObject* parent)
    : QObject(parent), time_()
{
    connect(&getTimer(), &QTimer::timeout, [&]
    {
        time_ = time_.addSecs(1);
        emit updateTime(time_.toString());
    });
}

void MinesweeperModelQt::open(int row, int column)
{
    auto changes = model_->open(row, column);
    emit updateView(changes);
    for(auto&& change: changes) {
        if(change.first == CellView::Mine) {
            onFinish(false);
            return;
        }
    }
    if(model_->isSucceeded()) {
        onFinish(true);
    }
}

void MinesweeperModelQt::nextState(int row, int column)
{
    auto change = model_->nextState(row, column);
    emit updateView(change);
}

void MinesweeperModelQt::autoOpen(int row, int column)
{
    auto changes = model_->autoOpen(row, column);
    emit updateView(changes);
    for(auto&& change: changes) {
        if(change.first == CellView::Mine) {
            onFinish(false);
            return;
        }
    }
    if(model_->isSucceeded()) {
        onFinish(true);
    }
}

void MinesweeperModelQt::onFinish(bool isSucceeded)
{
    emit finish(isSucceeded, time_.toString());
    getTimer().stop();
    time_ = QTime{0, 0};
}

void MinesweeperModelQt::clicked(int row, int column, ClickType type)
{
    if(!model_->isInitialized()) {
        model_->initialize(row, column);
        getTimer().start(1000);
        time_ = QTime{0, 0};
    }
    if(type == ClickType::Open) {
        open(row, column);
    } else if(type == ClickType::NextState) {
        nextState(row, column);
    } else if(type == ClickType::AutoOpen) {
        autoOpen(row, column);
    }
}

void MinesweeperModelQt::start(int row, int column, int mine)
{
    model_ = std::make_unique<MinesweeperModel>(row, column, mine);
    getTimer().stop();
    emit initView(row, column);
}

void MinesweeperModelQt::restart()
{
    model_->setUninitialized();
    getTimer().stop();
    emit initView(model_->getRow(), model_->getColumn());
}
} // namespace MS
