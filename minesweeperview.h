#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QWidget>
#include <QScopedPointer>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>

#include "minesweepercommon.h"

namespace Ui {
class MinesweeperView;
} // namespace Ui

namespace MS {
class CellRectItem : public QObject, public QGraphicsRectItem {
    Q_OBJECT

    int row_;
    int column_;

    void mousePressEvent(QGraphicsSceneMouseEvent*);
    void mouseMoveEvent(QGraphicsSceneMouseEvent*);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent*);

public:
    CellRectItem(int row, int column, CellView view = CellView::None);

signals:
    void clicked(int row, int column, ClickType type);
};

class MinesweeperView : public QWidget {
    Q_OBJECT

    QScopedPointer<Ui::MinesweeperView> ui_;

public:
    explicit MinesweeperView(QWidget* parent = nullptr);
    ~MinesweeperView();

public slots:
    void initView(int row, int column);
    void updateView(std::vector<CellChange> changes);
    void finish(bool isSucceeded);

signals:
    void clicked(int row, int column, ClickType type);
    void quit();
    void restart();
    void menu();
};
} // namespace MS

#endif // MINESWEEPERVIEW_H
