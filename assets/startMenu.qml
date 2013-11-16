import QtQuick 1.1

Rectangle {
    id: startmenu
    width: 640
    height: 360
    Image {
            anchors.fill: parent
            source: "images/mainMenuBG.jpg"
        }
    signal sendQMLMessage(int messageID, bool continuePreviousGame)
    signal sendRumbleMessage(int rumbleMessage) // rumbleMessage is ignored actually
    signal gameFinishedQML(int numTrUs, int numTiUs)
    signal musicPlayPauseQML();

    property bool iAmActive: false
    property bool levelContinuable: false
    // this signal, when received from mygraphicsscene.cpp, initiates animation of the UI.
    // The animation is not started automatically because startmenu.width and .height may change on different devices (see mygraphicsscene.cpp)
    signal beginAnimation()
    onBeginAnimation:{
        rectangleMovement1.start();
        rectangleMovement2.start();
        rectangleMovement3.start();
        rectangleMovement4.start();
    }
    onGameFinishedQML:{
        gameFinished.numTrapsUsed = numTrUs;
        gameFinished.numTimeUsed = numTiUs;
        endRectangleMovement.start();
    }
    Text {
        id: myText
        anchors.centerIn: parent
        color: "white"
        text: "Trap, Shake, Kill 'em!"
        font.pixelSize: Math.floor(startmenu.height/10); //50
        smooth: true
        SequentialAnimation on font.letterSpacing {
            running: appWindow.iAmActive
            loops: Animation.Infinite;
            NumberAnimation { from: 0; to: 100; easing.type: Easing.InQuad; duration: 4000 }
        }
        SequentialAnimation on font.pixelSize {
            running: appWindow.iAmActive
            loops: Animation.Infinite;
            NumberAnimation { from: 1; to: 50; easing.type: Easing.InQuad; duration: 2000 }
            PauseAnimation { duration: 2000 }
        }
        SequentialAnimation on opacity {
            running: appWindow.iAmActive
            loops: Animation.Infinite;
            NumberAnimation { from: 1; to: 0; duration: 3500 }
            PauseAnimation { duration: 500 }
        }
    }
    Rectangle {
        id: startGame
        opacity: 0.7
        width: startmenu.width/3
        height: startmenu.width/6
        gradient: Gradient {
                 //GradientStop { position: 0.0; color: "gold" }
                 //GradientStop { position: 1.0; color: "darkorange" }
                 GradientStop { position: 0.0; color: "lightsteelblue" }
                 GradientStop { position: 1.0; color: "gray" }
        }
        border.color: "black"
        border.width: 2
        radius: 10
        //anchors.right: startmenu.right
        //anchors.bottom: startmenu.bottom
        x: startmenu.width - startGame.width-startGame.border.width - 3
        y: startmenu.height

        SequentialAnimation on y {
            id: rectangleMovement1
            running: false
            PauseAnimation { duration: 1000 }
            PropertyAnimation { to: startmenu.height-startGame.height-startGame.border.width - 3; duration: 1200; easing.type: Easing.OutElastic}
        }
        Text{
            id: startGameText
            text: "START GAME"
            anchors.horizontalCenter: startGame.horizontalCenter
            anchors.verticalCenter: startGame.verticalCenter
            font.pixelSize: Math.floor(startmenu.height/12); font.bold: false
        }
        MouseArea {
                 anchors.fill: parent
                 // when clicked, send a one
                 onClicked: {
                     startmenu.sendRumbleMessage(1) // 1 is dummy
                     startmenu.sendQMLMessage(1, false)
                 }//console.log("button clicked")
        }
    }
    Rectangle {
        id: continueGame
        width: startmenu.width/2.5
        height: startmenu.width/9
        opacity: 0.2 + levelContinuable*0.5
        gradient: Gradient {
                 GradientStop { position: 0.0; color: "lightsteelblue" }
                 GradientStop { position: 1.0; color: "gray" }
        }
        border.color: "black"
        border.width: 2
        radius: 10
        anchors.right: startGame.left
        anchors.bottom: startGame.bottom
        /*
        x: startmenu.width - startGame.width - continueGame.width - 2*continueGame.border.width - 3
        y: startmenu.height

        SequentialAnimation on y {
            id: rectangleMovement4
            running: false
            PauseAnimation { duration: 1000 }
            PropertyAnimation { to: startmenu.height-continueGame.height-continueGame.border.width - 3; duration: 1200; easing.type: Easing.OutElastic}
        }
        */
        Text{
            id: continueGameText
            text: "CONTINUE GAME"
            anchors.horizontalCenter: continueGame.horizontalCenter
            anchors.verticalCenter: continueGame.verticalCenter
            font.pixelSize: Math.floor(startmenu.height/12); font.bold: false
        }
        MouseArea {
                 anchors.fill: parent
                 // when clicked, send a one
                 onClicked: {
                     if(levelContinuable){
                         startmenu.sendRumbleMessage(1) // 1 is dummy
                         startmenu.sendQMLMessage(1, true)
                         }
                 }
        }
    }
    Rectangle {
        id: endGame
        width: startmenu.width/6
        height: startmenu.width/6
        opacity: 0.7
        gradient: Gradient {
                 GradientStop { position: 0.0; color: "lightsteelblue" }
                 GradientStop { position: 1.0; color: "gray" }
        }
        border.color: "black"
        border.width: 2
        radius: 10
        x: 0+startGame.border.width + 3
        y: startmenu.height
        SequentialAnimation on y {
            id: rectangleMovement2
            running: false
            PauseAnimation { duration: 1600 }
            PropertyAnimation { to: startmenu.height-endGame.height-startGame.border.width - 3; duration: 1200; easing.type: Easing.OutElastic}
        }
        //anchors.left: startmenu.left
        //anchors.bottom: startmenu.bottom
        Text{
            id: endGameText
            text: "QUIT"
            anchors.horizontalCenter: endGame.horizontalCenter
            anchors.verticalCenter: endGame.verticalCenter
            font.pixelSize: startGameText.font.pixelSize; font.bold: false
        }
        MouseArea {
                 anchors.fill: parent
                 // when clicked, send a zero
                 onClicked: {
                     startmenu.sendRumbleMessage(1) // 1 is dummy
                     //startmenu.sendExitMessage()//console.log("button clicked")
                     Qt.quit();
                 }
        }
    }
    //
    // PLAY PAUSE MUSIC
    //
    Rectangle {
        width: startmenu.width/8
        height: startmenu.width/8
        anchors.top: instr.top
        anchors.right: instr.left
        opacity: 0.7
        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            GradientStop { position: 1.0; color: "gray" }
        }
        border.color: "black"
        border.width: 2
        radius: height/2
        Text{
            text: "||"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: startGameText.font.pixelSize; font.bold: true
        }
        //Image {
        //    anchors.fill: parent
        //    source: "images/media-playback-pause.png"
        //}
        MouseArea {
            anchors.fill: parent
            // when clicked, send a zero
            onClicked: {
                startmenu.sendRumbleMessage(1); // 1 is dummy
                startmenu.musicPlayPauseQML(); // 
                }
            }
    }
    
    //
    // HELP!
    //
    Rectangle {
        id: instr
        width: startmenu.width/3
        height: startmenu.width/6
        opacity: 0.7
        gradient: Gradient {
                 //GradientStop { position: 0.0; color: "limegreen" }
                 //GradientStop { position: 1.0; color: "lime" }
                 GradientStop { position: 0.0; color: "lightsteelblue" }
                 GradientStop { position: 1.0; color: "gray" }
        }
        border.color: "black"
        border.width: 2
        radius: 10
        x: startmenu.width - instr.width-startGame.border.width - 3
        y: -instr.height - 3
        SequentialAnimation on y {
            id: rectangleMovement3
            running: false
            PauseAnimation { duration: 500 }
            PropertyAnimation { to: startGame.border.width + 3; duration: 1200; easing.type: Easing.OutElastic}
        }
        Text{
            id: instrText
            text: "HELP"
            anchors.horizontalCenter: instr.horizontalCenter
            anchors.verticalCenter: instr.verticalCenter
            font.pixelSize: startGameText.font.pixelSize; font.bold: false
        }
        MouseArea {
                 anchors.fill: parent
                 // when clicked, send a zero
                 onClicked: {
                     insertextbox.state = 'moved';
                     startmenu.sendRumbleMessage(1); // 1 is dummy

                 }
        }
    }

    Rectangle {
        id: insertextbox
        width: startmenu.width/1.5
        height: insertextboxText.paintedHeight
        border.color: "black"
        border.width: 2
        gradient: Gradient {
                 //GradientStop { position: 0.0; color: "limegreen" }
                 //GradientStop { position: 1.0; color: "lawngreen" }
                 GradientStop { position: 0.0; color: "gray" }
                 GradientStop { position: 1.0; color: "darkGray" }
        }
        x: startmenu.width + insertextbox.border.width
        y: -insertextbox.height - insertextbox.border.width
        Text{
            id: insertextboxText
            width: insertextbox.width
            wrapMode: Text.Wrap
            text: "Use both thumbs to create a trap on screen. Catch an enemy in the trap and shake the device for a kill! Don't kill the friendly U.F.O:s though! The circle at the top right corner indicates how many traps remain by getting smaller in diameter after each deployed trap. Segments of the circle are also eaten away by time! <br /> Images: ESA/NASA http://www.spacetelescope.org/<br />Music: http://www.dreamstate.to/"
            anchors.fill: insertextbox
            //anchors.verticalCenter: instr.verticalCenter
            font.pixelSize: Math.floor(startGameText.font.pixelSize*0.8); font.bold: false
        }
        MouseArea {
                anchors.fill: parent
                onPressed: insertextbox.state = 'original';
                //onReleased: myRect.state = '';
            }
        states: [
            State {
                name: "original";
                PropertyChanges { target: insertextbox; x: startmenu.width+insertextbox.border.width + 3; y:-startmenu.height-insertextbox.border.width - 3; color: "blue" }
            },
            State {
                name: "moved"
                PropertyChanges { target: insertextbox; x: startmenu.width-insertextbox.width-insertextbox.border.width - 3; y: insertextbox.border.width - 3; color: "orange" }
            }
        ]
        transitions: [
                Transition {
                    NumberAnimation { properties: "x,y"; duration: 300; easing.type: Easing.OutBack }
                }
            ]
    }
    
    //
    // GAME FINISHED - screen
    //
    Rectangle {
        id: gameFinished
        width: startmenu.width
        height: startmenu.height
        property int numTrapsUsed: 0
        property int numTimeUsed: 0
        x: startmenu.width
        y: 0
        SequentialAnimation on x {
            id: endRectangleMovement
            running: false
            PauseAnimation { duration: 200 }
            PropertyAnimation { to: 0; duration: 1200; easing.type: Easing.OutElastic}
        }
        Image {
            anchors.fill: parent
            source: "images/gameFinishedBG.jpg"
        }
    
        Text{
            id: scoreText
            anchors.left: endGame2.right
            anchors.top: parent.top
            font.pixelSize: Math.floor(startmenu.height/10);
            style: Text.Outline; styleColor: "red"
            color: "crimson"
            text: "Traps deployed: " + gameFinished.numTrapsUsed + ". Time used: " + gameFinished.numTimeUsed
        }
    
        ListModel {
            id: finishedList
            ListElement {
                myTopic: "Congratulations!"
                myDescription: "You have just finished all levels. Well done and thanks for playing!"
            }
            ListElement {
                myTopic: "Credits:"
                myDescription: "All code by Miika Ahdesmaki 2013. Feedback welcome: miika.ahdesmaki@ovi.com"
            }
         }
        ListView {
            //anchors.centerIn: parent
            anchors.top: endGame2.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            width: gameFinished.width * 0.8
            //height: 350
            model: finishedList
            delegate: Text {
                width: parent.width
                wrapMode: Text.WordWrap
                font.pixelSize: Math.floor(startmenu.height/16);
                //color: "steelblue"
                //style: Text.Raised; styleColor: "white"
                color: "lightskyblue"
                style: Text.Outline; styleColor: "darkblue"
                text: myTopic + "<br />" + myDescription + "<br />"
            }
         }
    
        Rectangle {
            id: endGame2
            width: startmenu.width/6
            height: startmenu.width/6
            opacity: 0.7
            gradient: Gradient {
                     GradientStop { position: 0.0; color: "lightsteelblue" }
                     GradientStop { position: 1.0; color: "gray" }
            }
            border.color: "black"
            border.width: 2
            radius: 10
            anchors.top: parent.top
            anchors.left: parent.left
    
            Text{
                id: endGameText2
                text: "QUIT"
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: Math.floor(startmenu.height/10); 
                //font.bold: false
            }
            MouseArea {
                     anchors.fill: parent
                     onClicked: {
                         startmenu.sendRumbleMessage(1) // 1 is dummy
                         Qt.quit();
                     }
            } // end mousearea
        } // end Rectangle
    } // end gameFinished
}// end main screen
