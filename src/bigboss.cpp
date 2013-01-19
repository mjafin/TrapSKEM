#include "bigboss.h"
#include "creature.h"
#include <QtGui/QGraphicsScene>
#include <QtGui/QPainter>
//#include <QtGui/QStyleOption>
#include <QtOpenGL/QGLWidget>
#include <math.h>
#include <qmath.h>
//#include <QDebug>
#include <QtGui/QPolygonF>

static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

static qreal normalizeAngle(qreal angle)
{
    while (angle < 0)
        angle += TwoPi;
    while (angle > TwoPi)
        angle -= TwoPi;
    return angle;
}

//! [0]
BigBoss::BigBoss(QGraphicsItem * myParent = 0)
    : Creature(myParent), myEyeDirection(0),
      myColour(qrand() % 256, qrand() % 256, qrand() % 256)
{
    Creature::myCreatureType = 3;
}
//! [0]

//virtual functions
QRectF BigBoss::boundingRect() const
{
    qreal adjust = 0.5;
    // rectangle must contain all items within (redrawing depends on it)
    // 	QRectF ( qreal x, qreal y, qreal width, qreal height )
    return QRectF(-25 - adjust, -30 - adjust,
                  50 + adjust, 42 + adjust);
}
QPainterPath BigBoss::shape() const
{
    QPainterPath path;
    // this path is used for collision detection etc.
    QPolygonF myPolygon;
    myPolygon << QPointF(0, 12) << QPointF(-25, -14) << QPointF(0, -30)<< QPointF(25, -14);
    path.addPolygon(myPolygon);
    return path;
}
void BigBoss::paint(QPainter * painter, const QStyleOptionGraphicsItem * /*option*/,
           QWidget * /*widget*/)
{
    //if (painter->paintEngine()->type() == QPaintEngine::OpenGL2 || painter->paintEngine()->type() == QPaintEngine::OpenGL){
    //    //qDebug()<<"OpenGL reporting!";
    //}
    // QPainter: (uses OpenGL for rectangles, ellipses etc. if above if-clause is true..?)
    // Body
    static QLinearGradient hullGradient(0,-45,0,5);
    hullGradient.setColorAt(0, Qt::white);
    hullGradient.setColorAt(1, myColour);
    painter->setBrush(QBrush(hullGradient));
    //painter->setBrush(myColour);
    static const QPointF hullPoints[3] = {
        QPointF(-9, 15),
        QPointF(0,-55),
        QPointF(9, 15)
    };
    painter->drawPolygon(hullPoints, 3);

    // "Scanner" eye
    static QRadialGradient eyeGradient(0, -32, 8, 0, -32);
    eyeGradient.setColorAt(0, Qt::red);
    eyeGradient.setColorAt(1, Qt::white);
    painter->setBrush(QBrush(eyeGradient));
    //painter->setBrush(Qt::red);
    painter->drawEllipse(-7, -39, 14, 14);

    //  Pupil of "Scanner"
    painter->setBrush(Qt::black);
    qreal dx = ::sin(Creature::myAngle) * 10;
    myEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;
    painter->drawEllipse(QRectF(-2.0 + 3*myEyeDirection, -36, 4, 4));

    // Guns
    painter->setBrush(Qt::gray);
    painter->drawRect(-24,-18,4,10);
    painter->drawRect(20,-18,4,10);

    // Wings
    static QConicalGradient wingGradient(0,20,180);
    wingGradient.setColorAt(0, Qt::white);
    wingGradient.setColorAt(1, Creature::myIsCaught ? Qt::red : Qt::darkYellow); //Qt::gray);
    painter->setBrush(QBrush(wingGradient));
    //painter->setBrush(Creature::myIsCaught ? Qt::red : Qt::darkYellow);
    //1st way of implementing the wings
    static const QPointF wingPoints[4] = {
        QPointF(0, 20),
        QPointF(-45, -20),
        QPointF(0, 0),
        QPointF(45, -20)
    };
    painter->drawPolygon(wingPoints, 4);
}


