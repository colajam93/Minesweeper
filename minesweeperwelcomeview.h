#ifndef MINESWEEPERWELCOMEVIEW_H
#define MINESWEEPERWELCOMEVIEW_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class MinesweeperWelcomeView;
} // namespace Ui

namespace MS {
class MinesweeperWelcomeView : public QWidget {
    Q_OBJECT

    QScopedPointer<Ui::MinesweeperWelcomeView> ui_;

public:
    explicit MinesweeperWelcomeView(QWidget* parent = nullptr);
    ~MinesweeperWelcomeView();

signals:
    void quit();
    void start(int row, int column, int mines);
};
} // namespace MS

#endif // MINESWEEPERWELCOMEVIEW_H
