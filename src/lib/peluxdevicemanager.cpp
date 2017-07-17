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

#include "peluxdevicemanager.h"
#include "peluxdevicemanager_p.h"
#include "solid/solidbackend.h"

PeluxDeviceManagerPrivate::PeluxDeviceManagerPrivate(PeluxDeviceManager *qptr)
    : q_ptr(qptr)
{
    Q_Q(PeluxDeviceManager);
    roles = {
        {q->IdRole, "id"},
        {q->ParentIdRole, "parentId"},
        {q->DeviceTypeRole, "deviceType"},
        {q->VendorRole, "vendor"},
        {q->ProductRole, "product"},
        {q->DescriptionRole, "description"},
        {q->IconRole, "icon"},
        {q->EmblemsRole, "emblems"},
        {q->ConnectionStatusRole, "status"},
        {q->MountpointRole, "mountPoint"},
        {q->DeviceRole, "device"},
        {q->RemovableRole, "removable"}
    };

    qRegisterMetaType<PeluxDeviceManagerEnums::DeviceType>("DeviceType");
    qRegisterMetaType<PeluxDeviceManagerEnums::ConnectionType>("ConnectionType");
    qRegisterMetaType<PeluxDeviceManagerEnums::DriveType>("DriveType");
    qRegisterMetaType<PeluxDeviceManagerEnums::ConnectionStatus>("ConnectionStatus");

    initialize();
}

PeluxDeviceManagerPrivate::~PeluxDeviceManagerPrivate()
{
    qDeleteAll(devices);
    devices.clear();
}

void PeluxDeviceManagerPrivate::initialize()
{
    Q_Q(PeluxDeviceManager);
    q->beginResetModel();

    SolidBackend * solidBackend = new SolidBackend(q);
    devices.reserve(solidBackend->count());
    devices.append(solidBackend->allDevices());
    QObject::connect(solidBackend, &SolidBackend::deviceAdded, [this, q](PeluxDevice * dev) {
        q->beginInsertRows(QModelIndex(), q->rowCount(), q->rowCount());
        devices.append(dev);
        Q_EMIT q->deviceAdded(dev);
        q->endInsertRows();
    });
    QObject::connect(solidBackend, &SolidBackend::deviceRemoved, [this, q](PeluxDevice * dev) {
        const int i = devices.indexOf(dev);
        q->beginRemoveRows(QModelIndex(), i, i);
        Q_EMIT q->deviceRemoved(dev);
        devices.remove(i);
        q->endRemoveRows();
    });

    // TODO query other backends, fill data

    q->endResetModel();
}


PeluxDeviceManager::PeluxDeviceManager(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PeluxDeviceManagerPrivate(this))
{
    connect(this, &QAbstractListModel::rowsInserted, this, &PeluxDeviceManager::countChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &PeluxDeviceManager::countChanged);
    connect(this, &QAbstractListModel::modelReset, this, &PeluxDeviceManager::countChanged);
}

PeluxDeviceManager::~PeluxDeviceManager()
{
    delete d_ptr;
}

int PeluxDeviceManager::rowCount(const QModelIndex &) const
{
    Q_D(const PeluxDeviceManager);
    return d->devices.count();
}

QHash<int, QByteArray> PeluxDeviceManager::roleNames() const
{
    Q_D(const PeluxDeviceManager);
    return d->roles;
}

QVariant PeluxDeviceManager::data(const QModelIndex &index, int role) const
{
    Q_D(const PeluxDeviceManager);

    if (!index.isValid()) {
        return QVariant();
    }

    const int row = index.row();

    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    const auto item = d->devices.at(row);
    if (!item) {
        return QVariant();
    }

    switch (role) {
    case IdRole: return item->id();
    case ParentIdRole: return item->parentId();
    case DeviceTypeRole: return item->deviceType();
    case ProductRole: return item->product();
    case VendorRole: return item->vendor();
    case DescriptionRole: return item->description();
    case IconRole: return item->icon();
    case EmblemsRole: return item->emblems();
    case ConnectionStatusRole: return item->status();
    case MountpointRole: return item->mountPoint();
    case DeviceRole: return item->device();
    case RemovableRole: return item->isRemovable();
    }

    return QVariant();
}

PeluxDevice* PeluxDeviceManager::get(int i) const
{
    Q_D(const PeluxDeviceManager);

    if (i < 0 || i >= rowCount()) {
        return nullptr;
    }

    return d->devices.at(i);
}

QVector<PeluxDevice *> PeluxDeviceManager::allDevices() const
{
    Q_D(const PeluxDeviceManager);

    return d->devices;
}

QVector<PeluxDevice *> PeluxDeviceManager::allDevicesOfType(PeluxDeviceManagerEnums::DeviceType type) const
{
    Q_D(const PeluxDeviceManager);

    QVector<PeluxDevice *> result;
    result.reserve(d->devices.count());

    for (PeluxDevice *device: qAsConst(d->devices)) {
        if (device->deviceType() == type) {
            result.append(device);
        }
    }

    return result;
}
