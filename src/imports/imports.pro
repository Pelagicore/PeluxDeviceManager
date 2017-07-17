TEMPLATE = lib
CONFIG += plugin
QT += quick
TARGET = peluxdevicemanagerplugin

SOURCES = plugin.cpp

LIBS += -lPeluxDeviceManager

pluginfiles.files += $$_PRO_FILE_PWD_/qmldir
pluginfiles.path = $$[QT_INSTALL_QML]/PeluxDeviceManager
target.path = $$[QT_INSTALL_QML]/PeluxDeviceManager

INSTALLS += target pluginfiles
