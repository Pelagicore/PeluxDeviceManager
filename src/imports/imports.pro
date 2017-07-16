TEMPLATE = lib
CONFIG += plugin
QT += quick
TARGET = peluxdevicemanagerplugin

SOURCES = plugin.cpp

pluginfiles.files += $$_PRO_FILE_PWD_/qmldir
pluginfiles.path = $$[QT_INSTALL_PLUGINS]/PeluxDeviceManager
target.path = $$[QT_INSTALL_PLUGINS]/PeluxDeviceManager

INSTALLS += target pluginfiles
