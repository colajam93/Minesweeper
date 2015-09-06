#ifndef MINESWEEPERMODELQT_H
#define MINESWEEPERMODELQT_H

#include <QObject>

#include "minesweepermodel.h"

namespace MS {
class MinesweeperModelQt : public QObject
{
    Q_OBJECT

    MinesweeperModel model_;
    void open(int row, int column);

public:
    explicit MinesweeperModelQt(QObject* parent = nullptr, int row = 0, int column = 0, int mine = 0);

signals:
    void initView(int row, int column);
    void updateView(std::vector<CellChange> changes);
    void finish(bool isSucceeded);

public slots:
    void clicked(int row, int column, Qt::MouseButton button);
};
} // namespace MS

#endif // MINESWEEPERMODELQT_H
