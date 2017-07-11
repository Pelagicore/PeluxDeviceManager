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

#include <QVector>

#include "peluxsoliddevice.h"

class SolidBackend: public QObject
{
    Q_OBJECT
public:
    explicit SolidBackend(QObject * parent);
    ~SolidBackend() = default;
    QVector<PeluxDevice *> allDevices() const;
    int count() const;

Q_SIGNALS:
    void deviceAdded(PeluxDevice *dev);
    void deviceRemoved(PeluxDevice *dev);

private Q_SLOTS:
    void onDeviceAdded(const QString &udi);
    void onDeviceRemoved(const QString &udi);

private:
    void initialize();
    QVector<PeluxDevice *> m_devices;
};
