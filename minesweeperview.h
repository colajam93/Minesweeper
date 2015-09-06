#ifndef MINESWEEPERVIEW_H
#define MINESWEEPERVIEW_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class MinesweeperView;
} // namespace Ui

namespace MS {
class MinesweeperView : public QWidget
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
