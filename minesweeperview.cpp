#include "minesweeperview.h"
#include "ui_minesweeperview.h"

#include <QGraphicsScene>
#include <QSizeF>
#include <QRect>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QMessageBox>

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

QGraphicsSimpleTextItem* changedCellView(MS::CellView view, MS::Position position) {
    using CellView = MS::CellView;
    auto text = new QGraphicsSimpleTextItem;
    text->setPos(basePoint(position.row, position.column));
    auto font = text->font();
    font.setPointSize(25);
    text->setFont(font);
    if(view == CellView::Mine) {
        text->setText("*");
    } else if(view == CellView::One) {
        text->setText("1");
    } else if(view == CellView::Two) {
        text->setText("2");
    } else if(view == CellView::Three) {
        text->setText("3");
    } else if(view == CellView::Four) {
        text->setText("4");
    } else if(view == CellView::Five) {
        text->setText("5");
    } else if(view == CellView::Six) {
        text->setText("6");
    } else if(view == CellView::Seven) {
        text->setText("7");
    } else if(view == CellView::Eight) {
        text->setText("8");
    }
    return text;
}
}

namespace MS {
CellRectItem::CellRectItem(int row, int column, CellState state)
    : row_(row), column_(column)
{
    auto rect = new QGraphicsRectItem({basePoint(row, column), cellQSizeF});
    rect->setPen({Qt::darkCyan, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin});
    rect->setBrush(Qt::cyan);
    rect->setZValue(cellBackgroundZValue);
    addToGroup(rect);
    if(state != CellState::None) {
        auto text = new QGraphicsSimpleTextItem;
        text->setPos(basePoint(row, column));
        auto font = text->font();
        font.setPointSize(25);
        text->setFont(font);
        if(state == CellState::Flag) {
            text->setText("F");
        } else if(state == CellState::Doubt) {
            text->setText("?");
        }
        addToGroup(text);
    }
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

void MinesweeperView::updateView(std::vector<CellChange> changes)
{
    for(auto&& change: changes) {
        const auto& view = change.first;
        const auto& position = change.second;
        auto scene = ui_->graphicsView->scene();
        auto targets = scene->items(basePoint(position.row, position.column));
        for(auto&& target: targets) {
            scene->removeItem(target);
            delete target;
        }
        if(view == CellView::Zero) {
            continue;
        } else if(view == CellView::None) {
            auto changed = new CellRectItem(position.row, position.column);
            scene->addItem(changed);
            connect(changed, &CellRectItem::clicked, this, &MinesweeperView::clicked);
        } else if(view == CellView::Flag) {
            auto changed = new CellRectItem(position.row, position.column, CellState::Flag);
            scene->addItem(changed);
            connect(changed, &CellRectItem::clicked, this, &MinesweeperView::clicked);
        } else if(view == CellView::Doubt) {
            auto changed = new CellRectItem(position.row, position.column, CellState::Doubt);
            scene->addItem(changed);
            connect(changed, &CellRectItem::clicked, this, &MinesweeperView::clicked);
        } else {
            auto changedTarget = changedCellView(view, position);
            scene->addItem(changedTarget);
        }
    }
}

void MinesweeperView::finish(bool isSucceeded)
{
    if(isSucceeded) {

    } else {
        auto message = new QMessageBox(QMessageBox::Information, "Failed", "Failed", QMessageBox::Ok);
        message->exec();
        emit quit();
    }
}
} // namespace MS
