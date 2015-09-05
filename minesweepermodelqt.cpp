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
} // namespace MS
