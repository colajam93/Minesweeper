#include <QObject>
#include <QApplication>
#include "minesweeperview.h"
#include "minesweepermodelqt.h"
#include "minesweeprwelcomeview.h"
#include "minesweepergameoverview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MS::MinesweeperModelQt model;
    MS::MinesweeperView w;
    MS::MinesweeprWelcomeView wv;
    MS::MinesweeperGameoverView gv;

    QObject::connect(&model, &MS::MinesweeperModelQt::initView, &w, &MS::MinesweeperView::initView);
    QObject::connect(&model, &MS::MinesweeperModelQt::updateView, &w, &MS::MinesweeperView::updateView);
    QObject::connect(&model, &MS::MinesweeperModelQt::finish, &gv, &MS::MinesweeperGameoverView::finish);
    QObject::connect(&model, &MS::MinesweeperModelQt::finish, &w, &MS::MinesweeperView::finish);
    QObject::connect(&w, &MS::MinesweeperView::clicked, &model, &MS::MinesweeperModelQt::clicked);
    QObject::connect(&w, &MS::MinesweeperView::quit, &a, &QApplication::quit);
    QObject::connect(&w, &MS::MinesweeperView::restart, &model, &MS::MinesweeperModelQt::restart);
    QObject::connect(&w, &MS::MinesweeperView::menu, &wv, &MS::MinesweeprWelcomeView::show);
    QObject::connect(&wv, &MS::MinesweeprWelcomeView::quit, &a, &QApplication::quit);
    QObject::connect(&wv, &MS::MinesweeprWelcomeView::start, &model, &MS::MinesweeperModelQt::start);
    QObject::connect(&wv, &MS::MinesweeprWelcomeView::start, &w, &MS::MinesweeperView::show);
    QObject::connect(&gv, &MS::MinesweeperGameoverView::quit, &a, &QApplication::quit);
    QObject::connect(&gv, &MS::MinesweeperGameoverView::restart, &model, &MS::MinesweeperModelQt::restart);
    QObject::connect(&gv, &MS::MinesweeperGameoverView::restart, &w, &MS::MinesweeperView::show);
    QObject::connect(&gv, &MS::MinesweeperGameoverView::menu, &wv, &MS::MinesweeprWelcomeView::show);
    wv.show();

    return a.exec();
}
