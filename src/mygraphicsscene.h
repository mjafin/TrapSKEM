#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H
#include <QtGui/QGraphicsScene>
#include <QTimer>
#include <bb/device/VibrationController>
//#include <QFeedbackHapticsEffect>
#include <bb/multimedia/MediaPlayer>
//#include <QMediaPlayer>
#include <QEvent>

class QGraphicsView;
class QDeclarativeEngine;
class QDeclarativeComponent;
//class Enemy;
class BGTouch;
class QGraphicsObject;
//class MediaPlayer;

class myGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit myGraphicsScene(const int scrWidth, const int scrHeight,
                             const int *EnCount, const int *AlCount,
                             const int *BBCount, const int *TrCount,
                             const qreal *spMultPerLevel, const int nLevels,
                             const int *tPLevel, QStringList bGImageList);
    virtual ~myGraphicsScene();
signals:
    void callingQuits();
    void beginQmlAnimation();
    void gameFinished(int numTrapsUsed, int numTimeUsed);

public slots:
    void signalHandler(int stateChangeID, bool continuePreviousGame = false);
	void myRumbleGenerator(int notUsed);
    void startMyMusic(bb::multimedia::MediaState::Type state);
    void pausePlayMyMusic();
    void stopMyGame();

//protected:
    // event handlers
    bool event(QEvent *event);
private:
    QGraphicsView *myView; // graphics view

    QDeclarativeEngine *myQmlengine; // qml engine for qt quick
    QDeclarativeComponent *myQmlcomponent; // qml component for qt quick
    QGraphicsObject *myQmlobject; // qt quick object that can be put into a graphics scene (the main menu if you wish)
    BGTouch *myBGItem; // a pointer pointing to a level of the game
    QTimer myAdvanceTimer; // timer for advance()
    bb::multimedia::MediaPlayer *myMusicPlayer;
    int myCurrentLevel; // variable that tells the current level (starting from 0)
    int myTotalTimeUsed;
    int myTotalTrapsUsed;
    bb::device::VibrationController myHapticsRumble;
    const int myScreenWidth;
    const int myScreenHeight;
    const int *myEnemyCount;
    const int *myAllyCount;
    const int *myBigBossCount;
    const int *myTrapCount;
    const qreal *mySpeedMultiplierPerLevel;
    const int *myTimePerLevel;
    const int myNumLevels;
    QStringList myBGImageList;
    bool myGameActive; // are we in game?
    bool myIsMinimised; // is the window minimised?
    bool myQmlobjectCreated; // is the qml object created?
    bool myGlobalMusicPause; // is a global pause (from qml) enabled?
};

#endif // MYGRAPHICSSCENE_H
