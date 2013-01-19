#include "creature.h"

#include <QtGui/QGraphicsScene>
#include <QtGui/QStyleOption>
//#include <QAccelerometer>
#include <QtOpenGL/QGLWidget>
#include <math.h>
#include <qmath.h>
//#include <QDebug>

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
Creature::Creature(QGraphicsItem * myParent = 0)
    //: QGraphicsItem(myParent), myAngle(0), mySpeed(0), myMaxSpeed(5),
    : QGraphicsObject(myParent), myAngle(0), mySpeed(0), myMaxSpeed(5),
      myAmIDead(false), myAccelerometer(0),
      myAllEnemiesKilled(false)
{
    setRotation(qrand() % (360 * 16));
    myIsCaught = false; // by default the Creature is not caught by the user
    myAccelerometer = new QAccelerometer;
}
//! [0]
Creature::~Creature()
{
    myAccelerometer->stop();
    delete myAccelerometer;
}

//virtual functions
QRectF Creature::boundingRect() const
{
}
QPainterPath Creature::shape() const
{
}
void Creature::paint(QPainter * /*painter*/, const QStyleOptionGraphicsItem * /*option*/,
           QWidget * /*widget*/)
{
}


//! [4]
void Creature::advance(int step)
{
    // store previous state of myIsCaught;
    bool tempWasCaught = myIsCaught;
    // check if the item is colliding with a line (stopping criterion)
    myIsCaught = false;
    foreach (QGraphicsItem *item, scene()->collidingItems(this)){
        if (item->type() == 6 && item->isVisible()) // graphics line item is of type 6
            myIsCaught = true;
    }
    if (!tempWasCaught && myIsCaught) // if the Creature wasn't caught previously but now is..
    {
        myAccelerometer->start();
    }
    else if (tempWasCaught && !myIsCaught) // if the Creature was caught but isn't anymore..
    {
        myAccelerometer->stop();
    }
    if (myIsCaught && myAccelerometer->isActive())
    {
        QAccelerometerReading *accReading = myAccelerometer->reading(); // const pointer
        qreal accel = qSqrt(qPow(accReading->x(),2)+qPow(accReading->y(),2)+qPow(accReading->z(),2));
        if (qAbs(9.81-accel)>3.5 && accel > 0.01){
            //qDebug()<<"Acceleration currently "<<accel;
            // remove me from scene
            //scene()->removeItem(this);
            // hide item
            setVisible(false);
            // emit a signal "I'm dead" (as a failsafe, use myAmIDead to make sure this happens once only!)
            if (!myAmIDead){
                myAmIDead = true;
                emit creatureKilled(myCreatureType); //myCreatureType=1 for enemy, myCreatureType=2 for ally
            }
        }

    }

    if (!step || myIsCaught)
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
            myAngle += (myAngle < -Pi / 2) ? 0.25 : -0.25;
        } else if (angleToCenter >= Pi && angleToCenter < (Pi + Pi / 2 + Pi / 4)) {
            // Rotate right
            myAngle += (myAngle < Pi / 2) ? 0.25 : -0.25;
        }
    } else if (::sin(myAngle) < 0) {
        myAngle += 0.25;
    } else if (::sin(myAngle) > 0) {
        myAngle -= 0.25;
//! [5] //! [6]
    }
//! [6]


    // Add some random movement
    if ((qrand() % 10) == 0) {
        if (qrand() % 1)
            myAngle += (qrand() % 100) / 500.0;
        else
            myAngle -= (qrand() % 100) / 500.0;
    }

//! [10]

//! [11]
    //mySpeed += (-50 + qrand() % 100) / 100.0;
    mySpeed += (-50 + qrand() % 100) / 50.0;

    qreal dx = ::sin(myAngle) * 10;
    //myEyeDirection = (qAbs(dx / 5) < 1) ? 0 : dx / 5;

    setRotation(rotation() + dx);
    //setPos(mapToParent(0, -(3 + sin(mySpeed) * 3)));
    setPos(mapToParent(0, -(myMaxSpeed + sin(mySpeed) * myMaxSpeed)));
}
//! [11]




void Creature::setMaxSpeedMult(qreal maxspeedmult){
    myMaxSpeed = 5*maxspeedmult;
}



// slot: this slot should be called when all ordinary enemies have been killed
void Creature::allEnemiesKilledEnabler(){
    myAllEnemiesKilled = true;
}
