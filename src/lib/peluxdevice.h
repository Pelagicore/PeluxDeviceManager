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

    Q_PROPERTY(QString id READ id CONSTANT)
    Q_PROPERTY(QString parentId READ parentId CONSTANT)
    Q_PROPERTY(PeluxDeviceManagerEnums::DeviceType deviceType READ deviceType CONSTANT)
    Q_PROPERTY(QString vendor READ vendor CONSTANT)
    Q_PROPERTY(QString product READ product CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString icon READ icon CONSTANT)
    Q_PROPERTY(QStringList emblems READ emblems NOTIFY statusChanged)
    Q_PROPERTY(PeluxDeviceManagerEnums::ConnectionStatus status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString mountPoint READ mountPoint NOTIFY statusChanged)
    Q_PROPERTY(QString device READ device CONSTANT)
    Q_PROPERTY(bool removable READ isRemovable CONSTANT)
    Q_PROPERTY(PeluxDeviceManagerEnums::DriveType driveType READ driveType CONSTANT)

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
    virtual QString mountPoint() const = 0;
    virtual QString device() const = 0;
    virtual bool isRemovable() const = 0;
    virtual PeluxDeviceManagerEnums::DriveType driveType() const = 0;

    PeluxDeviceManagerEnums::ConnectionStatus status() const;
    virtual void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status) = 0;

protected:
    PeluxDeviceManagerEnums::ConnectionStatus m_status{PeluxDeviceManagerEnums::UnknownConnectionStatus};

Q_SIGNALS:
    void statusChanged(PeluxDeviceManagerEnums::ConnectionStatus status);
};
