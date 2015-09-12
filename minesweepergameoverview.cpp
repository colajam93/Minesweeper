#include "minesweepergameoverview.h"
#include "ui_minesweepergameoverview.h"

MinesweeperGameoverView::MinesweeperGameoverView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MinesweeperGameoverView)
{
    ui->setupUi(this);
}

MinesweeperGameoverView::~MinesweeperGameoverView()
{
    delete ui;
}
