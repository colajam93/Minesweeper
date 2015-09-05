#ifndef MINESWEEPERMODELQT_H
#define MINESWEEPERMODELQT_H

#include <QObject>

#include "minesweepermodel.h"

namespace MS {
class MinesweeperModelQt : public QObject
{
    Q_OBJECT

    MinesweeperModel model_;

public:
    explicit MinesweeperModelQt(QObject* parent = nullptr, int row = 0, int column = 0, int mine = 0);

signals:
    void initView(int row, int column);
    void updateView(std::vector<CellChange> changes);

public slots:
    void open(int row, int column);
};
} // namespace MS

#endif // MINESWEEPERMODELQT_H
