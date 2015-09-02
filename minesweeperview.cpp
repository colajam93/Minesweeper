#include "minesweeperview.h"
#include "ui_minesweeperview.h"

MinesweeperView::MinesweeperView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinesweeperView)
{
    ui->setupUi(this);
}

MinesweeperView::~MinesweeperView()
{
    delete ui;
}
