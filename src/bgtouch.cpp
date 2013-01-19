#include <QtGui/QBrush>
#include <QtGui/QPen>
#include <QtGui/QTouchEvent>
//#include <QtDebug>
#include <QList>
#include <QtGui/QFont>
#include <QtGui/QGraphicsScene>
#include <QTimer>
#include "bgtouch.h"

BGTouch::BGTouch(QPixmap BGImage, int levelNum, int numTraps, int enemyInitCount, int timeForLevel , int bigBossCount)
    : QGraphicsPixmapItem(BGImage), myLevelNum(levelNum), myEnemyCount(enemyInitCount), myNumTrapsAtBeginning(numTraps),
      myNumTrapsRemaining(numTraps), myBigBossCount(bigBossCount),
      myMaxTouchPoints(2), myTouchPointCount(0), myTwoPointsUsed(false), myLevelCompleted(false), myTrapIsDeployed(false),
      myLevelFailed(false), myTimeRemaining(timeForLevel), myTimeAtBeginning(timeForLevel), myIsPaused(false),
      myFallThroughTouchEvent(false), myNumTrapsUsedHelper(0)
{
    // accept touch events
    setAcceptTouchEvents(true);
    // initialise touch points
    QRadialGradient gradient(0, 0, 50, 5, 5);
    gradient.setColorAt(0, QColor::fromRgbF(0, 1, 0, 1));
    gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
    myNode1 = new QGraphicsEllipseItem(-18,-18,36,36,this);
    myNode1->setBrush(QBrush(gradient));
    myNode1->setPos(0, 0);
    myNode1->hide();
    myNode2 = new QGraphicsEllipseItem(-18,-18,36,36,this);
    myNode2->setBrush(QBrush(gradient));
    myNode2->setPos(0, 0);
    myNode2->hide();

    //initialise line item
    myConnector = new QGraphicsLineItem(0, 0, 0, 0, this);
    myConnector->setPen(QPen(Qt::red, 3, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin));
    myConnector->hide();

    //initialise on-screen trap indicators
    updateTrapsIndicator(true);

    connect(&myLevelTimer, SIGNAL(timeout()), this, SLOT(timeKeeper()));
    //initialise pause item
    mySimplePauseText = showTextOnScreen("Paused! Hit top left corner.",QBrush(Qt::yellow),QPen(Qt::gray));
    if(myLevelNum==0){
        myIsPaused = false;
        mySimplePauseText->hide(); // no need to delete as this is child of "this"
        // set timer to reduce time remaining
        myLevelTimerStart(1000);
    }else{
        myIsPaused = true;
        mySimplePauseText->show();
    }

}

//Destructor? Not necessary so far as all objects are made children of "this"
//BGTouch::~BGTouch()
//{
//}
int BGTouch::myReturnNumTrapsUsed(){
    return myNumTrapsUsedHelper;
}

int BGTouch::myReturnTimeUsed(){
    return myTimeAtBeginning - myTimeRemaining;
}

int BGTouch::myReturnEnemiesLeft(){
    return myEnemyCount;
}

