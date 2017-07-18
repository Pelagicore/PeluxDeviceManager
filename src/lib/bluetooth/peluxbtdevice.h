/*
 *   Copyright (C) 2017 Pelagicore AB
 *
 *   Author: Lukáš Tinkl <ltinkl@luxoft.com>
 *
 *   This library is free software; you can redistribute it and/or 
 *   modify it under the terms of the GNU Lesser General Public License
 *   as published by the Free Software Foundation; version 2.1.
 *
 *   This library is distributed in the hope that it will be useful, but
 *   WITHOUT ANY WARRANTY; without even the implied warranty of 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 *   Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public 
 *   License along with this library; if not, write to the Free Software
 *   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
 *   MA 02110-1301 USA
 * 
 *   SPDX-License-Identifier: LGPL-2.1
 *
 */

#pragma once

#include <QPointer>
#include <QBluetoothServiceInfo>
#include <QBluetoothSocket>
#include <QBluetoothLocalDevice>

#include "peluxdevice.h"

class PeluxBluetoothDevice : public PeluxDevice
{
    Q_OBJECT
public:
    explicit PeluxBluetoothDevice(const QBluetoothServiceInfo &btInfo, QBluetoothLocalDevice *btLocalDev, QObject * parent);
    ~PeluxBluetoothDevice() = default;

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
    QString uuid() const override;

    void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status) override;

private:
    void checkConnectionStatus();
    void updateStatus(PeluxDeviceManagerEnums::ConnectionStatus status);
    void connectToSocket();
    void disconnectFromSocket();
    QBluetoothLocalDevice::Pairing pairingStatus() const;
    QBluetoothServiceInfo m_info;
    QString m_id; // store the ID separately as the underlying BtDevice might be gone anytime
    QScopedPointer<QBluetoothSocket> m_socket;
    QPointer<QBluetoothLocalDevice> m_btLocalDevice;
};
