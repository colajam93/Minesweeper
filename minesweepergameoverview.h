#ifndef MINESWEEPERGAMEOVERVIEW_H
#define MINESWEEPERGAMEOVERVIEW_H

#include <QWidget>
#include <QScopedPointer>
#include <QString>

namespace Ui {
class MinesweeperGameoverView;
} // namespace Ui

namespace MS {
class MinesweeperGameoverView : public QWidget {
    Q_OBJECT

    QScopedPointer<Ui::MinesweeperGameoverView> ui_;

public:
    explicit MinesweeperGameoverView(QWidget* parent = 0);
    ~MinesweeperGameoverView();

signals:
    void quit();
    void restart();
    void menu();

public slots:
    void finish(bool isSucceeded, QString time);
};
} // namespace MS

#endif // MINESWEEPERGAMEOVERVIEW_H
