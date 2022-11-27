/****************************************************************************
** Meta object code from reading C++ file 'download.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.14.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../serialport_ymodem_pro/download.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'download.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.14.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_Download_t {
    QByteArrayData data[13];
    char stringdata0[165];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Download_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Download_t qt_meta_stringdata_Download = {
    {
QT_MOC_LITERAL(0, 0, 8), // "Download"
QT_MOC_LITERAL(1, 9, 13), // "signalSendMsg"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 4), // "sMsg"
QT_MOC_LITERAL(4, 29, 21), // "signalSendDownloadMsg"
QT_MOC_LITERAL(5, 51, 25), // "signalSendDownloadProcess"
QT_MOC_LITERAL(6, 77, 12), // "processValue"
QT_MOC_LITERAL(7, 90, 21), // "signalSendDownloadEnd"
QT_MOC_LITERAL(8, 112, 7), // "doWork1"
QT_MOC_LITERAL(9, 120, 8), // "portName"
QT_MOC_LITERAL(10, 129, 8), // "baudRate"
QT_MOC_LITERAL(11, 138, 8), // "fileSize"
QT_MOC_LITERAL(12, 147, 17) // "yModemPackageSize"

    },
    "Download\0signalSendMsg\0\0sMsg\0"
    "signalSendDownloadMsg\0signalSendDownloadProcess\0"
    "processValue\0signalSendDownloadEnd\0"
    "doWork1\0portName\0baudRate\0fileSize\0"
    "yModemPackageSize"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Download[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    1,   42,    2, 0x06 /* Public */,
       5,    1,   45,    2, 0x06 /* Public */,
       7,    1,   48,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    4,   51,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    6,
    QMetaType::Void, QMetaType::QString,    3,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int, QMetaType::Int,    9,   10,   11,   12,

       0        // eod
};

void Download::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<Download *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signalSendMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->signalSendDownloadMsg((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->signalSendDownloadProcess((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->signalSendDownloadEnd((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->doWork1((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (Download::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Download::signalSendMsg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (Download::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Download::signalSendDownloadMsg)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (Download::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Download::signalSendDownloadProcess)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (Download::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&Download::signalSendDownloadEnd)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject Download::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_Download.data,
    qt_meta_data_Download,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *Download::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Download::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_Download.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int Download::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void Download::signalSendMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void Download::signalSendDownloadMsg(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void Download::signalSendDownloadProcess(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void Download::signalSendDownloadEnd(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