bool BGTouch::sceneEvent(QEvent *event)
{
    //don't process touch events if myNumTrapsRemaining==0 (i.e. Game Over)
    if (myNumTrapsRemaining==0){
        switch (event->type()) {
            case QEvent::TouchBegin:
            case QEvent::TouchUpdate:
            case QEvent::TouchEnd:
            {
                //QTimer::singleShot(1500, this, SLOT(gameOver())); // game over
                //emit bgTouchSignal(3); // 3 is the identifier to "Game Over"
                return true; // touch event handled
            }
            default:
                return QGraphicsItem::sceneEvent(event); // other kind of event
        }
    }


    switch (event->type()) {
    case QEvent::TouchBegin:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event); // get event        
        const QList<QTouchEvent::TouchPoint> & myTouchPoints = touchEvent->touchPoints();
        // touchEvent->touchPoints().size() seems to be always 1 here
        //fprintf(stdout, "BGTouch::sceneEvent() TouchUpdate, points %d\n", touchEvent->touchPoints().size());
        //fflush(stdout);
        myTouchPointCount = 1;
        myFallThroughTouchEvent = false; // this is a helper bool for pause game
        // Check if pause was enabled (observe top left corner)
        if(myTouchPoints[0].pos().x()<=this->boundingRect().height()/6 // division by 6 "works"
                && myTouchPoints[0].pos().y()<=this->boundingRect().height()/6){
            if(myIsPaused){
                myIsPaused = false;
                mySimplePauseText->hide();
                myLevelTimerStart(1000);
                myFallThroughTouchEvent = true; // fall through the QEvent::TouchUpdate and QEvent::TouchEnd parts of this touch event
            }else{
                mySimplePauseText->show();
                myIsPaused = true;
                myLevelTimerStop();
            }
            break;
        }
        if(myIsPaused) // don't update anything if game is paused
            break;
        //myNode1 only updated when the touch event starts
        myNode1->setPos(myTouchPoints[0].pos().x(), myTouchPoints[0].pos().y());
        myNode1->show();
        //qDebug()<<"TouchBegin, location y: " << myTouchPoints[0].pos().y() << " x: " << myTouchPoints[0].pos().x();
        break;
    }
    case QEvent::TouchUpdate:
    {
        if(myIsPaused || myFallThroughTouchEvent) // don't update anything if game is paused
            break;
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event); // get event
        //qDebug()<<"BGTouch::sceneEvent() TouchUpdate, points "<<touchEvent->touchPoints().size();
        //fprintf(stdout, "BGTouch::sceneEvent() TouchUpdate, points %d\n", touchEvent->touchPoints().size());
        //fflush(stdout);
        // if myTouchPointCount was 2 or higher previously but now there are 1 or less...
        int touchPointCountTemp = touchEvent->touchPoints().size(); // how many touch points are we talking about?
        //qDebug()<<"TouchUpdate, num points: " << touchPointCountTemp;
        if (myTouchPointCount > 1 && touchPointCountTemp < 2)
        {
            myTrapIsDeployed = false; // slow timer
            myNode2->hide();
            myConnector->hide(); // hide myConnector as there are no longer two touch points
            myTwoPointsUsed = true;
        }        
        if (touchPointCountTemp > 2)
            touchPointCountTemp = 2; // saturate to 2 (actually S^3 can't handle more)
        // get updated position in scene
        const QList<QTouchEvent::TouchPoint> & myTouchPoints = touchEvent->touchPoints();

        // update position of first point
        switch (myTouchPoints[0].state()) {
            case Qt::TouchPointStationary:
                break; // don't do anything if this touch point hasn't moved
        default:
                myNode1->setPos(myTouchPoints[0].pos().x(), myTouchPoints[0].pos().y());
        }

        // update 2nd item in scene
        if (touchPointCountTemp == 2 && !myTwoPointsUsed)
        {
            //qDebug()<<"Point 2 " << myTouchPoints[1].pos().y()<< " state is" << myTouchPoints[1].state();
            switch (myTouchPoints[1].state()) {
                case Qt::TouchPointStationary:
                    break;// don't do anything if this touch point hasn't moved
            default:
            {
                //qDebug()<<"Point 2 ,myTouchPointCount "<< touchPointCount;
                if (myTouchPointCount < 2) // if we previously didn't have the second touch point, show it now
                {
                    myNumTrapsUsedHelper++; // this helper is for book keeping only (scoring)
                    myTrapIsDeployed = true; // speed up timer!
                    myNode2->show();
                    myConnector->show();
                }
                myNode2->setPos(myTouchPoints[1].pos().x(), myTouchPoints[1].pos().y());
                //qDebug()<<"point 2 y value "<< myTouchPoints[1].pos().y();
             }
          }
            myConnector->setLine(myNode1->x(), myNode1->y(),     myNode2->x(), myNode2->y());
        }
        myTouchPointCount = touchPointCountTemp;
        break;
    }
    case QEvent::TouchEnd:
    {
        if(myIsPaused || myFallThroughTouchEvent) // don't update anything if game is paused
            break;
        //qDebug()<<"TouchEnd";

        // The following is necessary on BB10 (wasn't on Harmattan), as it's possible to go down from 2 touch points to 0
        if (myTouchPointCount > 1)
        	myTwoPointsUsed = true;

        myTrapIsDeployed = false; // slow timer (this should already be false, but just in case)
        // as the touch event has ended (no fingers touching), hide all touch points
        myNode1->hide();
        myNode2->hide();
        myConnector->hide();
        myTouchPointCount = 0; // no more touch points
        //fprintf(stdout, "TouchEnd event, myNumTrapsRemaining: %d, myLevelCompleted: %d, myTwoPointsUsed: %d\n",myNumTrapsRemaining,myLevelCompleted,myTwoPointsUsed);
        //fflush(stdout);
        if (myNumTrapsRemaining > 0 && !myLevelCompleted && myTwoPointsUsed) // Only reduce number of traps if level isn't completed!
            myNumTrapsRemaining--; // reduce number of remaining "traps"
        myTwoPointsUsed = false; // two points not visited any more
        if (myNumTrapsRemaining==0 && !myLevelCompleted && !myLevelFailed)
        {
            // game over mate!
            myLevelFailed = true;
            showTextOnScreen("Game over!",QBrush(Qt::red),QPen(Qt::white));
            myLevelTimerStop(); // stop level timer
            QTimer::singleShot(4000, this, SLOT(gameOver())); // game over
        }
        updateTrapsIndicator(false);
        break;
    }
        // if this wasn't a touch event, use ordinary event handler
    default:
        return QGraphicsItem::sceneEvent(event);
    }
    return true; // the event was a touch event and was handled here, thus return true
}



