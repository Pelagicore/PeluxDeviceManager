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

#include <Solid/Device>

#include "peluxdevice.h"

class PeluxSolidDevice : public PeluxDevice
{
    Q_OBJECT
public:
    explicit PeluxSolidDevice(const Solid::Device &solidDevice, QObject * parent);
    ~PeluxSolidDevice() = default;

    QString id() const;
    QString parentId() const;
    PeluxDeviceManagerEnums::DeviceType deviceType() const;
    QString vendor() const;
    QString product() const;
    QString description() const;
    QString icon() const;
    QStringList emblems() const;
    PeluxDeviceManagerEnums::ConnectionStatus status() const;
    void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status);

private:
    void checkConnectionStatus();

    Solid::Device m_device;
    QString m_id;
    PeluxDeviceManagerEnums::ConnectionStatus m_status{PeluxDeviceManagerEnums::UnknownConnectionStatus};
};
