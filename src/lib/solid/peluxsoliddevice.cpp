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
#include <Solid/Block>

#include <QDebug>

#include "peluxsoliddevice.h"

PeluxSolidDevice::PeluxSolidDevice(const Solid::Device &solidDevice, QObject *parent)
    : PeluxDevice(parent)
    , m_device(solidDevice)
    , m_id(solidDevice.udi())
{
    if (m_device.is<Solid::StorageAccess>()) {
        connect(m_device.as<Solid::StorageAccess>(), &Solid::StorageAccess::accessibilityChanged, [this](bool accessible, const QString &udi) {
            if (udi == id()) {
                if (accessible) {
                    updateStatus(PeluxDeviceManagerEnums::Connected);
                } else {
                    updateStatus(PeluxDeviceManagerEnums::Disconnected);
                }
            }
        });
        connect(m_device.as<Solid::StorageAccess>(), &Solid::StorageAccess::setupDone, this, &PeluxSolidDevice::onStorageResult);
        connect(m_device.as<Solid::StorageAccess>(), &Solid::StorageAccess::teardownDone, this, &PeluxSolidDevice::onStorageResult);
    }
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
        if (!m_device.as<Solid::StorageAccess>()->isIgnored()
                && !m_device.as<Solid::StorageVolume>()->isIgnored()) { // skip "ignored" partitions, like swap or system reserved
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

QString PeluxSolidDevice::mountPoint() const
{
    if (m_device.is<Solid::StorageAccess>()) {
        return m_device.as<Solid::StorageAccess>()->filePath();
    }
    return QString();
}

QString PeluxSolidDevice::device() const
{
    if (m_device.is<Solid::Block>()) {
        return m_device.as<Solid::Block>()->device();
    }
    return QString();
}

bool PeluxSolidDevice::isRemovable() const
{
    if (m_device.is<Solid::StorageDrive>()) {
        return m_device.as<Solid::StorageDrive>()->isRemovable();
    } else if (m_device.is<Solid::OpticalDisc>()) {
        return true;
    } else if (m_device.is<Solid::StorageAccess>()) {
        Solid::Device drive(m_device.parentUdi());
        if (drive.is<Solid::StorageDrive>()) {
            return drive.as<Solid::StorageDrive>()->isRemovable();
        }
    }

    return false;
}

void PeluxSolidDevice::setStatus(PeluxDeviceManagerEnums::ConnectionStatus status)
{
    if (status != m_status && m_device.is<Solid::StorageAccess>()) {
        if (status == PeluxDeviceManagerEnums::Connected) {
            m_device.as<Solid::StorageAccess>()->setup();
            updateStatus(PeluxDeviceManagerEnums::Connecting);
        } else if (status == PeluxDeviceManagerEnums::Disconnected) {
            m_device.as<Solid::StorageAccess>()->teardown();
            updateStatus(PeluxDeviceManagerEnums::Disconnecting);
        }
    }
}

void PeluxSolidDevice::onStorageResult(Solid::ErrorType error, const QVariant &errorData)
{
    qWarning() << Q_FUNC_INFO << "Mount/unmount operation ended with status:" << error << errorData.toString();
}

void PeluxSolidDevice::checkConnectionStatus()
{
    if (m_device.is<Solid::StorageAccess>()) {
        if (m_device.as<Solid::StorageAccess>()->isAccessible()) {
            updateStatus(PeluxDeviceManagerEnums::Connected);
        } else {
            updateStatus(PeluxDeviceManagerEnums::Disconnected);
        }
    } else {
        qDebug() << "Device" << id() << "doesn't support connection status changes";
    }
}

void PeluxSolidDevice::updateStatus(PeluxDeviceManagerEnums::ConnectionStatus status)
{
    if (status != m_status) {
        m_status = status;
        Q_EMIT statusChanged(m_status);
    }
}
