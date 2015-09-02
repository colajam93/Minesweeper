#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QWidget>

namespace Ui {
class MinesweeperView;
}

class MinesweeperView : public QWidget
{
    Q_OBJECT

public:
    explicit MinesweeperView(QWidget *parent = 0);
    ~MinesweeperView();

private:
    Ui::MinesweeperView *ui;
};

#endif // MINESWEEPERVIEW_H
