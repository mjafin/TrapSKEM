APP_NAME = TrapSKEM

CONFIG += qt warn_on cascades10 mobility
QT += core declarative opengl
MOBILITY += sensors
LIBS += -lbbsystem -lbbmultimedia -lbbdevice 
INCLUDEPATH += ../src ${QNX_TARGET}/usr/include/qt4/QtCore
DEPENDPATH += ../src ${QNX_TARGET}/usr/include/qt4/QtCore

include(config.pri)
