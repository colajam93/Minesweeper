#include "minesweeperview.h"
#include "ui_minesweeperview.h"

#include <QGraphicsScene>
#include <QSizeF>
#include <QRect>
#include <QBrush>
#include <QGraphicsRectItem>

namespace {
static constexpr float cellSize = 37.0f;
static constexpr float cellGap = 3.0f;
static constexpr float cellBase = cellSize + cellGap;
static const QSizeF cellQSizeF = {cellSize, cellSize};
static constexpr int cellBackgroundZValue = 0;
static constexpr int cellElementZValue = 1;

QPointF basePoint(int row, int column)
{
    return {cellBase * column + cellGap, cellBase * row + cellGap};
}
}

namespace MS {
CellRectItem::CellRectItem(int row, int column)
    : row_(row), column_(column)
{
    auto rect = new QGraphicsRectItem({basePoint(row, column), cellQSizeF});
    rect->setPen({Qt::darkCyan, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin});
    rect->setBrush(Qt::cyan);
    rect->setZValue(cellBackgroundZValue);
    addToGroup(rect);
}

void CellRectItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit clicked(row_, column_, event->button());
}

MinesweeperView::MinesweeperView(QWidget* parent) :
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
            auto cellRectItem = new CellRectItem{i, j};
            connect(cellRectItem, &CellRectItem::clicked, this, &MinesweeperView::clicked);
            scene->addItem(cellRectItem);
        }
    }

    QBrush backgroundBrush{{153, 204, 255}};
    scene->setBackgroundBrush(backgroundBrush);

    ui_->graphicsView->hide();
    ui_->graphicsView->setScene(scene);
    ui_->graphicsView->show();
}
} // namespace MS
