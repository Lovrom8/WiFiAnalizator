/****************************************************************************
** Meta object code from reading C++ file 'Citac.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.9)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "Citac.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Citac.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.9. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Citac_t {
    QByteArrayData data[8];
    char stringdata0[48];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Citac_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Citac_t qt_meta_stringdata_Citac = {
    {
QT_MOC_LITERAL(0, 0, 5), // "Citac"
QT_MOC_LITERAL(1, 6, 9), // "noviOkvir"
QT_MOC_LITERAL(2, 16, 0), // ""
QT_MOC_LITERAL(3, 17, 5), // "Okvir"
QT_MOC_LITERAL(4, 23, 5), // "okvir"
QT_MOC_LITERAL(5, 29, 8), // "noviCvor"
QT_MOC_LITERAL(6, 38, 4), // "Cvor"
QT_MOC_LITERAL(7, 43, 4) // "cvor"

    },
    "Citac\0noviOkvir\0\0Okvir\0okvir\0noviCvor\0"
    "Cvor\0cvor"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Citac[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x06 /* Public */,
       5,    1,   27,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,

       0        // eod
};

void Citac::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Citac *_t = static_cast<Citac *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->noviOkvir((*reinterpret_cast< Okvir(*)>(_a[1]))); break;
        case 1: _t->noviCvor((*reinterpret_cast< Cvor(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (Citac::*_t)(Okvir );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Citac::noviOkvir)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (Citac::*_t)(Cvor );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Citac::noviCvor)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject Citac::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_Citac.data,
      qt_meta_data_Citac,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *Citac::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Citac::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Citac.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Citac::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void Citac::noviOkvir(Okvir _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Citac::noviCvor(Cvor _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
