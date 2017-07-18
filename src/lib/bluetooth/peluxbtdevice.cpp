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

#include <QDebug>
#include <QBluetoothDeviceInfo>

#include "peluxbtdevice.h"

PeluxBluetoothDevice::PeluxBluetoothDevice(const QBluetoothServiceInfo &btInfo, QBluetoothLocalDevice *btLocalDev, QObject *parent)
    : PeluxDevice(parent)
    , m_info(btInfo)
    , m_btLocalDevice(btLocalDev)
{
    m_id = m_info.device().address().toString() + QStringLiteral("_") + m_info.serviceClassUuids().first().toString();

    connect(m_btLocalDevice.data(), &QBluetoothLocalDevice::error, [this](QBluetoothLocalDevice::Error error) {
        qWarning() << "Pairing error" << device() << error;
        updateStatus(PeluxDeviceManagerEnums::Unpaired);
    });

    updateStatus(PeluxDeviceManagerEnums::Disconnected);
    checkConnectionStatus();
}

QString PeluxBluetoothDevice::id() const
{
    return m_id;
}

QString PeluxBluetoothDevice::parentId() const
{
    return QString();
}

PeluxDeviceManagerEnums::DeviceType PeluxBluetoothDevice::deviceType() const
{
    return PeluxDeviceManagerEnums::Bluetooth;
}

QString PeluxBluetoothDevice::vendor() const
{
    return m_info.serviceProvider();
}

QString PeluxBluetoothDevice::product() const
{
    return m_info.device().name();
}

QString PeluxBluetoothDevice::description() const
{
    QString text = m_info.serviceName();
    if (!m_info.serviceDescription().isEmpty()) {
        text.append(QStringLiteral(" (") + m_info.serviceDescription() + QStringLiteral(")"));
    }
    return text;
}

QString PeluxBluetoothDevice::icon() const
{
    // FIXME maybe use QBluetoothDeviceInfo::majorDeviceClass() to differentiate
    return QStringLiteral("bluetooth");
}

QStringList PeluxBluetoothDevice::emblems() const
{
    // FIXME ditto as icon()
    return {};
}

QString PeluxBluetoothDevice::mountPoint() const
{
    // TODO
    return QString();
}

QString PeluxBluetoothDevice::device() const
{
    return m_info.device().address().toString();
}

bool PeluxBluetoothDevice::isRemovable() const
{
    return true;
}

PeluxDeviceManagerEnums::DriveType PeluxBluetoothDevice::driveType() const
{
    return PeluxDeviceManagerEnums::UnknownDriveType;
}

QString PeluxBluetoothDevice::uuid() const
{
    return !m_info.serviceClassUuids().isEmpty() ? m_info.serviceClassUuids().first().toString() : QString();
}

void PeluxBluetoothDevice::setStatus(PeluxDeviceManagerEnums::ConnectionStatus status)
{
    if (m_btLocalDevice.isNull()) { // BT link gone
        updateStatus(PeluxDeviceManagerEnums::Disconnected);
        return;
    }

    if (status == PeluxDeviceManagerEnums::Connected) { // connect + pair (optional)
        if (pairingStatus() == QBluetoothLocalDevice::Unpaired) { // let's pair first
            updateStatus(PeluxDeviceManagerEnums::Connecting);
            connect(m_btLocalDevice.data(), &QBluetoothLocalDevice::pairingFinished,
                    [this](const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {
                qInfo() << "!! Pairing finished with:" << address.toString() << id();
                if (address == m_info.device().address()) {
                    if (pairing == QBluetoothLocalDevice::Paired || pairing == QBluetoothLocalDevice::AuthorizedPaired) {
                        connectToSocket();
                    }
                }
            });
            qInfo() << "!!! Request pairing with:" << m_info.device().address();
            m_btLocalDevice->requestPairing(m_info.device().address(), QBluetoothLocalDevice::AuthorizedPaired);
        } else { // already paired, let's just connect to the socket
            connectToSocket();
        }
    } else if (status == PeluxDeviceManagerEnums::Disconnected) { // disconnect
        updateStatus(PeluxDeviceManagerEnums::Disconnecting);
        disconnectFromSocket();
    } else if (status == PeluxDeviceManagerEnums::Unpaired) { // unpair
        if (pairingStatus() > QBluetoothLocalDevice::Unpaired) {
            connect(m_btLocalDevice.data(), &QBluetoothLocalDevice::pairingFinished,
                    [this](const QBluetoothAddress &address, QBluetoothLocalDevice::Pairing pairing) {
                if (address == m_info.device().address() && pairing == QBluetoothLocalDevice::Unpaired) {
                    updateStatus(PeluxDeviceManagerEnums::Unpaired);
                }
            });
            m_btLocalDevice->requestPairing(m_info.device().address(), QBluetoothLocalDevice::Unpaired);
        }
    }
}

void PeluxBluetoothDevice::connectToSocket()
{
    if (!m_info.isValid()) {
        qWarning() << Q_FUNC_INFO << "!!! Invalid BT service info";
        return;
    }
    updateStatus(PeluxDeviceManagerEnums::Connecting);

    m_socket.reset(new QBluetoothSocket(m_info.socketProtocol(), this));
    connect(m_socket.data(), &QBluetoothSocket::connected, [this]() {
        qInfo() << "!!! Service connected:" << id();
        updateStatus(PeluxDeviceManagerEnums::Connected);
    });
    connect(m_socket.data(), &QBluetoothSocket::disconnected, [this]() {
        qInfo() << "!!! Service disconnected:" << id();
        updateStatus(PeluxDeviceManagerEnums::Disconnected);
    });
    connect(m_socket.data(), QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            [this](QBluetoothSocket::SocketError error) {
        qWarning() << "Socket error:" << error << m_socket->errorString();
    });
    m_socket->connectToService(m_info.device().address(), m_info.serviceClassUuids().first());
}

void PeluxBluetoothDevice::disconnectFromSocket()
{
    if (!m_socket) {
        qWarning() << Q_FUNC_INFO << "!!! Invalid socket info";
        return;
    }

    m_socket->disconnectFromService();
    //m_socket->close();
    disconnect(m_socket.data());
}

void PeluxBluetoothDevice::checkConnectionStatus()
{
    switch (pairingStatus()) {
    case QBluetoothLocalDevice::Unpaired:
        updateStatus(PeluxDeviceManagerEnums::Unpaired);
        break;
    case QBluetoothLocalDevice::Paired:
        updateStatus(PeluxDeviceManagerEnums::Paired);
        break;
    case QBluetoothLocalDevice::AuthorizedPaired:
        updateStatus(PeluxDeviceManagerEnums::PairedAuthorized);
        break;
    default:
        break;
    }
}

void PeluxBluetoothDevice::updateStatus(PeluxDeviceManagerEnums::ConnectionStatus status)
{
    if (m_status != status) {
        m_status = status;
        Q_EMIT statusChanged(m_status);
    }
}

QBluetoothLocalDevice::Pairing PeluxBluetoothDevice::pairingStatus() const
{
    if (m_btLocalDevice.isNull()) {
        return QBluetoothLocalDevice::Unpaired;
    }

    return m_btLocalDevice->pairingStatus(m_info.device().address());
}
