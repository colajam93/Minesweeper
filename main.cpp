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
    MS::MinesweeperView view;
    MS::MinesweeprWelcomeView welcomeView;
    MS::MinesweeperGameoverView gameoverView;

    QObject::connect(&model, &MS::MinesweeperModelQt::initView, &view, &MS::MinesweeperView::initView);
    QObject::connect(&model, &MS::MinesweeperModelQt::updateView, &view, &MS::MinesweeperView::updateView);
    QObject::connect(&model, &MS::MinesweeperModelQt::finish, &gameoverView, &MS::MinesweeperGameoverView::finish);
    QObject::connect(&model, &MS::MinesweeperModelQt::finish, &view, &MS::MinesweeperView::finish);
    QObject::connect(&view, &MS::MinesweeperView::clicked, &model, &MS::MinesweeperModelQt::clicked);
    QObject::connect(&view, &MS::MinesweeperView::quit, &a, &QApplication::quit);
    QObject::connect(&view, &MS::MinesweeperView::restart, &model, &MS::MinesweeperModelQt::restart);
    QObject::connect(&view, &MS::MinesweeperView::menu, &welcomeView, &MS::MinesweeprWelcomeView::show);
    QObject::connect(&welcomeView, &MS::MinesweeprWelcomeView::quit, &a, &QApplication::quit);
    QObject::connect(&welcomeView, &MS::MinesweeprWelcomeView::start, &model, &MS::MinesweeperModelQt::start);
    QObject::connect(&welcomeView, &MS::MinesweeprWelcomeView::start, &view, &MS::MinesweeperView::show);
    QObject::connect(&gameoverView, &MS::MinesweeperGameoverView::quit, &a, &QApplication::quit);
    QObject::connect(&gameoverView, &MS::MinesweeperGameoverView::restart, &model, &MS::MinesweeperModelQt::restart);
    QObject::connect(&gameoverView, &MS::MinesweeperGameoverView::menu, &welcomeView, &MS::MinesweeprWelcomeView::show);
    QObject::connect(&gameoverView, &MS::MinesweeperGameoverView::menu, &view, &MS::MinesweeperView::hide);
    welcomeView.show();

    return a.exec();
}
