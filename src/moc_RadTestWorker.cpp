/****************************************************************************
** Meta object code from reading C++ file 'RadTestWorker.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/RadTestWorker.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadTestWorker.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RadTestWorker_t {
    QByteArrayData data[16];
    char stringdata0[140];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RadTestWorker_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RadTestWorker_t qt_meta_stringdata_RadTestWorker = {
    {
QT_MOC_LITERAL(0, 0, 13), // "RadTestWorker"
QT_MOC_LITERAL(1, 14, 14), // "counterchanged"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 9), // "needReset"
QT_MOC_LITERAL(4, 40, 5), // "level"
QT_MOC_LITERAL(5, 46, 9), // "f7_needed"
QT_MOC_LITERAL(6, 56, 1), // "e"
QT_MOC_LITERAL(7, 58, 7), // "runTest"
QT_MOC_LITERAL(8, 66, 8), // "turnOnF0"
QT_MOC_LITERAL(9, 75, 8), // "turnOnF1"
QT_MOC_LITERAL(10, 84, 8), // "turnOnF2"
QT_MOC_LITERAL(11, 93, 8), // "turnOnF3"
QT_MOC_LITERAL(12, 102, 9), // "turnOnF4a"
QT_MOC_LITERAL(13, 112, 9), // "turnOnF4b"
QT_MOC_LITERAL(14, 122, 8), // "turnOnF5"
QT_MOC_LITERAL(15, 131, 8) // "turnOnF7"

    },
    "RadTestWorker\0counterchanged\0\0needReset\0"
    "level\0f7_needed\0e\0runTest\0turnOnF0\0"
    "turnOnF1\0turnOnF2\0turnOnF3\0turnOnF4a\0"
    "turnOnF4b\0turnOnF5\0turnOnF7"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RadTestWorker[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x06 /* Public */,
       3,    1,   80,    2, 0x06 /* Public */,
       3,    0,   83,    2, 0x26 /* Public | MethodCloned */,
       5,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   87,    2, 0x08 /* Private */,
       8,    1,   88,    2, 0x0a /* Public */,
       9,    1,   91,    2, 0x0a /* Public */,
      10,    1,   94,    2, 0x0a /* Public */,
      11,    1,   97,    2, 0x0a /* Public */,
      12,    1,  100,    2, 0x0a /* Public */,
      13,    1,  103,    2, 0x0a /* Public */,
      14,    1,  106,    2, 0x0a /* Public */,
      15,    1,  109,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    6,

       0        // eod
};

void RadTestWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RadTestWorker *_t = static_cast<RadTestWorker *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->counterchanged(); break;
        case 1: _t->needReset((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->needReset(); break;
        case 3: _t->f7_needed((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 4: _t->runTest(); break;
        case 5: _t->turnOnF0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 6: _t->turnOnF1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 7: _t->turnOnF2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->turnOnF3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 9: _t->turnOnF4a((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->turnOnF4b((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->turnOnF5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->turnOnF7((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (RadTestWorker::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadTestWorker::counterchanged)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (RadTestWorker::*_t)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadTestWorker::needReset)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (RadTestWorker::*_t)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RadTestWorker::f7_needed)) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject RadTestWorker::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RadTestWorker.data,
      qt_meta_data_RadTestWorker,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RadTestWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RadTestWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RadTestWorker.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int RadTestWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void RadTestWorker::counterchanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void RadTestWorker::needReset(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 3
void RadTestWorker::f7_needed(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
