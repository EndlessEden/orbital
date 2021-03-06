/*
 * Copyright 2013 Giulio Camuffo <giuliocamuffo@gmail.com>
 *
 * This file is part of Orbital
 *
 * Orbital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Orbital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Orbital.  If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.1
import Orbital 1.0
import QtQuick.Window 2.1

Item {
    id: elementsChooser
    property QtObject screen: null

    Column {
        anchors.fill: parent
        anchors.margins: 2
        spacing: 2
        Row {
            id: elementsList
            height: 30
            spacing: 2

            ListModel {
                id: elementsModel

                property var elements: Client.elementsInfo
                onElementsChanged: {
                    clear();
                    for (var i = 0; i < elements.length; ++i) {
                        if (elements[i].type == ElementInfo.Item) {
                            append(elements[i]);
                        }
                    }
                }
            }

            Repeater {
                model: elementsModel

                Button {
                    width: Math.min(100, elementsChooser.width / elementsModel.count - elementsList.spacing)
                    height: elementsList.height
                    caption: prettyName

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            if (screen) {
                                var newElem = Ui.createElement(name, screen);
                                newElem.publish();
                            }
                        }
                    }
                }
            }
        }
        Row {
            id: stylesList
            height: 20
            spacing: 2

            Text {
                height: stylesList.height
                text: qsTr("Style:")
                verticalAlignment: Qt.AlignVCenter
            }

            Repeater {
                model: Client.stylesInfo

                Button {
                    width: 100
                    height: stylesList.height
                    caption: modelData.prettyName

                    onClicked: {
                        Ui.styleName = modelData.name
                    }
                }
            }
        }
    }

}