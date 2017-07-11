/*
    Copyright (C) 2017 Pelagicore AB

    Authored by: Lukáš Tinkl <ltinkl@luxoft.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <QObject>

#include "peluxdevicemanager_global.h"
#include "peluxdevicemanagerenums.h"

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString parentId READ parentId)
    Q_PROPERTY(PeluxDeviceManagerEnums::DeviceType deviceType READ deviceType)
    Q_PROPERTY(QString vendor READ vendor)
    Q_PROPERTY(QString product READ product)
    Q_PROPERTY(QString description READ description)
    Q_PROPERTY(QString icon READ icon)
    Q_PROPERTY(QStringList emblems READ emblems NOTIFY statusChanged)
    Q_PROPERTY(PeluxDeviceManagerEnums::ConnectionStatus status READ status WRITE setStatus NOTIFY statusChanged)

public:
    explicit PeluxDevice(QObject *parent = nullptr);
    ~PeluxDevice() = default;

    virtual QString id() const = 0;
    virtual QString parentId() const = 0;
    virtual PeluxDeviceManagerEnums::DeviceType deviceType() const = 0;
    virtual QString vendor() const = 0;
    virtual QString product() const = 0;
    virtual QString description() const = 0;
    virtual QString icon() const = 0;
    virtual QStringList emblems() const = 0;

    virtual PeluxDeviceManagerEnums::ConnectionStatus status() const = 0;
    virtual void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status) = 0;

Q_SIGNALS:
    void statusChanged(PeluxDeviceManagerEnums::ConnectionStatus status);
};