//
// PRIVATE FUNCTIONS
//

// This function plots ellipses on screen to show how many "traps" the player has left to deploy
void BGTouch::updateTrapsIndicator(bool initialise)
{
    // indicator: a circle whose radius get smaller as more "traps" are used
    //int reductionOfRad = 4;
    //int EllipseRad = reductionOfRad*myNumTrapsRemaining;
    //int EllipseRad = myNumTrapsRemaining * this->boundingRect().height() / 70;
    int EllipseRad = this->boundingRect().height() * myTimeRemaining / (5*myTimeAtBeginning);
    if(initialise){
        // as "this" is passed to the constructor of myTrapsIndicator, graphics view should take care of its deletion?
        myTrapsIndicator = new QGraphicsEllipseItem(-EllipseRad/2,-EllipseRad/2,EllipseRad,EllipseRad,this);
        myTrapsIndicator->setAcceptTouchEvents(false);
        QPointF myOffset = this->offset();
        // introduce a gradient
        QRadialGradient gradient(0, 0, EllipseRad/1.5, 5, 5);
        gradient.setColorAt(0, QColor::fromRgbF(.2, .7, .2, 1));
        gradient.setColorAt(1, QColor::fromRgbF(0, 0, 0, 0));
        myTrapsIndicator->setBrush(QBrush(gradient));
        myTrapsIndicator->setPos(myOffset.x()+5+EllipseRad/2, myOffset.y()+5+EllipseRad/2);
        myTrapsIndicator->setOpacity(0.8);
        myTrapsIndicator->show();

        QGraphicsSimpleTextItem * levelNumText = new QGraphicsSimpleTextItem( QString("%1").arg(myLevelNum+1), this);
        levelNumText->setAcceptTouchEvents(false);
        int fontPixelSize = this->boundingRect().height()/10; // take approximate height of image divided by twelwe
        QFont messageFont = QFont("System", -1, QFont::Bold);
        messageFont.setPixelSize(fontPixelSize);
        levelNumText->setFont(messageFont);
        levelNumText->setBrush(QBrush(QColor(0,127,255,127)));
        levelNumText->setPen(QPen(Qt::blue));
        levelNumText->setPos(myOffset.x()+5+EllipseRad/2 - levelNumText->boundingRect().width()/2 ,
                             myOffset.y()+5+EllipseRad/2 - levelNumText->boundingRect().height()/2);

    }else{
        myTrapsIndicator->setRect(-EllipseRad/2,-EllipseRad/2,EllipseRad,EllipseRad);
        //myTrapsIndicator->setSpanAngle(5760*myTimeRemaining/myTimeAtBeginning); //360deg * 16 = 5760
        myTrapsIndicator->setSpanAngle(5760*myNumTrapsRemaining/myNumTrapsAtBeginning); //360deg * 16 = 5760
        //fprintf(stdout, "myNumTrapsRemaining: %d, myNumTrapsAtBeginning: %d\n",myNumTrapsRemaining,myNumTrapsAtBeginning);
        //fflush(stdout);
    }
    return;
}


