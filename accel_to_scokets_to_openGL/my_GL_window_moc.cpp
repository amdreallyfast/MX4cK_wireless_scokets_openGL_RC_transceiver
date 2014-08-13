/****************************************************************************
** Meta object code from reading C++ file 'my_GL_window.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "my_GL_window.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'my_GL_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_my_GL_window[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      14,   13,   13,   13, 0x08,
      35,   29,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_my_GL_window[] = {
    "my_GL_window\0\0timer_update()\0X,Y,Z\0"
    "receive_serial_data(float,float,float)\0"
};

void my_GL_window::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        my_GL_window *_t = static_cast<my_GL_window *>(_o);
        switch (_id) {
        case 0: _t->timer_update(); break;
        case 1: _t->receive_serial_data((*reinterpret_cast< float(*)>(_a[1])),(*reinterpret_cast< float(*)>(_a[2])),(*reinterpret_cast< float(*)>(_a[3]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData my_GL_window::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject my_GL_window::staticMetaObject = {
    { &QGLWidget::staticMetaObject, qt_meta_stringdata_my_GL_window,
      qt_meta_data_my_GL_window, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &my_GL_window::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *my_GL_window::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *my_GL_window::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_my_GL_window))
        return static_cast<void*>(const_cast< my_GL_window*>(this));
    return QGLWidget::qt_metacast(_clname);
}

int my_GL_window::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGLWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
