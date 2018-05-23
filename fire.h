#ifndef FIRE_H
#define FIRE_H
#include <QGraphicsItem>


class Fire : public QGraphicsItem
{
public:
    Fire(QString nazev,QColor barva,int sirka, int vyska);
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

#endif // FIRE_H
