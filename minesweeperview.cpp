#include "minesweeperview.h"
#include "ui_minesweeperview.h"

namespace MS {
MinesweeperView::MinesweeperView(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::MinesweeperView)
{
    ui_->setupUi(this);

}

MinesweeperView::~MinesweeperView()
{
}
} // namespace MS
