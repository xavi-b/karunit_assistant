import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Karunit 1.0
import KarunitPlugins 1.0

ColumnLayout {
    anchors.fill: parent

    Flickable {
        Layout.fillHeight: true
        Layout.fillWidth: true

        contentWidth: text.width
        contentHeight: text.height

        TextArea {
            id: text
            readOnly: true
            text: KUPAssistantPluginConnector.configuration
        }
    }

    Button {
        Layout.alignment: Qt.AlignHCenter
        text: KUPAssistantPluginConnector.listening ? qsTr("Cancel") : qsTr("Listen")
        onClicked: KUPAssistantPluginConnector.listening ? KUPAssistantPluginConnector.cancel() : KUPAssistantPluginConnector.listen()
    }
}
