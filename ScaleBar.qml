/***************************************************************************
  ScaleBar.qml
  --------------------------------------
  Date                 : Nove 2017
  Copyright            : (C) 2017 by Peter Petrik
  Email                : zilolv at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 2.2
import QgisQuick 0.1 as QgsQuick

Item {
    property alias mapSettings: scaleBarKit.mapSettings
    property alias preferredWidth: scaleBarKit.preferredWidth

    QgsQuick.ScaleBarKit {
        id: scaleBarKit
    }

    Rectangle {
        id: rectBar
        width: scaleBarKit.width
        height: parent.height
        color: "black"
        Behavior on width {
          SmoothedAnimation { duration: 200 }
        }
    }

    Label {
        id: label
        text: scaleBarKit.distance + " " + scaleBarKit.units
        color: "white"
        anchors.left: rectBar.left
        anchors.leftMargin: 10
    }

}