void BGTouch::creatureSubtractor(int creatureType){
    switch (creatureType){
    case 1: // enemy killed!
    {
        myEnemyCount--;
        if (myEnemyCount==0)
            emit allEnemiesKilled();
        if (myEnemyCount <= 0 && !myLevelCompleted && !myLevelFailed && myBigBossCount <= 0){
            showTextOnScreen("Level completed!", QBrush(Qt::cyan), QPen(Qt::blue));
            QTimer::singleShot(4000, this, SLOT(levelEnded()));
            myLevelCompleted = true;
            myLevelTimerStop(); // stop level timer
        }
        break;
    }
    case 2: // ally killed! Game over!!!
    {
        if (!myLevelCompleted && !myLevelFailed){
            myLevelFailed = true;
            myLevelTimerStop(); // stop level timer
            myNumTrapsRemaining = 0;
            QTimer::singleShot(4000, this, SLOT(gameOver())); // game over
            showTextOnScreen("Ally killed - game over!",QBrush(Qt::red),QPen(Qt::magenta));
        }
        break;
    }
    case 3: // big boss killed (should not be possible unless myEnemyCount = 0)
    {
        myBigBossCount--;
        if(!myLevelCompleted && myBigBossCount <= 0){
            showTextOnScreen("Level finished!", QBrush(Qt::cyan), QPen(Qt::blue));
            QTimer::singleShot(6000, this, SLOT(levelEnded()));
            myLevelCompleted = true;
            myLevelTimerStop(); // stop level timer
        }
        break;
    }
    default:{
    	break;
    }
        //qDebug()<<"Should not be here!!!";
    }
}

void BGTouch::levelEnded(){
    emit bgTouchSignal(2); // 2 is the identifier to "Next level"
}

void BGTouch::gameOver(){
    emit bgTouchSignal(3); // 3 is the identifier to "Game Over"
}

// keep track of time
void BGTouch::timeKeeper(){
    //qDebug()<<"timeKeeper reporting";
    myTimeRemaining--;
    if(myTrapIsDeployed)
        myTimeRemaining--; // timer goes faster if a trap is deployed
    updateTrapsIndicator(false); // update the pie circle (false means no initialisation)
    if (myTimeRemaining <= 0){
        myLevelFailed = true;
        myLevelTimerStop();
        myNumTrapsRemaining = 0;
        QTimer::singleShot(4000, this, SLOT(gameOver())); // game over
        showTextOnScreen("Time out - Game over!",QBrush(Qt::red),QPen(Qt::white));
    }
}

// this function shows text on screen, e.g. "Game over" or "Level completed"
QGraphicsSimpleTextItem* BGTouch::showTextOnScreen(QString textfield, QBrush brush, QPen pen){
    QGraphicsSimpleTextItem *messageOnScreen = new QGraphicsSimpleTextItem(textfield, this);
    int fontPixelSize = this->boundingRect().height()/10; // take approximate height of image divided by twelwe
    QFont messageFont = QFont("System", -1, QFont::Bold);
    messageFont.setPixelSize(fontPixelSize);
    messageOnScreen->setFont(messageFont);
    messageOnScreen->setBrush(brush);
    messageOnScreen->setPen(pen);
    messageOnScreen->setPos(this->boundingRect().center().x()-messageOnScreen->boundingRect().width()/2,
                    this->boundingRect().center().y()-messageOnScreen->boundingRect().height()/2);
    return messageOnScreen;
}

void BGTouch::myLevelTimerStart(int interval){
    myLevelTimer.start(interval);

}

void BGTouch::myLevelTimerStop(){
    myLevelTimer.stop();
}
