#ifndef ALLY_H
#define ALLY_H
#include <QtGui/QGraphicsItem>
#include "creature.h"

class Ally : public Creature
{
public:
    Ally(QGraphicsItem * myParent);
    // virtual ~Ally();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    //void setMaxSpeedMult(qreal maxspeedmult);

private:
    //qreal myAngle;
    //qreal mySpeed;
    //qreal myMaxSpeed;
    qreal myEyeDirection;
    QColor myColour;
    //bool myIsCaught; // is the Ally caught by the user's line?
    //bool myAmIDead;
};

#endif // ALLY_H
