/****************************************************************************
** Meta object code from reading C++ file 'cplatediliverythread.h'
**
** Created: Thu Jun 28 14:36:39 2012
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../PlateDilivery/cplatediliverythread.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'cplatediliverythread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_CPlateDiliveryThread[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: signature, parameters, type, tag, flags
      30,   22,   21,   21, 0x05,

 // slots: signature, parameters, type, tag, flags
      59,   22,   21,   21, 0x0a,
      92,   21,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_CPlateDiliveryThread[] = {
    "CPlateDiliveryThread\0\0lstData\0"
    "WeighingRequest(QStringList)\0"
    "HandlePlateDilivery(QStringList)\0"
    "IncommingData()\0"
};

const QMetaObject CPlateDiliveryThread::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_CPlateDiliveryThread,
      qt_meta_data_CPlateDiliveryThread, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &CPlateDiliveryThread::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *CPlateDiliveryThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *CPlateDiliveryThread::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CPlateDiliveryThread))
        return static_cast<void*>(const_cast< CPlateDiliveryThread*>(this));
    return QThread::qt_metacast(_clname);
}

int CPlateDiliveryThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: WeighingRequest((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 1: HandlePlateDilivery((*reinterpret_cast< QStringList(*)>(_a[1]))); break;
        case 2: IncommingData(); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void CPlateDiliveryThread::WeighingRequest(QStringList _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
