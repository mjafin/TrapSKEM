// Base class for different creatures (allies and enemies)

#ifndef CREATURE_H
#define CREATURE_H
//#include <QtGui>
#include <QtGui/QGraphicsItem>

#include <QtSensors/QAccelerometer>

using namespace QtMobility;
//QTM_USE_NAMESPACE

//class Creature : public QObject, public QGraphicsItem // <- this gives a warning
class Creature: public QGraphicsObject // this should work
{
    Q_OBJECT
public:
    Creature(QGraphicsItem * myParent);
    virtual ~Creature();

    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    void setMaxSpeedMult(qreal maxspeedmult);
public slots:
    void allEnemiesKilledEnabler(); // this is used by the bigboss class mainly
signals:
    void creatureKilled(int);
protected:
    virtual void advance(int step);
    int myCreatureType; // what type of creature?
    bool myIsCaught; // is the Creature caught by the user's line?
    qreal myAngle;
    qreal mySpeed;
    qreal myMaxSpeed;
    bool myAmIDead;
    QAccelerometer *myAccelerometer;
    bool myAllEnemiesKilled;

private:
    //qreal mySpeed;
    //qreal myMaxSpeed;
    //qreal myEyeDirection;
    //QColor myColour;
    //bool myAmIDead;
    //QAccelerometer *myAccelerometer;
};

#endif // CREATURE_H
