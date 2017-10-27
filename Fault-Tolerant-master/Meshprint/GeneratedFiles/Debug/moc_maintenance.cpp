/****************************************************************************
** Meta object code from reading C++ file 'maintenance.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../maintenance.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'maintenance.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MyDialog_t {
    QByteArrayData data[13];
    char stringdata0[253];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MyDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MyDialog_t qt_meta_stringdata_MyDialog = {
    {
QT_MOC_LITERAL(0, 0, 8), // "MyDialog"
QT_MOC_LITERAL(1, 9, 7), // "SendMsg"
QT_MOC_LITERAL(2, 17, 0), // ""
QT_MOC_LITERAL(3, 18, 23), // "on_updateButton_clicked"
QT_MOC_LITERAL(4, 42, 28), // "on_maintenanceButton_clicked"
QT_MOC_LITERAL(5, 71, 22), // "on_clearButton_clicked"
QT_MOC_LITERAL(6, 94, 26), // "on_markcolorbutton_clicked"
QT_MOC_LITERAL(7, 121, 25), // "on_createTriangle_clicked"
QT_MOC_LITERAL(8, 147, 21), // "on_repairhole_clicked"
QT_MOC_LITERAL(9, 169, 30), // "on_intersectAndoverlapCheckBox"
QT_MOC_LITERAL(10, 200, 28), // "on_markbadisdebutton_clicked"
QT_MOC_LITERAL(11, 229, 11), // "DialogClose"
QT_MOC_LITERAL(12, 241, 11) // "receiveData"

    },
    "MyDialog\0SendMsg\0\0on_updateButton_clicked\0"
    "on_maintenanceButton_clicked\0"
    "on_clearButton_clicked\0"
    "on_markcolorbutton_clicked\0"
    "on_createTriangle_clicked\0"
    "on_repairhole_clicked\0"
    "on_intersectAndoverlapCheckBox\0"
    "on_markbadisdebutton_clicked\0DialogClose\0"
    "receiveData"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MyDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   72,    2, 0x0a /* Public */,
       4,    0,   73,    2, 0x0a /* Public */,
       5,    0,   74,    2, 0x0a /* Public */,
       6,    0,   75,    2, 0x0a /* Public */,
       7,    0,   76,    2, 0x0a /* Public */,
       8,    0,   77,    2, 0x0a /* Public */,
       9,    0,   78,    2, 0x0a /* Public */,
      10,    0,   79,    2, 0x0a /* Public */,
      11,    0,   80,    2, 0x0a /* Public */,
      12,    1,   81,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,

       0        // eod
};

void MyDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MyDialog *_t = static_cast<MyDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->on_updateButton_clicked(); break;
        case 2: _t->on_maintenanceButton_clicked(); break;
        case 3: _t->on_clearButton_clicked(); break;
        case 4: _t->on_markcolorbutton_clicked(); break;
        case 5: _t->on_createTriangle_clicked(); break;
        case 6: _t->on_repairhole_clicked(); break;
        case 7: _t->on_intersectAndoverlapCheckBox(); break;
        case 8: _t->on_markbadisdebutton_clicked(); break;
        case 9: _t->DialogClose(); break;
        case 10: _t->receiveData((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MyDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MyDialog::SendMsg)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MyDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_MyDialog.data,
      qt_meta_data_MyDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MyDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MyDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MyDialog.stringdata0))
        return static_cast<void*>(const_cast< MyDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int MyDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}

// SIGNAL 0
void MyDialog::SendMsg(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
