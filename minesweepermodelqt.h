#ifndef MINESWEEPERMODELQT_H
#define MINESWEEPERMODELQT_H

#include <QObject>
#include <QString>
#include <memory>
#include <QTime>

#include "minesweepermodel.h"
#include "minesweepercommon.h"

namespace MS {
class MinesweeperModelQt : public QObject
{
    Q_OBJECT

    std::unique_ptr<MinesweeperModel> model_;
    QTime time_;
    void open(int row, int column);
    void nextState(int row, int column);
    void autoOpen(int row, int column);
    void onFinish(bool isSucceeded);

public:
    explicit MinesweeperModelQt(QObject* parent = nullptr);

signals:
    void initView(int row, int column);
    void updateView(std::vector<CellChange> changes);
    void updateTime(QString time);
    void finish(bool isSucceeded, QString time);

public slots:
    void clicked(int row, int column, ClickType type);
    void start(int row, int column, int mine);
    void restart();
};
} // namespace MS

#endif // MINESWEEPERMODELQT_H
