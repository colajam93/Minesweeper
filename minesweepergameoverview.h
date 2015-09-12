#ifndef MINESWEEPERGAMEOVERVIEW_H
#define MINESWEEPERGAMEOVERVIEW_H

#include <QWidget>

namespace Ui {
class MinesweeperGameoverView;
}

class MinesweeperGameoverView : public QWidget
{
    Q_OBJECT

public:
    explicit MinesweeperGameoverView(QWidget *parent = 0);
    ~MinesweeperGameoverView();

private:
    Ui::MinesweeperGameoverView *ui;
};

#endif // MINESWEEPERGAMEOVERVIEW_H
