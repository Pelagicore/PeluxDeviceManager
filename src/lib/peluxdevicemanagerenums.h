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

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDeviceManagerEnums {
    Q_GADGET
public:
    enum DeviceType {
        UnknownDeviceType = 0,
        StorageDrive,
        StorageDisc,
        OpticalDisc,
        Camera,
        PortableMediaPlayer,
        Bluetooth,
        Last = 0xffff
    };
    Q_ENUM(DeviceType)

    enum ConnectionStatus {
        UnknownConnectionStatus = 0,
        Disconnecting,
        Disconnected,
        Connecting,
        Connected,
    };
    Q_ENUM(ConnectionStatus)

    enum ConnectionType {
        UnknownConnectionType = 0,
        Ide,
        Usb,
        Firewire,
        Scsi,
        Sata,
        Platform,
        Wireless,
    };
    Q_ENUM(ConnectionType)

    enum DriveType {
        UnknownDriveType = 0,
        HardDisk,
        CdromDrive,
        Floppy,
        Tape,
        CompactFlash,
        MemoryStick,
        SmartMedia,
        SdMmc,
        Xd,
    };
    Q_ENUM(DriveType)
};
