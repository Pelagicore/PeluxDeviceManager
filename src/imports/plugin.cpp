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

#include <QQmlExtensionPlugin>
#include <QQmlEngine>

#include "../lib/peluxdevicemanager.h"

class PeluxDeviceManagerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
public:
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("PeluxDeviceManager"));
        qmlRegisterUncreatableType<PeluxDevice>(uri, 1, 0, "PeluxDevice", "PeluxDevice is not accessible directly");
        qmlRegisterUncreatableType<PeluxDeviceManagerEnums>(uri, 1, 0, "PeluxDeviceManagerEnums", "PeluxDeviceManagerEnums is not accessible directly");
        qmlRegisterSingletonType<PeluxDeviceManager>(uri, 1, 0, "PeluxDeviceManager", [](QQmlEngine*, QJSEngine*)
                -> QObject* { return new PeluxDeviceManager; });
    }
};

#include "plugin.moc"
