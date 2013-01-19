#include "mygraphicsscene.h"
#include "enemy.h"
#include "ally.h"
#include "bigboss.h"
#include "bgtouch.h"
#include <math.h>
//#include <QDebug>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtDeclarative/QDeclarativeComponent>
#include <QtCore>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLFormat>
#include <QtGui/QGraphicsObject>
#include <QtGui/QGraphicsView>
#include <QtGui/QPixmap>
#include <bb/multimedia/MediaPlayer>
#include <QEvent>

//using namespace QtMobility;

myGraphicsScene::myGraphicsScene(const int scrWidth, const int scrHeight,
                                 const int *EnCount, const int *AlCount,
                                 const int *BBCount, const int *TrCount,
                                 const qreal *spMultPerLevel, const int nLevels,
                                 const int *tPLevel, QStringList bGImageList) :
    myScreenWidth(scrWidth),
    myScreenHeight(scrHeight), myEnemyCount(EnCount), myAllyCount(AlCount), myBigBossCount(BBCount),
    myTrapCount(TrCount), mySpeedMultiplierPerLevel(spMultPerLevel), myNumLevels(nLevels),
    myCurrentLevel(0), myTimePerLevel(tPLevel), myBGImageList(bGImageList),
    myTotalTimeUsed(0), myTotalTrapsUsed(0), myMusicPlayer(0), myGameActive(false),
    myIsMinimised(false), myQmlobjectCreated(false), myGlobalMusicPause(false)
{
    //QTimer myAdvanceTimer;
    QObject::connect(&myAdvanceTimer, SIGNAL(timeout()), this, SLOT(advance()));

    // set scene settings
    setSceneRect(0,0, myScreenWidth, myScreenHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);

    myView = new QGraphicsView;

    myView->setScene(this);
    myView->setFrameShape(QFrame::NoFrame); // disable frame (->true full screen)

    // OpenGL
    myView->QGraphicsView::setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers)));
    //QGLFormat format = QGLFormat::defaultFormat();
    //format.setSampleBuffers(false);
    //QGLWidget *glWidget = new QGLWidget(format);
    //myView->QGraphicsView::setViewport(glWidget);
        myView->setViewportUpdateMode(
                QGraphicsView::FullViewportUpdate);

    // disable scrolling
    myView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    myView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // lock landscape (done in bar-descriptor)

    //antialiasing
    myView->setRenderHint(QPainter::Antialiasing);

//! [4] //! [5]
    myView->setCacheMode(QGraphicsView::CacheBackground);
//! [5] //! [6]
    myView->showFullScreen();

    // Initialise start menu from qml
    myQmlengine = new QDeclarativeEngine(this);
    //myQmlcomponent = new QDeclarativeComponent(myQmlengine, QUrl("asset:///startMenu.qml")); // for now (ugly as ....)
    myQmlcomponent = new QDeclarativeComponent(myQmlengine, QUrl("app/native/assets/startMenu.qml")); // for now (ugly as ....)
    qDebug() << myQmlcomponent->errors();
    myQmlobject =
        qobject_cast<QGraphicsObject *>(myQmlcomponent->create());

    // change default size base on device specs
    myQmlobject->setProperty("width",myScreenWidth);
    myQmlobject->setProperty("height",myScreenHeight);

    addItem(myQmlobject);
    myQmlobjectCreated = true; // this is used in the event handler below
    myQmlobject->setProperty("iAmActive", true);

    //Connect QML signal to handler
    QObject::connect(myQmlobject, SIGNAL(sendQMLMessage(int, bool)), this, SLOT(signalHandler(int, bool)));
    // haptics
    QObject::connect(myQmlobject, SIGNAL(sendRumbleMessage(int)), this, SLOT(myRumbleGenerator(int)));
    //Connect QML exit signal to callingQuits signal (that is connected to QApplication::quit() slot in main.cpp)
    //QObject::connect(myQmlengine, SIGNAL(quit()), this, SIGNAL(callingQuits()));

    // initialise animations for startMenu.qml
    QObject::connect(this, SIGNAL(beginQmlAnimation()),myQmlobject, SIGNAL(beginAnimation()) );
    emit beginQmlAnimation();

    // connect "game finished" signals
    QObject::connect(this, SIGNAL(gameFinished(int, int)), myQmlobject, SIGNAL(gameFinishedQML(int, int)));

    // play music
    myMusicPlayer = new bb::multimedia::MediaPlayer(this);
    QUrl myUrl = QUrl("app/native/assets/music/white_winter_moon_comp.mp3");
    myMusicPlayer->setSourceUrl(myUrl);
    myMusicPlayer->play();
    QObject::connect(myMusicPlayer, SIGNAL(mediaStateChanged(bb::multimedia::MediaState::Type)), this, SLOT(startMyMusic(bb::multimedia::MediaState::Type))); // start from beginning when track ends
    QObject::connect(myQmlobject, SIGNAL(musicPlayPauseQML()), this, SLOT(pausePlayMyMusic()));
    QObject::connect(myQmlengine, SIGNAL(quit()), this, SLOT(stopMyGame()));
}

