#pragma once

#include <QHash>

class PeluxDeviceManager;

class PeluxDeviceManagerPrivate
{
public:
    Q_DECLARE_PUBLIC(PeluxDeviceManager)
    PeluxDeviceManager *q_ptr;

    PeluxDeviceManagerPrivate(PeluxDeviceManager *qptr);
    QHash<int, QByteArray> roles;
};
