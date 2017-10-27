/****************************************************************************
** Meta object code from reading C++ file 'renderingwidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../renderingwidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'renderingwidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RenderingWidget_t {
    QByteArrayData data[16];
    char stringdata0[196];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RenderingWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RenderingWidget_t qt_meta_stringdata_RenderingWidget = {
    {
QT_MOC_LITERAL(0, 0, 15), // "RenderingWidget"
QT_MOC_LITERAL(1, 16, 15), // "sendMsgtoDialog"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "ResetView"
QT_MOC_LITERAL(4, 43, 7), // "RecvMsg"
QT_MOC_LITERAL(5, 51, 16), // "ApplyMaintenance"
QT_MOC_LITERAL(6, 68, 13), // "SetBackground"
QT_MOC_LITERAL(7, 82, 8), // "ReadMesh"
QT_MOC_LITERAL(8, 91, 9), // "WriteMesh"
QT_MOC_LITERAL(9, 101, 14), // "CheckDrawPoint"
QT_MOC_LITERAL(10, 116, 13), // "CheckDrawEdge"
QT_MOC_LITERAL(11, 130, 13), // "CheckDrawFace"
QT_MOC_LITERAL(12, 144, 10), // "CheckLight"
QT_MOC_LITERAL(13, 155, 9), // "CheckGrid"
QT_MOC_LITERAL(14, 165, 16), // "CheckDrawTexture"
QT_MOC_LITERAL(15, 182, 13) // "CheckDrawAxes"

    },
    "RenderingWidget\0sendMsgtoDialog\0\0"
    "ResetView\0RecvMsg\0ApplyMaintenance\0"
    "SetBackground\0ReadMesh\0WriteMesh\0"
    "CheckDrawPoint\0CheckDrawEdge\0CheckDrawFace\0"
    "CheckLight\0CheckGrid\0CheckDrawTexture\0"
    "CheckDrawAxes"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RenderingWidget[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   84,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    0,   87,    2, 0x0a /* Public */,
       4,    1,   88,    2, 0x0a /* Public */,
       5,    0,   91,    2, 0x0a /* Public */,
       6,    0,   92,    2, 0x0a /* Public */,
       7,    0,   93,    2, 0x0a /* Public */,
       8,    0,   94,    2, 0x0a /* Public */,
       9,    0,   95,    2, 0x0a /* Public */,
      10,    0,   96,    2, 0x0a /* Public */,
      11,    0,   97,    2, 0x0a /* Public */,
      12,    0,   98,    2, 0x0a /* Public */,
      13,    0,   99,    2, 0x0a /* Public */,
      14,    0,  100,    2, 0x0a /* Public */,
      15,    0,  101,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void RenderingWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RenderingWidget *_t = static_cast<RenderingWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendMsgtoDialog((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->ResetView(); break;
        case 2: _t->RecvMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->ApplyMaintenance(); break;
        case 4: _t->SetBackground(); break;
        case 5: _t->ReadMesh(); break;
        case 6: _t->WriteMesh(); break;
        case 7: _t->CheckDrawPoint(); break;
        case 8: _t->CheckDrawEdge(); break;
        case 9: _t->CheckDrawFace(); break;
        case 10: _t->CheckLight(); break;
        case 11: _t->CheckGrid(); break;
        case 12: _t->CheckDrawTexture(); break;
        case 13: _t->CheckDrawAxes(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (RenderingWidget::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&RenderingWidget::sendMsgtoDialog)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject RenderingWidget::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_RenderingWidget.data,
      qt_meta_data_RenderingWidget,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RenderingWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RenderingWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RenderingWidget.stringdata0))
        return static_cast<void*>(const_cast< RenderingWidget*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int RenderingWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void RenderingWidget::sendMsgtoDialog(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
