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

#include <QAbstractListModel>

#include "peluxdevice.h"
#include "peluxdevicemanager_global.h"
#include "peluxdevicemanagerenums.h"

class PeluxDeviceManagerPrivate;

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDeviceManager: public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

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
    };
    Q_ENUM(Roles)

    explicit PeluxDeviceManager(QObject *parent = nullptr);
    ~PeluxDeviceManager();
    int count() const;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

    Q_INVOKABLE PeluxDevice* get(int i) const;
    Q_INVOKABLE QVector<PeluxDevice *> allDevices() const;
    Q_INVOKABLE QVector<PeluxDevice *> allDevicesOfType(PeluxDeviceManagerEnums::DeviceType type) const;

Q_SIGNALS:
    void countChanged();

private:
    Q_DISABLE_COPY(PeluxDeviceManager)
    Q_DECLARE_PRIVATE(PeluxDeviceManager)
    PeluxDeviceManagerPrivate *const d_ptr;
};
