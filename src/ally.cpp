#include "ally.h"
#include "creature.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>
#include <math.h>
#include <qmath.h>
//#include <QDebug>
#include <QtGui/QPolygonF>

//! [0]
Ally::Ally(QGraphicsItem * myParent = 0)
    : Creature(myParent),  myEyeDirection(0),
      myColour(qrand() % 256, qrand() % 256, qrand() % 256)
{
    myCreatureType = 2; //myCreatureType is member variable of Creature (1 for enemy, 2 for ally)
}
//! [0]


//! [1]
QRectF Ally::boundingRect() const
{
    qreal adjust = 0.5;
    // rectangle must contain all items within (redrawing depends on it)
    // 	QRectF ( qreal x, qreal y, qreal width, qreal height )
    return QRectF(-20 - adjust, -15 - adjust,
                  40 + adjust, 28 + adjust);
}
//! [1]

//! [2]
QPainterPath Ally::shape() const
{
    QPainterPath path;
    // this path is used for collision detection etc.
    path.addEllipse(-20, -10, 40, 20);
    path.addEllipse(-12,-15,24,15);
    return path;
}
//! [2]

//! [3]
void Ally::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    // Feet
    painter->setBrush(Qt::gray);
    painter->drawLine(QPointF(-16,6),QPointF(-20,10));
    painter->drawLine(QPointF(0,9),QPointF(0,13));
    painter->drawLine(QPointF(16,6),QPointF(20,10));

    // Body
    static QRadialGradient gradient(0, -3, 15, 0, -3);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::gray);
    painter->setBrush(QBrush(gradient));
    painter->drawEllipse(-20, -10, 40, 20);
    // "Dome"
    painter->setBrush(myColour);
    painter->drawEllipse(-12,-15,24,15);

    // Alien face (turns red when caught)
    painter->setBrush(myIsCaught ? Qt::red : Qt::cyan);
    painter->drawEllipse(-4, -10, 8, 8);

    // Alien eye
    painter->setBrush(Qt::black);
    qreal dx = ::sin(Creature::myAngle) * 10;
    myEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;
    painter->drawEllipse(QRectF(-2.0 + myEyeDirection, -8, 4, 4));
}

