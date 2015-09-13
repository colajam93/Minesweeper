#include "minesweeperview.h"
#include "ui_minesweeperview.h"

#include <QGraphicsScene>
#include <QSizeF>
#include <QRect>
#include <QBrush>
#include <QGraphicsRectItem>
#include <QGraphicsSimpleTextItem>
#include <QPushButton>
#include <QFont>

namespace {
static constexpr float cellSize = 37.0f;
static constexpr float cellGap = 3.0f;
static constexpr float cellBase = cellSize + cellGap;
static const QSizeF cellQSizeF = {cellSize, cellSize};
static constexpr int cellBackgroundZValue = 0;
static const QBrush cellBrush{Qt::cyan};

const QFont& getTextFont()
{
    static QFont font;
    font.setPointSize(25);
    return font;
}

QPointF basePoint(int row, int column)
{
    return {cellBase * column + cellGap, cellBase * row + cellGap};
}

QGraphicsSimpleTextItem* getTextItem(MS::CellView view, MS::Position position) {
    using CellView = MS::CellView;
    auto text = new QGraphicsSimpleTextItem;
    text->setPos(basePoint(position.row, position.column));
    text->setFont(getTextFont());
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
    } else if(view == CellView::Flag) {
        text->setText("F");
    } else if(view == CellView::Doubt) {
        text->setText("?");
    }
    return text;
}
}

namespace MS {
CellRectItem::CellRectItem(int row, int column, CellView view)
    : QGraphicsRectItem({basePoint(row, column), cellQSizeF}), row_(row), column_(column)
{
    setZValue(cellBackgroundZValue);
    if(view == CellView::None || view == CellView::Flag || view == CellView::Doubt) {
        setPen({Qt::darkCyan, 1.5, Qt::SolidLine, Qt::SquareCap, Qt::RoundJoin});
        setBrush(cellBrush);
    } else {
        setPen({Qt::NoPen});
        setBrush({});
    }
    if(view != CellView::None) {
        auto text = getTextItem(view, {row, column});
        text->setParentItem(this);
    }
}

void CellRectItem::mousePressEvent(QGraphicsSceneMouseEvent*)
{
}

void CellRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent*)
{
    if(!(brush() == QBrush{})) {
        auto tempBrush = cellBrush;
        tempBrush.setColor(tempBrush.color().darker());
        setBrush(tempBrush);
    }
}

void CellRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(!(brush() == QBrush{})) {
        setBrush(cellBrush);
    }

    if(!boundingRect().contains(event->pos())) {
        return;
    }
    auto button = event->button();
    if(button == Qt::LeftButton) {
        emit clicked(row_, column_, ClickType::Open);
    } else if(button == Qt::RightButton) {
        emit clicked(row_, column_, ClickType::NextState);
    } else if(button == Qt::MidButton) {
        emit clicked(row_, column_, ClickType::AutoOpen);
    }
}

void CellRectItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent*)
{
    if(!(brush() == QBrush{})) {
        setBrush(cellBrush);
    }

    emit clicked(row_, column_, ClickType::AutoOpen);
}

MinesweeperView::MinesweeperView(QWidget* parent) :
    QWidget(parent),
    ui_(new Ui::MinesweeperView)
{
    ui_->setupUi(this);
    connect(ui_->quitButton, &QPushButton::pressed, this, &MinesweeperView::quit);
    connect(ui_->restartButton, &QPushButton::pressed, this, &MinesweeperView::restart);
    connect(ui_->menuButton, &QPushButton::pressed, [this]
    {
        hide();
        emit menu();
    });
}

MinesweeperView::~MinesweeperView()
{
}

void MinesweeperView::initView(int row, int column)
{
    hide();
    const QRect rect{0, 0, static_cast<int>(cellBase * column + cellGap), static_cast<int>(cellBase * row + cellGap)};
    ui_->graphicsView->setSceneRect(rect);
    constexpr int widthSizeOffset = 22 * 2;
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
    if(auto oldScene = ui_->graphicsView->scene()) {
        delete oldScene;
    }

    QBrush backgroundBrush{{153, 204, 255}};
    scene->setBackgroundBrush(backgroundBrush);

    ui_->graphicsView->setScene(scene);
    ui_->graphicsView->show();
    ui_->timeLabel->setText("00:00:00");
    setEnabled(true);
    show();
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
        } else {
            auto changed = new CellRectItem(position.row, position.column, view);
            scene->addItem(changed);
            connect(changed, &CellRectItem::clicked, this, &MinesweeperView::clicked);
        }
    }
}

void MinesweeperView::updateTime(QString time)
{
    ui_->timeLabel->setText(time);
}

void MinesweeperView::finish(bool)
{
    setDisabled(true);
}
} // namespace MS
