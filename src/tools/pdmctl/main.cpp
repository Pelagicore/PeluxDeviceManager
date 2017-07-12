#include <QCoreApplication>
#include <QDebug>

#include "../../lib/peluxdevicemanager.h"

void displayDevice(PeluxDevice * device)
{
    qWarning() << qUtf8Printable(device->id() + ":");
    qWarning() << qUtf8Printable("\t" + device->description());
    qWarning() << "\tType: " << device->deviceType();
    qWarning() << "\tStatus:" << device->status();
    if (!device->device().isEmpty()) {
        qWarning() << "\tLow level device:" << device->device();
    }
    if (device->status() == PeluxDeviceManagerEnums::Connected) {
        qWarning() << "\tMountpoint:" << device->mountPoint();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QScopedPointer<PeluxDeviceManager> mgr(new PeluxDeviceManager);

    QVector<PeluxDevice *> devices = mgr->allDevices();
    for (PeluxDevice * device: qAsConst(devices)) {
        displayDevice(device);
    }

    qWarning() << "\nTotal number of devices:" << mgr->rowCount();

    QObject::connect(mgr.data(), &PeluxDeviceManager::deviceAdded, &displayDevice);
    QObject::connect(mgr.data(), &PeluxDeviceManager::deviceRemoved, [](PeluxDevice * dev) {
        qWarning() << "Device disconnected:" << dev->id();
    });

    qWarning() << "\n-- Monitoring hotplug devices; Press Ctrl+C to quit --";

    return a.exec();
}
