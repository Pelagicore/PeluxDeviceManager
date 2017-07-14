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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>

#include "../../lib/peluxdevicemanager.h"

void displayDevice(PeluxDevice * device)
{
    qWarning() << qUtf8Printable(device->id() + ":");
    qWarning() << qUtf8Printable("\t" + device->description());
    qWarning() << "\tType: " << device->deviceType();
    qWarning() << "\tStatus:" << device->status();
    if (device->driveType() != PeluxDeviceManagerEnums::UnknownDriveType) {
        qWarning() << "\tDrive type:" << device->driveType();
    }
    if (!device->device().isEmpty()) {
        qWarning() << "\tLow level device:" << device->device();
    }
    if (device->status() == PeluxDeviceManagerEnums::Connected) {
        qWarning() << "\tMountpoint:" << device->mountPoint();
    }
    qWarning() << "\tRemovable:" << device->isRemovable();
}

void listDevices(const QScopedPointer<PeluxDeviceManager> & mgr) {
    QVector<PeluxDevice *> devices = mgr->allDevices();
    for (PeluxDevice * device: qAsConst(devices)) {
        displayDevice(device);
    }
    qWarning() << "\nTotal number of devices:" << mgr->rowCount();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName(a.translate("", "Pelux Device Manager console tool"));
    a.setApplicationVersion(QStringLiteral("1.0"));
    a.setOrganizationName(QStringLiteral("Pelagicore"));

    QCommandLineParser parser;

    parser.addPositionalArgument(QStringLiteral("command"), a.translate("", "Command to perform (list | monitor)"),
                                 QStringLiteral("list | monitor"));

    parser.addHelpOption();
    parser.addVersionOption();
    parser.process(a);

    QScopedPointer<PeluxDeviceManager> mgr(new PeluxDeviceManager);

    const QStringList args = parser.positionalArguments();
    if (args.isEmpty() || args.first() == QStringLiteral("list")) {
        listDevices(mgr);
        return EXIT_SUCCESS;
    } else if (args.first() == QStringLiteral("monitor")) {
        listDevices(mgr);

        QObject::connect(mgr.data(), &PeluxDeviceManager::deviceAdded, &displayDevice);
        QObject::connect(mgr.data(), &PeluxDeviceManager::deviceRemoved, [](PeluxDevice * dev) {
            qWarning() << "Device disconnected:" << dev->id();
        });

        qWarning() << "\n-- Monitoring hotplug devices; Press Ctrl+C to quit --";
        return a.exec();
    }

    parser.showHelp(EXIT_FAILURE);
}
