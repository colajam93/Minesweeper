#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QWidget>
#include <QScopedPointer>
#include <QGraphicsRectItem>
#include <QObject>

namespace Ui {
class MinesweeperView;
} // namespace Ui

namespace MS {

class CellRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT

public:
    CellRectItem(int row, int column);
};

class MinesweeperView : public QWidget {
{
    Q_OBJECT

    QScopedPointer<Ui::MinesweeperView> ui_;

public:
    explicit MinesweeperView(QWidget* parent = nullptr);
    ~MinesweeperView();

public slots:
    void initView(int row, int column);
};
} // namespace MS

#endif // MINESWEEPERVIEW_H
