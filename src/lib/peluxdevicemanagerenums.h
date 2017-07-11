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
        Bluetooth,
        Wireless,
    };
    Q_ENUM(ConnectionType)

    enum DriveType {
        HardDisk = 0,
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
