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

#include <Solid/DeviceInterface>
#include <Solid/StorageAccess>
#include <Solid/StorageDrive>
#include <Solid/OpticalDisc>
#include <Solid/Camera>
#include <Solid/PortableMediaPlayer>

#include <QDebug>

#include "peluxsoliddevice.h"

PeluxSolidDevice::PeluxSolidDevice(const Solid::Device &solidDevice, QObject *parent)
    : PeluxDevice(parent)
    , m_device(solidDevice)
    , m_id(solidDevice.udi())
{
    checkConnectionStatus();
}

QString PeluxSolidDevice::id() const
{
    return m_id;
}

QString PeluxSolidDevice::parentId() const
{
    return m_device.parentUdi();
}

PeluxDeviceManagerEnums::DeviceType PeluxSolidDevice::deviceType() const
{
    if (m_device.is<Solid::StorageDrive>()) {
        return PeluxDeviceManagerEnums::StorageDrive;
    } else if (m_device.is<Solid::OpticalDisc>()) {
        return PeluxDeviceManagerEnums::OpticalDisc;
    } else if (m_device.is<Solid::StorageAccess>() && m_device.is<Solid::StorageVolume>()) {
        if (!m_device.as<Solid::StorageAccess>()->isIgnored()) { // skip "ignored" partitions, like swap or system reserved
            return PeluxDeviceManagerEnums::StorageDisc;
        }
    } else if (m_device.is<Solid::Camera>()) {
        return PeluxDeviceManagerEnums::Camera;
    } else if (m_device.is<Solid::PortableMediaPlayer>()) {
        return PeluxDeviceManagerEnums::PortableMediaPlayer;
    }

    return PeluxDeviceManagerEnums::UnknownDeviceType;
}

QString PeluxSolidDevice::vendor() const
{
    return m_device.vendor();
}

QString PeluxSolidDevice::product() const
{
    return m_device.product();
}

QString PeluxSolidDevice::description() const
{
    return m_device.description();
}

QString PeluxSolidDevice::icon() const
{
    return m_device.icon();
}

QStringList PeluxSolidDevice::emblems() const
{
    return m_device.emblems();
}

PeluxDeviceManagerEnums::ConnectionStatus PeluxSolidDevice::status() const
{
    return m_status;
}

QString PeluxSolidDevice::mountPoint() const
{
    if (m_device.is<Solid::StorageAccess>()) {
        return m_device.as<Solid::StorageAccess>()->filePath();
    }
    return QString();
}

void PeluxSolidDevice::setStatus(PeluxDeviceManagerEnums::ConnectionStatus status)
{
    if (status != m_status) {
        // TODO connect/disconnect
        Q_EMIT statusChanged(m_status);
    }
}

void PeluxSolidDevice::checkConnectionStatus()
{
    if (m_device.is<Solid::StorageAccess>()) {
        const auto access = m_device.as<Solid::StorageAccess>();
        if (access->isAccessible()) {
            m_status = PeluxDeviceManagerEnums::Connected;
        } else {
            m_status = PeluxDeviceManagerEnums::Disconnected;
        }
    } else {
        qDebug() << "Device" << id() << "doesn't support connection status changes";
    }
}
