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

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDevice : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    Q_PROPERTY(int deviceType READ deviceType WRITE setDeviceType NOTIFY deviceTypeChanged) // TODO export proper DeviceType
    Q_PROPERTY(QString vendor READ vendor WRITE setVendor NOTIFY vendorChanged)
    Q_PROPERTY(QString product READ product WRITE setProduct NOTIFY productChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QStringList emblems READ emblems WRITE setEmblems NOTIFY emblemsChanged)

public:
    explicit PeluxDevice(QObject *parent = nullptr);
    ~PeluxDevice() = default;

    virtual QString id() const = 0;
    virtual void setId(const QString &id) = 0;

    virtual QString parentId() const = 0;
    virtual void setParentId(const QString &parentId) = 0;

    virtual int deviceType() const = 0;
    virtual void setDeviceType(int deviceType) = 0;

    virtual QString vendor() const = 0;
    virtual void setVendor(const QString &vendor) = 0;

    virtual QString product() const = 0;
    virtual void setProduct(const QString &product) = 0;

    virtual QString description() const = 0;
    virtual void setDescription(const QString &description) = 0;

    virtual QString icon() const = 0;
    virtual void setIcon(const QString &icon) = 0;

    virtual QStringList emblems() const = 0;
    virtual void setEmblems(const QStringList &emblems) = 0;

Q_SIGNALS:
    void idChanged(const QString &id);
    void parentIdChanged(const QString &parentId);
    void deviceTypeChanged(int deviceType);
    void vendorChanged(const QString &vendor);
    void productChanged(const QString &product);
    void descriptionChanged(const QString &description);
    void iconChanged(const QString &icon);
    void emblemsChanged(const QStringList &emblems);
};
