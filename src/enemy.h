#ifndef ENEMY_H
#define ENEMY_H
#include <QtGui/QGraphicsItem>
#include "creature.h"

class Enemy : public Creature
{
public:
    Enemy(QGraphicsItem * myParent);
    // virtual ~Enemy();

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
    //bool myIsCaught; // is the enemy caught by the user's line?
    //bool myAmIDead;
};

#endif
