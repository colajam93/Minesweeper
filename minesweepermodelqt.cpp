#include "minesweepermodelqt.h"


namespace MS {
MinesweeperModelQt::MinesweeperModelQt(QObject* parent, int row, int column, int mine)
    : QObject(parent), model_(row, column, mine)
{
}

void MinesweeperModelQt::open(int row, int column)
{
    auto changes = model_.open(row, column);
    emit updateView(changes);
    for(auto&& change: changes) {
        if(change.first == CellView::Mine) {
            emit finish(false);
        }
    }
}

void MinesweeperModelQt::nextState(int row, int column)
{
    auto change = model_.nextState(row, column);
    emit updateView(change);
}

void MinesweeperModelQt::clicked(int row, int column, Qt::MouseButton button)
{
    if(button == Qt::LeftButton) {
        open(row, column);
    } else if(button == Qt::RightButton) {
        nextState(row, column);
    }
}
} // namespace MS
