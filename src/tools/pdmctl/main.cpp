#include <QCoreApplication>
#include <QDebug>

#include "../../lib/peluxdevicemanager.h"

void displayDevice(PeluxDevice * device)
{
    qWarning() << qUtf8Printable(device->id() + ":");
    qWarning() << qUtf8Printable("\t" + device->description());
    qWarning() << "\tProduct: " << qUtf8Printable(device->product());
    qWarning() << "\tStatus:" << device->status();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QScopedPointer<PeluxDeviceManager> mgr(new PeluxDeviceManager);

    QVector<PeluxDevice *> devices = mgr->allDevices();
    for (PeluxDevice * device: qAsConst(devices)) {
        displayDevice(device);
    }

    QObject::connect(mgr.data(), &PeluxDeviceManager::deviceAdded, &displayDevice);

    qWarning() << "\n-- Monitoring hotplug devices; Press Ctrl+C to quit --";

    return a.exec();
}
