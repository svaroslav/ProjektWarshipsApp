#ifndef GRIDMOUSEFRAME_H
#define GRIDMOUSEFRAME_H
#include <QGraphicsItem>
#include <QKeyEvent>
#include <QMouseEvent>
#include <qevent.h>


class GridMouseFrame : public QGraphicsItem
{
public:
    GridMouseFrame();
    QRectF boundingRect() const;
//    void mouseMoveEvent(QMouseEvent *event);
protected:
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    GridMouseFrame *gridFrame;
    GridMouseFrame *gridFrame1;
};

#endif // GRIDMOUSEFRAME_H
