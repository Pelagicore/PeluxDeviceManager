import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import PeluxDeviceManager 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "PDM Demo (%1 devices)".arg(PeluxDeviceManager.count)

    ListView {
        id: view
        anchors.fill: parent
        model: PeluxDeviceManager

        delegate: ItemDelegate {
            width: ListView.view.width
            contentItem: ColumnLayout {
                Label {
                    text: model.description
                    font.pointSize: 12
                }
                Label {
                    text: model.device + (model.status == PeluxDeviceManagerEnums.Connected ?
                                              " (Connected at %1)".arg(model.mountPoint) : "")
                    font.pointSize: 10
                    font.italic: model.status != PeluxDeviceManagerEnums.Connected
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

    Connections {
        // Demonstrate how to connect to PDM signals
        target: PeluxDeviceManager
        onDeviceAdded: console.info("Device added:", device.id)
        onDeviceRemoved: console.info("Device removed:", device.id)
    }
}
