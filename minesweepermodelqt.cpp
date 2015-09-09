#include "minesweepermodelqt.h"


namespace MS {
MinesweeperModelQt::MinesweeperModelQt(QObject* parent)
    : QObject(parent)
{
}

void MinesweeperModelQt::open(int row, int column)
{
    auto changes = model_->open(row, column);
    emit updateView(changes);
    for(auto&& change: changes) {
        if(change.first == CellView::Mine) {
            emit finish(false);
        }
    }
    if(model_->isSucceeded()) {
        emit finish(true);
    }
}

void MinesweeperModelQt::nextState(int row, int column)
{
    auto change = model_->nextState(row, column);
    emit updateView(change);
}

void MinesweeperModelQt::clicked(int row, int column, Qt::MouseButton button)
{
    if(!model_->isInitialized()) {
        model_->initialize(row, column);
    }
    if(button == Qt::LeftButton) {
        open(row, column);
    } else if(button == Qt::RightButton) {
        nextState(row, column);
    }
}

void MinesweeperModelQt::start(int row, int column, int mine)
{
    model_ = std::make_unique<MinesweeperModel>(row, column, mine);
    emit initView(row, column);
}
} // namespace MS
