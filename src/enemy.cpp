#include "enemy.h"
#include "creature.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
#include <QtGui/QStyleOption>
#include <math.h>
#include <qmath.h>
//#include <QDebug>
#include <QtGui/QPolygonF>

//! [0]
Enemy::Enemy(QGraphicsItem * myParent = 0)
    : Creature(myParent),  myEyeDirection(0),
      myColour(qrand() % 256, qrand() % 256, qrand() % 256)
{
    myCreatureType = 1; //myCreatureType is member variable of Creature
}
//! [0]


//! [1]
QRectF Enemy::boundingRect() const
{
    qreal adjust = 0.5;
    // rectangle must contain all items within (redrawing depends on it)
    // 	QRectF ( qreal x, qreal y, qreal width, qreal height )
    return QRectF(-25 - adjust, -30 - adjust,
                  50 + adjust, 42 + adjust);
}
//! [1]

//! [2]
QPainterPath Enemy::shape() const
{
    QPainterPath path;
    // this path is used for collision detection etc.
    QPolygonF myPolygon;
    myPolygon << QPointF(0, 12) << QPointF(-25, -14) << QPointF(0, -30)<< QPointF(25, -14);
    path.addPolygon(myPolygon);
    //older version
    /*
    path.addRect(-10, -20, 20, 40); // add body
    path.addEllipse(-25, -8, 24, 16); // add first wing
    path.addEllipse(1, -8, 24, 16); // add second wing
    */
    return path;
}
//! [2]

//! [3]
void Enemy::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    //if (painter->paintEngine()->type() == QPaintEngine::OpenGL2 || painter->paintEngine()->type() == QPaintEngine::OpenGL){
    //    //qDebug()<<"OpenGL reporting!";
    //}
    // QPainter: (uses OpenGL for rectangles, ellipses etc. if above if-clause is true..?)
    // Body
    //painter->setBrush(myColour);
    static QLinearGradient hullGradient(0,-25,0,5);
    hullGradient.setColorAt(0, Qt::white);
    hullGradient.setColorAt(1, myColour);
    painter->setBrush(QBrush(hullGradient));
    // painter->drawEllipse(-10, -20, 20, 40);
    static const QPointF hullPoints[3] = {
        QPointF(-5.0, 10),
        QPointF(0, -30),
        QPointF(5, 10)
    };
    painter->drawPolygon(hullPoints, 3);

    // "Scanner" eye
    painter->setBrush(Qt::red);
    painter->drawEllipse(-4, -20, 8, 8);

    // Guns
    painter->setBrush(Qt::gray);
    painter->drawRect(-16,-12,4,8);
    painter->drawRect(12,-12,4,8);
    //  Pupil of "Scanner"
    qreal dx = ::sin(Creature::myAngle) * 10;
    myEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;
    painter->drawEllipse(QRectF(-2.0 + 2*myEyeDirection, -22, 4, 4));

    // Wings
    //painter->setBrush(myIsCaught ? Qt::red : Qt::darkYellow);
    static QConicalGradient wingGradient(0,12,180);
    wingGradient.setColorAt(0, Qt::white);
    wingGradient.setColorAt(1, Creature::myIsCaught ? Qt::red : Qt::darkYellow); //Qt::gray);
    painter->setBrush(QBrush(wingGradient));
    //1st way of implementing the wings
    static const QPointF wingPoints[4] = {
        QPointF(0, 12),
        QPointF(-25, -14),
        QPointF(0, 4),
        QPointF(25, -14)
    };
    painter->drawPolygon(wingPoints, 4);
}
