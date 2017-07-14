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
#include <Solid/SolidNamespace>

#include "peluxdevice.h"

class PeluxSolidDevice : public PeluxDevice
{
    Q_OBJECT
public:
    explicit PeluxSolidDevice(const Solid::Device &solidDevice, QObject * parent);
    ~PeluxSolidDevice() = default;

    QString id() const override;
    QString parentId() const override;
    PeluxDeviceManagerEnums::DeviceType deviceType() const override;
    QString vendor() const override;
    QString product() const override;
    QString description() const override;
    QString icon() const override;
    QStringList emblems() const override;
    QString mountPoint() const override;
    QString device() const override;
    bool isRemovable() const override;
    PeluxDeviceManagerEnums::DriveType driveType() const override;

    void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status) override;

private Q_SLOTS:
    void onStorageResult(Solid::ErrorType error, const QVariant &errorData);

private:
    void checkConnectionStatus();
    void updateStatus(PeluxDeviceManagerEnums::ConnectionStatus status);

    Solid::Device m_device;
    QString m_id; // store the ID separately as the underlying Solid::Device might be gone anytime
};
