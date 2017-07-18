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

#include <QObject>
#include <QVector>

#include <QBluetoothLocalDevice>
#include <QBluetoothServiceDiscoveryAgent>

#include "peluxbtdevice.h"
#include "rfkill.h"

class BluetoothBackend : public QObject
{
    Q_OBJECT
public:
    explicit BluetoothBackend(QObject *parent = nullptr);
    ~BluetoothBackend() = default;

    QVector<PeluxDevice *> allDevices() const;
    int count() const;

    void startDiscovery();
    void stopDiscovery();

    bool isDiscoveryActive() const;

    void confirmPairing(bool accept);

Q_SIGNALS:
    void deviceAdded(PeluxDevice *dev);
    void deviceRemoved(PeluxDevice *dev);
    void isDiscoveryActiveChanged(bool active);

    void pairingConfirmation(const QString &address, const QString &pin);

private Q_SLOTS:
    void onServiceDiscovered(const QBluetoothServiceInfo &info);
    void onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error);
    void onHostModeStateChanged(QBluetoothLocalDevice::HostMode state);

private:
    void doStartDiscovery();
    void clearDevices();
    void setDiscoveryActive(bool active);
    QVector<PeluxDevice *> m_devices;
    QScopedPointer<QBluetoothLocalDevice> m_btDevice;
    QBluetoothServiceDiscoveryAgent *m_btAgent{nullptr};
    BluezQt::Rfkill *m_rfKill{nullptr};
    bool m_discoveryActive{false};
};
