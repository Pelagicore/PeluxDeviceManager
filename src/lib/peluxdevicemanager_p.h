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

#include <QHash>

#include "peluxdevice.h"
#include "bluetooth/bluetoothbackend.h"

class PeluxDeviceManager;

class PeluxDeviceManagerPrivate
{
public:
    Q_DECLARE_PUBLIC(PeluxDeviceManager)
    PeluxDeviceManager *q_ptr;

    PeluxDeviceManagerPrivate(PeluxDeviceManager *qptr);
    ~PeluxDeviceManagerPrivate();
    void initialize();
    void listenToDevice(PeluxDevice *dev);

    QHash<int, QByteArray> roles;
    QVector<PeluxDevice *> devices;

    BluetoothBackend *btBackend{nullptr};
};
