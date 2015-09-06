#include "minesweeperview.h"
#include "ui_minesweeperview.h"

#include <QGraphicsScene>
#include <QSizeF>
#include <QRect>
#include <QRectF>

namespace {
static constexpr float cellSize = 37.0f;
static constexpr float cellGap = 3.0f;
static constexpr float cellBase = cellSize + cellGap;
static const QSizeF cellQSizeF = {cellSize, cellSize};

QPointF basePoint(int row, int column)
{
    return {cellBase * row + cellGap, cellBase * column + cellGap};
}
}

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

void MinesweeperView::initView(int row, int column)
{
    const QRect rect{0, 0, static_cast<int>(cellBase * column + cellGap), static_cast<int>(cellBase * row + cellGap)};
    ui_->graphicsView->setSceneRect(rect);
    constexpr int widthSizeOffset = 22;
    constexpr int heightSizeOffset = 25;
    setGeometry(0, 0, rect.width() + ui_->titleLabel->width() + widthSizeOffset, rect.height() + heightSizeOffset);
    auto scene = new QGraphicsScene(rect);
    for(int i = 0; i < row; ++i) {
        for(int j = 0; j < column; ++j) {
            scene->addRect(QRectF{basePoint(j, i), cellQSizeF});
        }
    }

    ui_->graphicsView->setScene(scene);
}
} // namespace MS
