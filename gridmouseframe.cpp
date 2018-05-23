#include "gridmouseframe.h"
#include <QDebug>
#include "view.h"

GridMouseFrame::GridMouseFrame()
{
    setMouseTracking(true);
    gridFrame = new GridMouseFrame();
    View::scene()->addItem(gridFrame);
    gridFrame->setPos(0,0);
    gridFrame1 = new GridMouseFrame();
    View::scene()->addItem(gridFrame1);
    gridFrame1->setPos(View::sirkaGrid + View::mezeraGrid,0);
}

QRectF GridMouseFrame::boundingRect() const
{
}

//void GridMouseFrame::mouseMoveEvent(QMouseEvent *event)
//{
//    qDebug() << event->pos();
//}

QPainterPath GridMouseFrame::shape() const
{
}

void GridMouseFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
}
