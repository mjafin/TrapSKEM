// Default empty project template
#include <QtGui/QApplication>
#include <QtDeclarative>
#include <QDebug>
#include <Qt/qdeclarativedebug.h>
#include "mygraphicsscene.h"
Q_DECL_EXPORT int main(int argc, char **argv)
{
	const int numLevels = 10; // number of levels
	const int EnemyCount [numLevels]    = {1,2,4,1,1,4,4,5,5,5}; // enemy counts for levels
	const int AllyCount [numLevels]     = {0,1,1,0,1,2,2,3,4,6}; // ally counts for levels
	const int TrapCount [numLevels]     = {9,7,5,4,3,5,4,5,4,3}; // trap counts for levels
	const int BigBossCount [numLevels]  = {0,0,0,1,1,0,0,0,1,2}; // trap counts for levels
	const qreal speedMultiplierPerLevel [numLevels] = {1,1.3,1.2,1.5,1.5,1.5,1.6,1.7,2,2}; // speeds of creatures for the levels
	const int timePerLevel [numLevels] = {30,20,20,20,20,20,20,20,30,25}; // time given for each level in seconds
	QStringList bGImageTable;
	bGImageTable << "app/native/assets/images/Level_00.jpg" << "app/native/assets/images/Level_01.jpg" <<
			"app/native/assets/images/Level_02.jpg" << "app/native/assets/images/Level_03.jpg" <<
			"app/native/assets/images/Level_04.jpg" << "app/native/assets/images/Level_05.jpg" <<
			"app/native/assets/images/Level_06.jpg" << "app/native/assets/images/Level_07.jpg" <<
			"app/native/assets/images/Level_08.jpg" << "app/native/assets/images/Level_09.jpg";

	// this is where the server is started etc
    QApplication app(argc, argv);
    const int screenHeight = qMin(app.desktop()->screenGeometry().height(),app.desktop()->screenGeometry().width());
    const int screenWidth  = qMax(app.desktop()->screenGeometry().height(),app.desktop()->screenGeometry().width());
    //qDebug()<< "height "<<  screenHeight << " width" << screenWidth;

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));

    myGraphicsScene scene(screenWidth, screenHeight, EnemyCount, AllyCount, BigBossCount, TrapCount, speedMultiplierPerLevel, numLevels, timePerLevel, bGImageTable);

    // this signal can be used for quitting
    QObject::connect(&scene, SIGNAL(callingQuits()), &app, SLOT(quit()));


    // we complete the transaction started in the app constructor and start the client event loop here
    return app.exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
