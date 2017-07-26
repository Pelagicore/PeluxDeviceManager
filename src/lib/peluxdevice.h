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

#include "peluxdevicemanager_global.h"
#include "peluxdevicemanagerenums.h"

/*!
 * \brief The PeluxDevice class
 *
 * An (almost) abstract class representing an entry in the PeluxDeviceManager model.
 *
 * \note Not to be instantiated directly, you can obtain instances from the model using
 * PeluxDeviceManager::get()
 */
class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDevice : public QObject
{
    Q_OBJECT

    /*!
     * \copydoc PeluxDeviceManager::IdRole
     */
    Q_PROPERTY(QString id READ id CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::ParentIdRole
     */
    Q_PROPERTY(QString parentId READ parentId CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::DeviceTypeRole
     */
    Q_PROPERTY(PeluxDeviceManagerEnums::DeviceType deviceType READ deviceType CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::VendorRole
     */
    Q_PROPERTY(QString vendor READ vendor CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::ProductRole
     */
    Q_PROPERTY(QString product READ product CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::DescriptionRole
     */
    Q_PROPERTY(QString description READ description CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::IconRole
     */
    Q_PROPERTY(QString icon READ icon CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::EmblemsRole
     */
    Q_PROPERTY(QStringList emblems READ emblems NOTIFY statusChanged)
    /*!
     * \copydoc PeluxDeviceManager::ConnectionStatusRole
     */
    Q_PROPERTY(PeluxDeviceManagerEnums::ConnectionStatus status READ status WRITE setStatus NOTIFY statusChanged)
    /*!
     * \copydoc PeluxDeviceManager::MountpointRole
     */
    Q_PROPERTY(QString mountPoint READ mountPoint NOTIFY statusChanged)
    /*!
     * \copydoc PeluxDeviceManager::DeviceRole
     */
    Q_PROPERTY(QString device READ device CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::RemovableRole
     */
    Q_PROPERTY(bool removable READ isRemovable CONSTANT)
    /*!
     * \copydoc PeluxDeviceManager::DriveTypeRole
     */
    Q_PROPERTY(PeluxDeviceManagerEnums::DriveType driveType READ driveType CONSTANT)

public:
    explicit PeluxDevice(QObject *parent = nullptr);
    ~PeluxDevice() = default;

    virtual QString id() const = 0;
    virtual QString parentId() const = 0;
    virtual PeluxDeviceManagerEnums::DeviceType deviceType() const = 0;
    virtual QString vendor() const = 0;
    virtual QString product() const = 0;
    virtual QString description() const = 0;
    virtual QString icon() const = 0;
    virtual QStringList emblems() const = 0;
    virtual QString mountPoint() const = 0;
    virtual QString device() const = 0;
    virtual bool isRemovable() const = 0;
    virtual PeluxDeviceManagerEnums::DriveType driveType() const = 0;
    virtual QString uuid() const;

    PeluxDeviceManagerEnums::ConnectionStatus status() const;
    virtual void setStatus(PeluxDeviceManagerEnums::ConnectionStatus status) = 0;

    /*!
     * Convenience method to change the status of the device to PeluxDeviceManagerEnums::Connected
     */
    Q_INVOKABLE void connectDevice();
    /*!
     * Convenience method to change the status of the device to PeluxDeviceManagerEnums::Disconnected
     */
    Q_INVOKABLE void disconnectDevice();

protected:
    PeluxDeviceManagerEnums::ConnectionStatus m_status{PeluxDeviceManagerEnums::UnknownConnectionStatus};

Q_SIGNALS:
    /*!
     * Emitted when the \p status of the device has changed
     */
    void statusChanged(PeluxDeviceManagerEnums::ConnectionStatus status);
};