//! [4]
void BigBoss::advance(int step)
{
    // store previous state of myIsCaught;
    bool tempWasCaught = Creature::myIsCaught;
    // check if the item is colliding with a line (stopping criterion)
    Creature::myIsCaught = false;
    foreach (QGraphicsItem *item, scene()->collidingItems(this)){
        if (item->type() == 6 && item->isVisible()) // graphics line item is of type 6
            Creature::myIsCaught = true;
    }
    if (!tempWasCaught && Creature::myIsCaught) // if the BigBoss wasn't caught previously but now is..
    {
        Creature::myAccelerometer->start();
    }
    else if (tempWasCaught && !Creature::myIsCaught) // if the BigBoss was caught but isn't anymore..
    {
        Creature::myAccelerometer->stop();
    }
    if (Creature::myIsCaught && Creature::myAccelerometer->isActive() &&
            Creature::myAllEnemiesKilled) //
    {
        QAccelerometerReading *accReading = Creature::myAccelerometer->reading(); // const pointer
        qreal accel = qSqrt(qPow(accReading->x(),2)+qPow(accReading->y(),2)+qPow(accReading->z(),2));
        if (qAbs(9.81-accel)>3.5 && accel > 0.01){
            //qDebug()<<"Acceleration currently "<<accel;
            // remove me from scene
            //scene()->removeItem(this);
            // hide item
            setVisible(false);
            // emit a signal "I'm dead" (as a failsafe, use Creature::myAmIDead to make sure this happens once only!)
            if (!Creature::myAmIDead){
                Creature::myAmIDead = true;
                emit creatureKilled(myCreatureType); //myCreatureType=1 for enemy, myCreatureType=2 for ally, myCreatureType=3 for big boss
            }
        }

    }

    if (!step) // || Creature::myIsCaught) // big bosses not stopped by traps..
        return;
//! [4]
    // Don't move too far away
//! [5]
    //QLineF lineToCenter(QPointF(0, 0), mapFromScene(0, 0));
    QLineF lineToCenter(QPointF(0, 0), mapFromScene(this->scene()->sceneRect().center()));
    if (lineToCenter.length() > 200) {
        qreal angleToCenter = ::acos(lineToCenter.dx() / lineToCenter.length());
        if (lineToCenter.dy() < 0)
            angleToCenter = TwoPi - angleToCenter;
        angleToCenter = normalizeAngle((Pi - angleToCenter) + Pi / 2);

        if (angleToCenter < Pi && angleToCenter > Pi / 4) {
            // Rotate left
            Creature::myAngle += (Creature::myAngle < -Pi / 2) ? 0.25 : -0.25;
        } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
            // Rotate right
            Creature::myAngle += (Creature::myAngle < Pi / 2) ? 0.25 : -0.25;
        }
    } else if (::sin(Creature::myAngle) < 0) {
        Creature::myAngle += 0.25;
    } else if (::sin(Creature::myAngle) > 0) {
        Creature::myAngle -= 0.25;
//! [5] //! [6]
    }
//! [6]


    // Add some random movement
    if ((qrand() % 10) == 0) {
        if (qrand() % 1)
            Creature::myAngle += (qrand() % 100) / 500.0;
        else
            Creature::myAngle -= (qrand() % 100) / 500.0;
    }

//! [10]

//! [11]
    //Creature::mySpeed += (-50 + qrand() % 100) / 100.0;
    Creature::mySpeed += (-50 + qrand() % 100) / 50.0;

    qreal dx = ::sin(Creature::myAngle) * 10;
    //myEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

    setRotation(rotation() + dx);
    //setPos(mapToParent(0, -(3 + sin(Creature::mySpeed) * 3)));
    setPos(mapToParent(0, -(Creature::myMaxSpeed + sin(Creature::mySpeed) * Creature::myMaxSpeed)));
}
//! [11]

