#ifndef BGTOUCH_H
#define BGTOUCH_H
//#include <QtGui>
#include <QtGui/QGraphicsItem>
#include <QObject>
#include <QTimer>
#include <QString>
#include <QtGui/QPixmap>

class BGTouch : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    BGTouch(QPixmap BGImage, int levelNum, int numTraps = 10, int enemyInitCount = 1, int timeForLevel = 25, int bigBossCount = 0);
    //virtual ~BGTouch();
    int myReturnNumTrapsUsed();
    int myReturnTimeUsed();
    int myReturnEnemiesLeft();
    void myLevelTimerStart(int interval);
    void myLevelTimerStop();
    // event handlers
    bool sceneEvent(QEvent *event);
public slots:
    void creatureSubtractor(int creatureType); //keeps track of remaining creatures (enemies as well as allies)
    void levelEnded();
    void gameOver();
    void timeKeeper();
signals:
    void bgTouchSignal(int stateChangeID);
    void allEnemiesKilled();
private:
    QGraphicsSimpleTextItem* showTextOnScreen(QString textfield, QBrush brush, QPen pen);
    void updateTrapsIndicator(bool initialise = false); // this function plots the "how many traps remaining" dots on the screen
    QGraphicsEllipseItem *myNode1;
    QGraphicsEllipseItem *myNode2;
    QGraphicsLineItem *myConnector;
    QGraphicsEllipseItem *myTrapsIndicator;
    QGraphicsSimpleTextItem * mySimplePauseText;
    int myLevelNum;
    int myMaxTouchPoints;
    int myTouchPointCount;
    int myEnemyCount;
    int myBigBossCount;
    int myNumTrapsRemaining;
    int myNumTrapsAtBeginning;
    int myNumTrapsUsedHelper;
    int myTimeRemaining;
    int myTimeAtBeginning;
    bool myTwoPointsUsed;
    bool myLevelCompleted;
    bool myLevelFailed;
    bool myTrapIsDeployed; // if a trap is deployed, timer will go faster
    bool myIsPaused;
    bool myFallThroughTouchEvent;
    QTimer myLevelTimer;
};

#endif // BGTOUCH_H
