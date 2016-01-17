#include "minesweepergameoverview.h"
#include "ui_minesweepergameoverview.h"

#include <QPushButton>

namespace MS {
MinesweeperGameoverView::MinesweeperGameoverView(QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::MinesweeperGameoverView)
{
    ui_->setupUi(this);

    connect(ui_->quitButton, &QPushButton::pressed, this,
            &MS::MinesweeperGameoverView::quit);
    connect(ui_->restartButton, &QPushButton::pressed, [this] {
        hide();
        emit restart();
    });
    connect(ui_->menuButton, &QPushButton::pressed, [this] {
        hide();
        emit menu();
    });
}

MinesweeperGameoverView::~MinesweeperGameoverView()
{
}

void MinesweeperGameoverView::finish(bool isSucceeded, QString time)
{
    ui_->timeLabel->setText(time);
    if (isSucceeded) {
        ui_->resultLabel->setText("Congratulations");
    }
    else {
        ui_->resultLabel->setText("Failed");
    }
    show();
}
} // namespace MS
