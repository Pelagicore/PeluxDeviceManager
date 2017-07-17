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

#include <Solid/Device>
#include <Solid/DeviceNotifier>

#include "solidbackend.h"
#include "peluxsoliddevice.h"

SolidBackend::SolidBackend(QObject *parent)
    : QObject(parent)
{
    initialize();
}

QVector<PeluxDevice *> SolidBackend::allDevices() const
{
    return m_devices;
}

int SolidBackend::count() const
{
    return m_devices.count();
}

void SolidBackend::onDeviceAdded(const QString &udi)
{
    Solid::Device solidDev(udi);
    if (!solidDev.isValid())
        return;

    PeluxSolidDevice * dev = new PeluxSolidDevice(solidDev, this);
    m_devices.append(dev);
    Q_EMIT deviceAdded(dev);
}

void SolidBackend::onDeviceRemoved(const QString &udi)
{
    for (PeluxDevice *dev: m_devices) {
        if (dev->id() == udi) {
            m_devices.removeAll(dev);
            Q_EMIT deviceRemoved(dev);
            break;
        }
    }
}

void SolidBackend::initialize()
{
    QList<Solid::Device> allSolidDevices = Solid::Device::allDevices();
    m_devices.reserve(allSolidDevices.count());

    for (const Solid::Device &solidDevice: qAsConst(allSolidDevices)) {
        if (solidDevice.isValid()) {
            PeluxSolidDevice * dev = new PeluxSolidDevice(solidDevice, this);
            if (dev->deviceType() != PeluxDeviceManagerEnums::UnknownDeviceType) {
                m_devices.append(dev);
            } else {
                delete dev;
            }
        }
    }

    connect(Solid::DeviceNotifier::instance(), &Solid::DeviceNotifier::deviceAdded, this, &SolidBackend::onDeviceAdded);
    connect(Solid::DeviceNotifier::instance(), &Solid::DeviceNotifier::deviceRemoved, this, &SolidBackend::onDeviceRemoved);
}
