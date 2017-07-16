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
#include <QTranslator>
#include <QLibraryInfo>

#include "../../lib/peluxdevicemanager.h"

void displayDevice(PeluxDevice * device)
{
    qInfo().noquote() << device->id();
    qInfo().nospace().noquote() << "\t" << qApp->translate("", "Description:") << " " << device->description();
    qInfo().nospace().noquote() << "\t" << qApp->translate("", "Type:") << " " << device->deviceType();
    qInfo().nospace().noquote() << "\t" << qApp->translate("", "Status:") << " " << device->status();
    if (device->driveType() != PeluxDeviceManagerEnums::UnknownDriveType) {
        qInfo().nospace().noquote() << "\t" << qApp->translate("", "Drive type:") << " " << device->driveType();
    }
    if (!device->device().isEmpty()) {
        qInfo().nospace().noquote() << "\t" << qApp->translate("", "Low level device:") << " " << device->device();
    }
    if (device->status() == PeluxDeviceManagerEnums::Connected) {
        qInfo().nospace().noquote() << "\t" << qApp->translate("", "Mountpoint:") << " " << device->mountPoint();
    }
    qInfo().nospace().noquote() << "\t" << qApp->translate("", "Removable:") << " " << device->isRemovable();
}

void listDevices(const QScopedPointer<PeluxDeviceManager> & mgr) {
    for (PeluxDevice * device: mgr->allDevices()) {
        displayDevice(device);
    }
    qInfo().nospace().noquote() << "\n" << qApp->translate("", "Total number of devices:") << " " << mgr->rowCount();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationVersion(VERSION_NUMBER);
    a.setOrganizationName(QStringLiteral("Pelagicore"));

    QTranslator qtTranslator;
    qtTranslator.load(QLocale::system(), QStringLiteral("qt_"), QString(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    a.installTranslator(&qtTranslator);

    QTranslator appTrans;
    appTrans.load(a.applicationName());
    a.installTranslator(&appTrans);

    QCommandLineParser parser;
    parser.setApplicationDescription(qApp->translate("", "Pelux Device Manager console tool"));

    parser.addPositionalArgument(QStringLiteral("command"), qApp->translate("", "Command to perform (list | monitor)"),
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
            qInfo().nospace().noquote() << qApp->translate("", "Device disconnected:") << " " << dev->id();
        });

        qInfo().nospace().noquote() << "\n-- " << qApp->translate("", "Monitoring hotplug devices; Press Ctrl+C to quit") << " --";
        return a.exec();
    }

    parser.showHelp(EXIT_FAILURE);
}
