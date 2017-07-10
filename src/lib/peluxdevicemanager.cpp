#include "peluxdevicemanager.h"
#include "peluxdevicemanager_p.h"


PeluxDeviceManagerPrivate::PeluxDeviceManagerPrivate(PeluxDeviceManager *qptr)
    : q_ptr(qptr)
{
    Q_Q(PeluxDeviceManager);
    roles = {
        {q->IdRole, "id"},
        {q->ParentIdRole, "parentId"},
        {q->DeviceTypeRole, "deviceType"},
        {q->VendorRole, "vendor"},
        {q->ProductRole, "product"},
        {q->DescriptionRole, "description"},
        {q->IconRole, "icon"},
        {q->EmblemsRole, "emblems"}
    };
}


PeluxDeviceManager::PeluxDeviceManager(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new PeluxDeviceManagerPrivate(this))
{
    connect(this, &QAbstractListModel::rowsInserted, this, &PeluxDeviceManager::countChanged);
    connect(this, &QAbstractListModel::rowsRemoved, this, &PeluxDeviceManager::countChanged);
    connect(this, &QAbstractListModel::modelReset, this, &PeluxDeviceManager::countChanged);
}

PeluxDeviceManager::~PeluxDeviceManager()
{
    delete d_ptr;
}

int PeluxDeviceManager::count() const
{
    return rowCount();
}

QHash<int, QByteArray> PeluxDeviceManager::roleNames() const
{
    Q_D(const PeluxDeviceManager);
    return d->roles;
}

QVariant PeluxDeviceManager::data(const QModelIndex &index, int role) const
{
    // TODO
    return QVariant();
}
