#include "minesweeperview.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MinesweeperView w;
    w.show();

    return a.exec();
}
