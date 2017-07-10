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

#include "peluxdevicemanager.h"
#include "peluxdevicemanager_p.h"

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
        {q->EmblemsRole, "emblems"}
    };
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

int PeluxDeviceManager::count() const
{
    return rowCount();
}

QHash<int, QByteArray> PeluxDeviceManager::roleNames() const
{
    Q_D(const PeluxDeviceManager);
    return d->roles;
}

QVariant PeluxDeviceManager::data(const QModelIndex &index, int role) const
{
    // TODO
    return QVariant();
}
