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

public:
    explicit MinesweeperView(QWidget* parent = nullptr);
    ~MinesweeperView();

private:
    QScopedPointer<Ui::MinesweeperView> ui_;
};
} // namespace MS

#endif // MINESWEEPERVIEW_H
