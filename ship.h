#ifndef SHIP_H
#define SHIP_H
#include <QGraphicsItem>


class ship : public QGraphicsItem
{
public:
    ship(QString nazev,QColor barva,int sirka, int vyska);
    QRectF boundingRect() const;

protected:
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    QColor barva;
    QString nazev;
    int sirka;
    int vyska;
};

#endif // SHIP_H
