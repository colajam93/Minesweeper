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
}

void MinesweeperModelQt::clicked(int row, int column, Qt::MouseButton button)
{
    if(button == Qt::LeftButton) {
        open(row, column);
    }
}
} // namespace MS
