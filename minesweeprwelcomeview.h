#ifndef MINESWEEPRWELCOMEVIEW_H
#define MINESWEEPRWELCOMEVIEW_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class MinesweeprWelcomeView;
} // namespace Ui

namespace MS {
class MinesweeprWelcomeView : public QWidget {
    Q_OBJECT

    QScopedPointer<Ui::MinesweeprWelcomeView> ui_;

public:
    explicit MinesweeprWelcomeView(QWidget* parent = 0);
    ~MinesweeprWelcomeView();

signals:
    void quit();
    void start(int row, int column, int mines);
};
} // namespace MS

#endif // MINESWEEPRWELCOMEVIEW_H
