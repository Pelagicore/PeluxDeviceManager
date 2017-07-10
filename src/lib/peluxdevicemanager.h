#pragma once

#include <QAbstractListModel>

#include "peluxdevicemanager_global.h"

class PeluxDeviceManagerPrivate;

class PELUXDEVICEMANAGERSHARED_EXPORT PeluxDeviceManager: public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        ParentIdRole,
        DeviceTypeRole,
        VendorRole,
        ProductRole,
        DescriptionRole,
        IconRole,
        EmblemsRole,

    };
    Q_ENUM(Roles)

    explicit PeluxDeviceManager(QObject *parent = nullptr);
    ~PeluxDeviceManager();
    int count() const;

    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role) const override;

Q_SIGNALS:
    void countChanged();

private:
    Q_DISABLE_COPY(PeluxDeviceManager)
    Q_DECLARE_PRIVATE(PeluxDeviceManager)
    PeluxDeviceManagerPrivate *const d_ptr;
};
