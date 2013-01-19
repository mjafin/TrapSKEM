/****************************************************************************
** Meta object code from reading C++ file 'bgtouch.h'
**
** Created: Thu Jan 17 22:25:45 2013
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../src/bgtouch.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'bgtouch.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_BGTouch[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: signature, parameters, type, tag, flags
      23,    9,    8,    8, 0x05,
      42,    8,    8,    8, 0x05,

 // slots: signature, parameters, type, tag, flags
      74,   61,    8,    8, 0x0a,
      98,    8,    8,    8, 0x0a,
     111,    8,    8,    8, 0x0a,
     122,    8,    8,    8, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_BGTouch[] = {
    "BGTouch\0\0stateChangeID\0bgTouchSignal(int)\0"
    "allEnemiesKilled()\0creatureType\0"
    "creatureSubtractor(int)\0levelEnded()\0"
    "gameOver()\0timeKeeper()\0"
};

void BGTouch::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        BGTouch *_t = static_cast<BGTouch *>(_o);
        switch (_id) {
        case 0: _t->bgTouchSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->allEnemiesKilled(); break;
        case 2: _t->creatureSubtractor((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->levelEnded(); break;
        case 4: _t->gameOver(); break;
        case 5: _t->timeKeeper(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData BGTouch::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject BGTouch::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_BGTouch,
      qt_meta_data_BGTouch, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &BGTouch::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *BGTouch::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *BGTouch::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_BGTouch))
        return static_cast<void*>(const_cast< BGTouch*>(this));
    if (!strcmp(_clname, "QGraphicsPixmapItem"))
        return static_cast< QGraphicsPixmapItem*>(const_cast< BGTouch*>(this));
    return QObject::qt_metacast(_clname);
}

int BGTouch::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void BGTouch::bgTouchSignal(int _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void BGTouch::allEnemiesKilled()
{
    QMetaObject::activate(this, &staticMetaObject, 1, 0);
}
QT_END_MOC_NAMESPACE