// Destructor
myGraphicsScene::~myGraphicsScene(){
    this->clear(); // Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged
    delete myQmlengine;
    delete myQmlobject;
    delete myQmlcomponent;
    delete myView;
}

////
//
// Member functions
//

void myGraphicsScene::signalHandler(int stateChangeID, bool continuePreviousGame){

    switch(stateChangeID){
    case 1: // this is a start new game signal from qml
        //remove qml UI from scene
        removeItem(myQmlobject);
        // If a game was lost ("game over") it's possible to continue from the previously played level
        if(continuePreviousGame){
            myCurrentLevel--; // subtract one, so the when it's ++:d below the same level is displayed that was previously played (and failed)
        }else{ // This means restart from beginning
            myCurrentLevel = -1; // this will be incremented to 0 below
            myTotalTimeUsed = 0; // reset time used
            myTotalTrapsUsed = 0; // reset trap count
        }
        //break; //no break, fall through to next case ->
    case 2: //  this is a "level completed" signal from game. Leave this as "case 2", as "case 1" depends on it being the next.
        myCurrentLevel++;
        // if we are already in the game, delete previous level, any enemies and stop timer
        if (myCurrentLevel > 0 && !continuePreviousGame){
            myGameActive = false; // this is used by the event() function
            myAdvanceTimer.stop(); // stop advance timer
            myBGItem->disconnect(); // disconnect all signals
            myBGItem->setVisible(false); // hide myBGItem (necessary?)
            // increase time used in-game
            myTotalTimeUsed = myTotalTimeUsed + myBGItem->myReturnTimeUsed();
            // increase number of traps used in-game
            myTotalTrapsUsed = myTotalTrapsUsed + myBGItem->myReturnNumTrapsUsed();
            this->clear(); // Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged

        }
        if(myCurrentLevel >= myNumLevels){ // No more levels
        //if(myCurrentLevel >= 1){ // No more levels
            myGameActive = true; // this is used by the event() function
            addItem(myQmlobject);
            emit gameFinished(myTotalTrapsUsed, myTotalTimeUsed); // this displays a screen with end credits (void gameFinished(int numTrapsUsed, int numTimeUsed);)
            return;
        }

        //add game bg to scene, initialise with number of traps available for this level
        myBGItem = new BGTouch(QPixmap(myBGImageList.at(myCurrentLevel)).scaled(myScreenWidth, myScreenHeight),
                               myCurrentLevel, myTrapCount[myCurrentLevel], myEnemyCount[myCurrentLevel],
                               myTimePerLevel[myCurrentLevel], myBigBossCount[myCurrentLevel]);
        //Connect touch point information to slot
        QObject::connect(myBGItem, SIGNAL(bgTouchSignal(int)), this, SLOT(signalHandler(int)));
        //add to scene
        addItem(myBGItem);
        //add Big Bosses myBigBossCount
        for (int i = 0; i < myBigBossCount[myCurrentLevel]; ++i) {
            BigBoss *bigboss = new BigBoss(myBGItem); // give myBGItem as parent (and thus when deleting the parent, the "big boss" is automatically deleted)
            //bigboss->setPos(::sin((i * 6.28) / myBigBossCount[myCurrentLevel]) * 200 + myScreenWidth/2,
            //             ::cos((i * 6.28) / myBigBossCount[myCurrentLevel]) * 200 + myScreenWidth/2);
            bigboss->setPos(myScreenWidth/2, myScreenWidth/2);
            bigboss->setMaxSpeedMult(mySpeedMultiplierPerLevel[myCurrentLevel]);
            //addItem(bigboss); // no need to add to scene as it's added to scene by setting myBGItem as parent
            connect(bigboss, SIGNAL(creatureKilled(int)), myBGItem, SLOT(creatureSubtractor(int)));
            connect(bigboss, SIGNAL(creatureKilled(int)), this, SLOT(myRumbleGenerator(int)));
            // note that allEnemiesKilledEnabler is actually a slot in creature.h but is only used by class bigboss
            connect(myBGItem, SIGNAL(allEnemiesKilled()), bigboss, SLOT(allEnemiesKilledEnabler())); // this is a way for the big bosses to know when all ordinary enemies are dealt with
        }

        //add enemies
        for (int i = 0; i < myEnemyCount[myCurrentLevel]; ++i) {
            Enemy *enemy = new Enemy(myBGItem); // give myBGItem as parent (and thus when deleting the parent, the "enemies" are automatically deleted)
            enemy->setPos(::sin((i * 6.28) / myEnemyCount[myCurrentLevel]) * 200 + myScreenWidth/2,
                         ::cos((i * 6.28) / myEnemyCount[myCurrentLevel]) * 200 + myScreenWidth/2);
            enemy->setMaxSpeedMult(mySpeedMultiplierPerLevel[myCurrentLevel]);
            //addItem(enemy); // no need to add to scene as it's added to scene by setting myBGItem as parent
            connect(enemy, SIGNAL(creatureKilled(int)), myBGItem, SLOT(creatureSubtractor(int)));
            connect(enemy, SIGNAL(creatureKilled(int)), this, SLOT(myRumbleGenerator(int)));
        }

        //add allies
        for (int i = 0; i < myAllyCount[myCurrentLevel]; ++i) {
            Ally *ally = new Ally(myBGItem); // give myBGItem as parent (and thus when deleting the parent, the "allies" are automatically deleted)
            ally->setPos(::sin((i * 6.28) / myAllyCount[myCurrentLevel] + 0.1) * 200 + myScreenWidth/2,
                         ::cos((i * 6.28) / myAllyCount[myCurrentLevel] + 0.1) * 200 + myScreenWidth/2);
            ally->setMaxSpeedMult(mySpeedMultiplierPerLevel[myCurrentLevel]);
            //addItem(ally); // no need to add to scene as it's added to scene by setting myBGItem as parent
            connect(ally, SIGNAL(creatureKilled(int)), myBGItem, SLOT(creatureSubtractor(int)));
            connect(ally, SIGNAL(creatureKilled(int)), this, SLOT(myRumbleGenerator(int)));
        }

        if (myIsMinimised){ // if minimised, don't start game
            myBGItem->myLevelTimerStop(); // level timer is started automatically when creating a new myBGItem, so stop it here as window is minimised
        } else
            myAdvanceTimer.start(1000/25); // / 33);
        myGameActive = true; // this is used by the event() function
        break;
    case 3: // this is a "game over" signal from game
        // stop timer, remove BG, remove items, return to main menu
        myGameActive = false; // this is used by the event() function
        myAdvanceTimer.stop();
        // increase time used in-game
        myTotalTimeUsed = myTotalTimeUsed + myBGItem->myReturnTimeUsed();
        // increase number of traps used in-game
        myTotalTrapsUsed = myTotalTrapsUsed + myBGItem->myReturnNumTrapsUsed();
        myBGItem->disconnect();
        myBGItem->setVisible(false);
        this->clear(); // Removes and deletes all items from the scene, but otherwise leaves the state of the scene unchanged
        addItem(myQmlobject);
        myQmlobject->setProperty("levelContinuable", true);
        break;
    default:
        //qDebug() << "Should not be here!";
        break;
    }
}

