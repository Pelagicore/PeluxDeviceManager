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
#include <QtGlobal>
#include <QTimer>

#include "bluetoothbackend.h"
#include "peluxbtdevice.h"

BluetoothBackend::BluetoothBackend(QObject *parent)
    : QObject(parent)
    , m_btDevice(new QBluetoothLocalDevice)
    , m_btAgent(new QBluetoothServiceDiscoveryAgent(this))
    , m_rfKill(new BluezQt::Rfkill(this))
{
    qInfo() << "Local BT device:" << m_btDevice->name() << m_btDevice->hostMode();
    connect(m_btDevice.data(), &QBluetoothLocalDevice::hostModeStateChanged, this, &BluetoothBackend::onHostModeStateChanged);
    connect(m_btDevice.data(), &QBluetoothLocalDevice::pairingDisplayConfirmation, [this](const QBluetoothAddress &address, QString pin) {
        qInfo() << "!!! Display pairing confirmation from" << address.toString() << pin;
        Q_EMIT pairingConfirmation(address.toString(), pin);
    });
    connect(m_btDevice.data(), &QBluetoothLocalDevice::pairingDisplayPinCode, [this](const QBluetoothAddress &address, QString pin) {
        qInfo() << "!!! Display pairing pin from" << address.toString() << pin;
        Q_EMIT pairingConfirmation(address.toString(), pin);
    });

    connect(m_btAgent, &QBluetoothServiceDiscoveryAgent::serviceDiscovered, this, &BluetoothBackend::onServiceDiscovered);
    connect(m_btAgent, QOverload<QBluetoothServiceDiscoveryAgent::Error>::of(&QBluetoothServiceDiscoveryAgent::error),
            this, &BluetoothBackend::onDiscoveryError);
    connect(m_btAgent, &QBluetoothServiceDiscoveryAgent::finished, [this]() {
        setDiscoveryActive(false);
    });
    connect(m_btAgent, &QBluetoothServiceDiscoveryAgent::canceled, [this]() {
        setDiscoveryActive(false);
    });

    connect(m_rfKill, &BluezQt::Rfkill::stateChanged, [this](BluezQt::Rfkill::State state) {
        if (state != BluezQt::Rfkill::Unblocked) { // rfkill turned off?
            clearDevices();
            setDiscoveryActive(false);
        }
    });

    for(const QBluetoothServiceInfo &service: m_btAgent->discoveredServices()) {
        onServiceDiscovered(service);
    }
}

QVector<PeluxDevice *> BluetoothBackend::allDevices() const
{
    return m_devices;
}

int BluetoothBackend::count() const
{
    return m_devices.count();
}

void BluetoothBackend::startDiscovery()
{
    if (m_rfKill->state() == BluezQt::Rfkill::Unblocked) {
        doStartDiscovery();
    } else if (m_rfKill->unblock()) {
        // wait a bit until the device is really available; even if it's unblocked, it still takes some time
        setDiscoveryActive(true);
        QTimer::singleShot(3000, this, &BluetoothBackend::doStartDiscovery);
    } else {
        qWarning() << Q_FUNC_INFO << "Unusable or hard-blocked Bluetooth adapter! Try powering it on manually first";
    }
}

void BluetoothBackend::doStartDiscovery()
{
    // this may contain stale reference when the rfkill is turned off/on
    // so always grab a new pointer when starting the device discovery
    m_btDevice.reset(new QBluetoothLocalDevice);

    if (m_btDevice->isValid()) {
        if (m_btAgent->isActive()) {
            qWarning() << Q_FUNC_INFO << "Bluetooth discovery already in progress";
            return;
        }

        setDiscoveryActive(true);
        m_btDevice->powerOn();
        m_btDevice->setHostMode(QBluetoothLocalDevice::HostDiscoverable);
        clearDevices();
        m_btAgent->start();
    } else {
        qWarning() << Q_FUNC_INFO << "Local Bluetooth adapter invalid! Power it on manually first";
    }
}

void BluetoothBackend::stopDiscovery()
{
    m_btAgent->stop();
}

bool BluetoothBackend::isDiscoveryActive() const
{
    return m_discoveryActive;
}

void BluetoothBackend::confirmPairing(bool accept)
{
    m_btDevice->pairingConfirmation(accept);
}

void BluetoothBackend::onServiceDiscovered(const QBluetoothServiceInfo &info)
{
    if (!info.isValid() || info.serviceClassUuids().isEmpty() || info.serviceName().isEmpty()) { // discard bogus services
        return;
    }

    qInfo() << "Service discovered:" << info.serviceName() << info.serviceDescription();
    qInfo() << "\ton device:" << info.device().name() << info.device().address().toString();
    qInfo() << "\tservice:" << info.serviceClassUuids().first();

    PeluxBluetoothDevice *dev = new PeluxBluetoothDevice(info, m_btDevice.data(), this);
    m_devices.append(dev);
    Q_EMIT deviceAdded(dev);
}

void BluetoothBackend::onDiscoveryError(QBluetoothServiceDiscoveryAgent::Error error)
{
    qWarning() << Q_FUNC_INFO << error << m_btAgent->errorString();
    setDiscoveryActive(m_btAgent->isActive());
}

void BluetoothBackend::onHostModeStateChanged(QBluetoothLocalDevice::HostMode state)
{
    qInfo() << "BT mode changed:" << state;
    if (state == QBluetoothLocalDevice::HostPoweredOff) {
        clearDevices();
    }
}

void BluetoothBackend::clearDevices()
{
    for (PeluxDevice *dev: m_devices) {
        Q_EMIT deviceRemoved(dev);
    }
    m_devices.clear();
    m_btAgent->clear();
}

void BluetoothBackend::setDiscoveryActive(bool active)
{
    if (active != m_discoveryActive) {
        m_discoveryActive = active;
        Q_EMIT isDiscoveryActiveChanged(m_discoveryActive);
    }
}
