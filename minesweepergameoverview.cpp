#include "minesweepergameoverview.h"
#include "ui_minesweepergameoverview.h"

namespace MS {
MinesweeperGameoverView::MinesweeperGameoverView(QWidget* parent) :
    QWidget(parent),
    ui_(new Ui::MinesweeperGameoverView)
{
    ui_->setupUi(this);
}

MinesweeperGameoverView::~MinesweeperGameoverView()
{
}
} // namespace MS

