/****************************************************************************
** Meta object code from reading C++ file 'RadTestGUI.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.7)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../include/RadTestGUI.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadTestGUI.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyQTextEdit_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyQTextEdit_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyQTextEdit_t qt_meta_stringdata_MyQTextEdit = {
    {
QT_MOC_LITERAL(0, 0, 11) // "MyQTextEdit"

    },
    "MyQTextEdit"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyQTextEdit[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void MyQTextEdit::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject MyQTextEdit::staticMetaObject = {
    { &QTextEdit::staticMetaObject, qt_meta_stringdata_MyQTextEdit.data,
      qt_meta_data_MyQTextEdit,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MyQTextEdit::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyQTextEdit::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MyQTextEdit.stringdata0))
        return static_cast<void*>(this);
    return QTextEdit::qt_metacast(_clname);
}

int MyQTextEdit::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTextEdit::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_FunctionBar_t {
    QByteArrayData data[1];
    char stringdata0[12];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FunctionBar_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FunctionBar_t qt_meta_stringdata_FunctionBar = {
    {
QT_MOC_LITERAL(0, 0, 11) // "FunctionBar"

    },
    "FunctionBar"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FunctionBar[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void FunctionBar::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject FunctionBar::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_FunctionBar.data,
      qt_meta_data_FunctionBar,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *FunctionBar::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FunctionBar::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FunctionBar.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FunctionBar::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    return _id;
}
struct qt_meta_stringdata_RadTestGUI_t {
    QByteArrayData data[21];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RadTestGUI_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RadTestGUI_t qt_meta_stringdata_RadTestGUI = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RadTestGUI"
QT_MOC_LITERAL(1, 11, 5), // "start"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 9), // "pausestep"
QT_MOC_LITERAL(4, 28, 13), // "insertComment"
QT_MOC_LITERAL(5, 42, 13), // "updatenumbers"
QT_MOC_LITERAL(6, 56, 4), // "stop"
QT_MOC_LITERAL(7, 61, 14), // "resetRequested"
QT_MOC_LITERAL(8, 76, 5), // "level"
QT_MOC_LITERAL(9, 82, 19), // "disableAllFunctions"
QT_MOC_LITERAL(10, 102, 1), // "e"
QT_MOC_LITERAL(11, 104, 9), // "turnOnBar"
QT_MOC_LITERAL(12, 114, 1), // "i"
QT_MOC_LITERAL(13, 116, 8), // "turnOnF0"
QT_MOC_LITERAL(14, 125, 8), // "turnOnF1"
QT_MOC_LITERAL(15, 134, 8), // "turnOnF2"
QT_MOC_LITERAL(16, 143, 8), // "turnOnF3"
QT_MOC_LITERAL(17, 152, 9), // "turnOnF4a"
QT_MOC_LITERAL(18, 162, 9), // "turnOnF4b"
QT_MOC_LITERAL(19, 172, 8), // "turnOnF5"
QT_MOC_LITERAL(20, 181, 8) // "turnOnF7"

    },
    "RadTestGUI\0start\0\0pausestep\0insertComment\0"
    "updatenumbers\0stop\0resetRequested\0"
    "level\0disableAllFunctions\0e\0turnOnBar\0"
    "i\0turnOnF0\0turnOnF1\0turnOnF2\0turnOnF3\0"
    "turnOnF4a\0turnOnF4b\0turnOnF5\0turnOnF7"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RadTestGUI[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      17,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   99,    2, 0x08 /* Private */,
       3,    0,  100,    2, 0x08 /* Private */,
       4,    0,  101,    2, 0x08 /* Private */,
       5,    0,  102,    2, 0x08 /* Private */,
       6,    0,  103,    2, 0x08 /* Private */,
       7,    1,  104,    2, 0x08 /* Private */,
       7,    0,  107,    2, 0x28 /* Private | MethodCloned */,
       9,    1,  108,    2, 0x08 /* Private */,
      11,    2,  111,    2, 0x08 /* Private */,
      13,    1,  116,    2, 0x08 /* Private */,
      14,    1,  119,    2, 0x08 /* Private */,
      15,    1,  122,    2, 0x08 /* Private */,
      16,    1,  125,    2, 0x08 /* Private */,
      17,    1,  128,    2, 0x08 /* Private */,
      18,    1,  131,    2, 0x08 /* Private */,
      19,    1,  134,    2, 0x08 /* Private */,
      20,    1,  137,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Int, QMetaType::Bool,   12,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,
    QMetaType::Void, QMetaType::Bool,   10,

       0        // eod
};

void RadTestGUI::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RadTestGUI *_t = static_cast<RadTestGUI *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->start(); break;
        case 1: _t->pausestep(); break;
        case 2: _t->insertComment(); break;
        case 3: _t->updatenumbers(); break;
        case 4: _t->stop(); break;
        case 5: _t->resetRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->resetRequested(); break;
        case 7: _t->disableAllFunctions((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 8: _t->turnOnBar((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 9: _t->turnOnF0((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->turnOnF1((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->turnOnF2((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 12: _t->turnOnF3((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->turnOnF4a((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 14: _t->turnOnF4b((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 15: _t->turnOnF5((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->turnOnF7((*reinterpret_cast< bool(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject RadTestGUI::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_RadTestGUI.data,
      qt_meta_data_RadTestGUI,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *RadTestGUI::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RadTestGUI::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RadTestGUI.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int RadTestGUI::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 17)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 17;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
