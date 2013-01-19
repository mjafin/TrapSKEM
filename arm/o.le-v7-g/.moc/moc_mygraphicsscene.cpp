/****************************************************************************
** Meta object code from reading C++ file 'mygraphicsscene.h'
**
** Created: Sat Jan 19 16:19:06 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/mygraphicsscene.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mygraphicsscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_myGraphicsScene[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x05,
      32,   16,   16,   16, 0x05,
      77,   52,   16,   16, 0x05,

 // slots: signature, parameters, type, tag, flags
     134,   99,   16,   16, 0x0a,
     172,  158,   16,   16, 0x2a,
     199,  191,   16,   16, 0x0a,
     228,  222,   16,   16, 0x0a,
     275,   16,   16,   16, 0x0a,
     294,   16,   16,   16, 0x0a,
     318,  312,  307,   16, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_myGraphicsScene[] = {
    "myGraphicsScene\0\0callingQuits()\0"
    "beginQmlAnimation()\0numTrapsUsed,numTimeUsed\0"
    "gameFinished(int,int)\0"
    "stateChangeID,continuePreviousGame\0"
    "signalHandler(int,bool)\0stateChangeID\0"
    "signalHandler(int)\0notUsed\0"
    "myRumbleGenerator(int)\0state\0"
    "startMyMusic(bb::multimedia::MediaState::Type)\0"
    "pausePlayMyMusic()\0stopMyGame()\0bool\0"
    "event\0event(QEvent*)\0"
};

void myGraphicsScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        myGraphicsScene *_t = static_cast<myGraphicsScene *>(_o);
        switch (_id) {
        case 0: _t->callingQuits(); break;
        case 1: _t->beginQmlAnimation(); break;
        case 2: _t->gameFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->signalHandler((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 4: _t->signalHandler((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->myRumbleGenerator((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->startMyMusic((*reinterpret_cast< bb::multimedia::MediaState::Type(*)>(_a[1]))); break;
        case 7: _t->pausePlayMyMusic(); break;
        case 8: _t->stopMyGame(); break;
        case 9: { bool _r = _t->event((*reinterpret_cast< QEvent*(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    }
}

const QMetaObjectExtraData myGraphicsScene::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject myGraphicsScene::staticMetaObject = {
    { &QGraphicsScene::staticMetaObject, qt_meta_stringdata_myGraphicsScene,
      qt_meta_data_myGraphicsScene, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &myGraphicsScene::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *myGraphicsScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *myGraphicsScene::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_myGraphicsScene))
        return static_cast<void*>(const_cast< myGraphicsScene*>(this));
    return QGraphicsScene::qt_metacast(_clname);
}

int myGraphicsScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGraphicsScene::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void myGraphicsScene::callingQuits()
{
    QMetaObject::activate(this, &staticMetaObject, 0, 0);
}

// SIGNAL 1
void myGraphicsScene::beginQmlAnimation()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}

// SIGNAL 2
void myGraphicsScene::gameFinished(int _t1, int _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
