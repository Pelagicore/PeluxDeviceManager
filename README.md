# PeluxDeviceManager
Pelux Suite Device Manager

This library provides a unified API to query and interact with all kinds of hardware devices. 

Current backends:
- Solid
- Bluetooth

## Dependencies
- Solid (https://api.kde.org/frameworks/solid/html/index.html)
  - build deps: flex/bison, udev, extra-cmake-modules
  - manual build: usually just a matter of `sudo apt build-dep solid` for Ubuntu or `sudo dnf builddep kf5-solid` for Fedora
  - runtime deps: udisks2 (dbus), MediaPlayerInfo (optional)
- Qt Bluetooth (http://doc.qt.io/qt-5/qtbluetooth-index.html)
  - runtime deps: dbus, bluez

## License
Copyright (C) 2017 Pelagicore AB

The source code included here is licensed under the LGPL 2.1. Please
see the "LICENSE" file for more information. Text documents are
licensed under creative commons 4.0

SPDX-License-Identifier: CC-BY-4.0