// slots
void myGraphicsScene::myRumbleGenerator(int /*unused*/){
    //qDebug() << "haptics state " << myHapticsRumble.state();
    //if (myHapticsRumble.state()==QFeedbackEffect::Stopped)
    //    myHapticsRumble.start();
	myHapticsRumble.start(100, 60); // intensity 100, duration 120ms
}

// this slot is used to repeat the same track
void myGraphicsScene::startMyMusic(bb::multimedia::MediaState::Type state){
    if (state == bb::multimedia::MediaState::Stopped)
        myMusicPlayer->play(); // re-play
}

// this slot serves for qml global play/pause purposes
void myGraphicsScene::pausePlayMyMusic(){
    myGlobalMusicPause = !myGlobalMusicPause; // flip bit
    if (myGlobalMusicPause){
        myMusicPlayer->pause();
    }else{
        myMusicPlayer->play();
    }

}
void myGraphicsScene::stopMyGame(){
	myMusicPlayer->pause();
	emit callingQuits();
}

// this event handler looks for minimisation / maximisation of the screen (e.g. screen saver, switching tasks)
bool myGraphicsScene::event(QEvent *event){
    if (event->type() == QEvent::WindowActivate) {
        myIsMinimised = false;
        if (myMusicPlayer!=NULL && !myGlobalMusicPause) // play music when maximising app (provided global pause is not on)
                myMusicPlayer->play();
        if(myGameActive){
            myAdvanceTimer.start(1000/25); // enable moving of creatures
            myBGItem->myLevelTimerStart(1000); // level timer (1 second timer)
        }
        if(myQmlobjectCreated)
            myQmlobject->setProperty("iAmActive", true);
    } else if (event->type() == QEvent::WindowDeactivate) {
        myIsMinimised = true;
        if (myMusicPlayer!=NULL)
            myMusicPlayer->pause(); // pause music when minimising app
        if(myGameActive){
            myAdvanceTimer.stop(); // disable moving of creatures
            myBGItem->myLevelTimerStop();
        }
        if(myQmlobjectCreated)
            myQmlobject->setProperty("iAmActive", false);
    }
    return QGraphicsScene::event(event); // send event further
}

