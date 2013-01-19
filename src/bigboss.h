#ifndef BIGBOSS_H
#define BIGBOSS_H
#include <QtGui/QGraphicsItem>
#include "creature.h"

class BigBoss : public Creature
{
public:
    BigBoss(QGraphicsItem * myParent);
    //virtual ~BigBoss();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    //void setMaxSpeedMult(qreal maxspeedmult);

protected:
    void advance(int step);
private:
    //qreal myAngle;
    //qreal mySpeed;
    //qreal myMaxSpeed;
    qreal myEyeDirection;
    QColor myColour;
    //bool myIsCaught; // is the enemy caught by the user's line?
    //bool myAmIDead;
};

#endif // BIGBOSS_H
