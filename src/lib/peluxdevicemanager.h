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

/*!
 * \brief The %PeluxDeviceManager class
 *
 * %PeluxDeviceManager serves as both a model (derived from QAbstractListModel) and an
 * entry point for querying the underlying system for HW information.
 *
 * \par C++ usage example
 *
 * \code{.cpp}
 * PeluxDeviceManager mgr;
 * for (PeluxDevice * dev: mgr.allDevices()) {
 *     qDebug() << "Device:" << dev->id() << dev->description();
 * }
 * qDebug() << "Total number of devices:" << mgr.rowCount();
 * \endcode
 *
 * \par QML usage example (imperative)
 *
 * The %PeluxDeviceManager can be used from QML as a singleton, using the model role names described
 * in PeluxDeviceManager::Roles. For examples, see below:
 *
 * \code{.js}
 * console.info("Number of devices:", PeluxDeviceManager.count)
 * for (var i = 0; i < PeluxDeviceManager.count; i++) {
 *     var device = PeluxDeviceManager.get(i);
 *     console.info("Device: %1 (%2)".arg(device.id).arg(device.description))
 * }
 * \endcode
 *
 * \par QML usage example (declarative)
 *
 * \code{.js}
 * import QtQuick 2.7
 * import PeluxDeviceManager 1.0
 * ...
 * ListView {
 *     model: PeluxDeviceManager
 *     delegate: Label {
 *         text: model.description
 *     }
 * }
 * Connections {
 *      // Demonstrate how to connect to PDM signals
 *      target: PeluxDeviceManager
 *      onDeviceAdded: console.info("Device added:", device.id)
 *      onDeviceRemoved: console.info("Device removed:", device.id)
 * }
 * \endcode
 */
class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDeviceManager: public QAbstractListModel
{
    Q_OBJECT

    /*!
     * \return the number of entries in the model
     */
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    /*!
     * \return whether the BT discovery is currently running
     * \sa startBtDiscovery(), stopBtDiscovery()
     */
    Q_PROPERTY(bool btDiscoveryActive READ isBtDiscoveryActive NOTIFY isBtDiscoveryActiveChanged)

public:
    /*!
     * \brief The QAbstractItemModel data roles
     */
    enum Roles {
        IdRole = Qt::UserRole + 1, /*!< ID of the device (role name: \p id, \c QString) */
        ParentIdRole, /*!< ID of the parent device, if any (role name: \p parentId, \c QString) */
        DeviceTypeRole, /*!< Type of the device (role name: \p deviceType, \c enum PeluxDeviceManagerEnums::DeviceType) */
        VendorRole, /*!< Vendor of the device (role name: \p vendor, \c QString) */
        ProductRole, /*!< Name of the product (role name: \p product, \c QString) */
        DescriptionRole, /*!< User-visible description/name of the device (role name: \p description, \c QString) */
        IconRole, /*!< Name of the icon representing the device (role name: \p icon, \c QString)
                  \sa https://standards.freedesktop.org/icon-naming-spec/icon-naming-spec-latest.html */
        EmblemsRole, /*!< List of emblems to decorate the icon with, to signify the status (role name: \p emblems, \c QStringList) */
        ConnectionStatusRole, /*!< Current status of the device (role name: \p status, \c enum PeluxDeviceManagerEnums::ConnectionStatus) */
        MountpointRole, /*!< Path under which the device is mounted, if any (role name: \p mountPoint, \c QString) */
        DeviceRole, /*!< Low-level device file, or HW address (role name: \p device, \c QString) */
        RemovableRole, /*!< Whether the device is removable (role name: \p removable, \c boolean) */
        DriveTypeRole, /*!< Type of the drive, unknown for other device types (role name: \p driveType,
                       \c enum PeluxDeviceManagerEnums::DriveType) */
        UuidRole, /*!< UUID of a BT device, empty otherwise (role name: \p uuid, \c QString) */
    };
    Q_ENUM(Roles)

    /*! CTOR */
    explicit PeluxDeviceManager(QObject *parent = nullptr);
    /*! DTOR */
    ~PeluxDeviceManager();

    /*!
     * \reimp
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    /*!
     * \reimp
     */
    QHash<int, QByteArray> roleNames() const override;
    /*!
     * \reimp
     */
    QVariant data(const QModelIndex &index, int role) const override;

    /*!
     * \return Pointer to the PeluxDevice at row \p i
     */
    Q_INVOKABLE PeluxDevice* get(int i) const;
    /*!
     * \return all devices in the model
     */
    Q_INVOKABLE QVector<PeluxDevice *> allDevices() const;
    /*!
     * \return all devices in the model of the given \p type
     */
    Q_INVOKABLE QVector<PeluxDevice *> allDevicesOfType(PeluxDeviceManagerEnums::DeviceType type) const;

    /*!
     * Start the discovery of Bluetooth devices.
     *
     * \sa stopBtDiscovery()
     */
    Q_INVOKABLE void startBtDiscovery();
    /*!
     * Stop the discovery of Bluetooth devices.
     *
     * \sa startBtDiscovery()
     */
    Q_INVOKABLE void stopBtDiscovery();
    /*!
     * \return whether the BT discovery is currently running
     * \sa startBtDiscovery(), stopBtDiscovery()
     */
    bool isBtDiscoveryActive() const;
    /*!
      * Confirm the most recent pairing request.
      *
      * To be called by the user to confirm the pairing request
      * \param accept whether to accept or reject the request
      */
    Q_INVOKABLE void confirmPairing(bool accept);

Q_SIGNALS:
    /*!
     * Emitted when a new \p device is found in the system
     */
    void deviceAdded(PeluxDevice * device);
    /*!
     * Emitted when a \p device disappears from the system
     * \warning Always check the \p device for existence first!
     */
    void deviceRemoved(PeluxDevice * device);
    /*!
     * Emitted when the number of items in this model changes
     */
    void countChanged();
    /*!
     * Emitted when the status of BT discovery changes
     */
    void isBtDiscoveryActiveChanged(bool active);

    /*!
      * Emitted as a result of a device requesting pairing with the local BT adapter.
      *
      * \param address BT HW address of the remote device
      * \param pin optional PIN to display (might be empty)
      *
      * To be displayed to the user upon pairing a device
      */
    void pairingConfirmation(const QString &address, const QString &pin);

private:
    Q_DISABLE_COPY(PeluxDeviceManager)
    Q_DECLARE_PRIVATE(PeluxDeviceManager)
    PeluxDeviceManagerPrivate *const d_ptr;
};
