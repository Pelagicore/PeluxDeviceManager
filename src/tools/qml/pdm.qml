import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import PeluxDeviceManager 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "PDM Demo (%1 devices)".arg(PeluxDeviceManager.count)

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: view
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: PeluxDeviceManager
            clip: true

            delegate: ItemDelegate {
                width: ListView.view.width
                contentItem: ColumnLayout {
                    Label {
                        text: model.description + (model.status == PeluxDeviceManagerEnums.Connected ? " (connected)" : "")
                        font.pointSize: 12
                        font.bold: model.status == PeluxDeviceManagerEnums.Connected
                    }
                    Label {
                        text: model.device
                        font.pointSize: 10
                        font.italic: model.status != PeluxDeviceManagerEnums.Connected
                    }
                }
                onClicked: {
                    console.info("Clicked device:", model.product);
                    console.info("\tservice description:", model.description);
                    console.info("\tstatus:", model.status);
                    if (model.status == PeluxDeviceManagerEnums.Connected) {
                        console.info("Disconnecting from:", model.id)
                        PeluxDeviceManager.get(index).disconnectDevice();
                    } else {
                        console.info("Connecting to:", model.id)
                        PeluxDeviceManager.get(index).connectDevice();
                    }
                }
            }

            Component.onCompleted: {
                // Demonstrate the non-declarative approach to get device info
                console.info("Number of devices:", PeluxDeviceManager.count)
                for (var i = 0; i < PeluxDeviceManager.count; i++) {
                    var device = PeluxDeviceManager.get(i);
                    console.info("Device: %1 (%2)".arg(device.id).arg(device.description))
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.margins: 10
            Layout.alignment: Qt.AlignHCenter
            Button {
                text: "Start BT discovery"
                enabled: !PeluxDeviceManager.btDiscoveryActive
                onClicked: PeluxDeviceManager.startBtDiscovery();
            }
            Button {
                text: "Stop BT discovery"
                enabled: PeluxDeviceManager.btDiscoveryActive
                onClicked: PeluxDeviceManager.stopBtDiscovery();
            }
        }
    }

    onClosing: {
        PeluxDeviceManager.stopBtDiscovery();
    }

    Connections {
        // Demonstrate how to connect to PDM signals
        target: PeluxDeviceManager
        onDeviceAdded: console.info("Device added:", device.id)
        onDeviceRemoved: console.info("Device removed:", device.id)
        onPairingConfirmation: console.info("!!! Pairing request for:", address, "with pin:", pin)
    }
}
