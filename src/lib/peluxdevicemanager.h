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

#include <QAbstractListModel>

#include "peluxdevice.h"
#include "peluxdevicemanager_global.h"
#include "peluxdevicemanagerenums.h"

class PeluxDeviceManagerPrivate;

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDeviceManager: public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_PROPERTY(bool btDiscoveryActive READ isBtDiscoveryActive NOTIFY isBtDiscoveryActiveChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ParentIdRole,
        DeviceTypeRole,
        VendorRole,
        ProductRole,
        DescriptionRole,
        IconRole,
        EmblemsRole,
        ConnectionStatusRole,
        MountpointRole,
        DeviceRole,
        RemovableRole,
        DriveTypeRole,
        UuidRole,
    };
    Q_ENUM(Roles)

    explicit PeluxDeviceManager(QObject *parent = nullptr);
    ~PeluxDeviceManager();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE PeluxDevice* get(int i) const;
    Q_INVOKABLE QVector<PeluxDevice *> allDevices() const;
    Q_INVOKABLE QVector<PeluxDevice *> allDevicesOfType(PeluxDeviceManagerEnums::DeviceType type) const;

    Q_INVOKABLE void startBtDiscovery();
    Q_INVOKABLE void stopBtDiscovery();
    bool isBtDiscoveryActive() const;
    /**
      * To be called by the user to confirm the pairing request
      */
    Q_INVOKABLE void confirmPairing(bool accept);

Q_SIGNALS:
    void deviceAdded(PeluxDevice * device);
    void deviceRemoved(PeluxDevice * device);
    void countChanged();
    void isBtDiscoveryActiveChanged(bool active);

    /**
      * To be displayed to the user upon pairing a device
      */
    void pairingConfirmation(const QString &address, const QString &pin);

private:
    Q_DISABLE_COPY(PeluxDeviceManager)
    Q_DECLARE_PRIVATE(PeluxDeviceManager)
    PeluxDeviceManagerPrivate *const d_ptr;
};
