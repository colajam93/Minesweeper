#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QWidget>
#include <QScopedPointer>
#include <QObject>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItemGroup>

namespace Ui {
class MinesweeperView;
} // namespace Ui

namespace MS {

class CellRectItem : public QObject, public QGraphicsItemGroup {
    Q_OBJECT

    int row_;
    int column_;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);

public:
    CellRectItem(int row, int column);

signals:
    void clicked(int row, int column, Qt::MouseButton button);
};

class MinesweeperView : public QWidget {
    Q_OBJECT

    QScopedPointer<Ui::MinesweeperView> ui_;

public:
    explicit MinesweeperView(QWidget* parent = nullptr);
    ~MinesweeperView();

public slots:
    void initView(int row, int column);

signals:
    void clicked(int row, int column, Qt::MouseButton button);
};
} // namespace MS

#endif // MINESWEEPERVIEW_H
