#ifndef MINESWEEPERMODELQT_H
#define MINESWEEPERMODELQT_H

#include <QObject>
#include <memory>

#include "minesweepermodel.h"

namespace MS {
class MinesweeperModelQt : public QObject
{
    Q_OBJECT

    std::unique_ptr<MinesweeperModel> model_;
    void open(int row, int column);
    void nextState(int row, int column);

public:
    explicit MinesweeperModelQt(QObject* parent = nullptr);

signals:
    void initView(int row, int column);
    void updateView(std::vector<CellChange> changes);
    void finish(bool isSucceeded);

public slots:
    void clicked(int row, int column, Qt::MouseButton button);
    void start(int row, int column, int mine);
    void restart();
};
} // namespace MS

#endif // MINESWEEPERMODELQT_H
