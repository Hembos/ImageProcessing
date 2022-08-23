import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0


Window {
    id: mainWindow
    title: qsTr("Image processing")

    Component.onCompleted: {
        mainWindow.showMaximized();
    }

    Column {
        Row {
            Button {
                id: chooseLevel

                text: "choose file"

                onClicked: fileDialog.visible = true

                FileDialog {
                    id: fileDialog
                    title: "Please choose a file"
                    folder: shortcuts.home
                    onAccepted: {
                           let path = fileDialog.fileUrl.toString();
                           path = path.replace(/^(file:\/{3})/,"");
                           let cleanPath = decodeURIComponent(path);
                           imageCapture.source = "image://imageProvider//" + cleanPath;
                       }
                }
            }

            Button {
                id: saveButton

                text: "save"

                onClicked: imageProcessing.saveImage();
            }
        }

        Row {
            Button {
                id: zoomIn

                text: "+"

                onClicked: imageChange("Zoom", ["ZoomIn"]);
            }

            Button {
                id: zoomOut

                text: "-"

                onClicked: imageChange("Zoom", ["ZoomOut"]);
            }

            Button {
                id: hand

                text: "hand"

                onClicked: toolChange("Hand");
            }

            Button {
                id: pen

                text: "pen"

                onClicked: toolChange("Pen")
            }

            Column {
                TextField {
                    id: redInput
                    text: "255"
                    placeholderText: "R"
                }

                TextField {
                    id: greenInput

                    placeholderText: "G"
                }

                TextField {
                    id: blueInput

                    placeholderText: "B"
                }

                TextField {
                    id: alphaInput
                    text: "100"
                    placeholderText: "A"
                }
            }

            Button {
                id: smartBrush

                text: "smartBrush"

                onClicked: toolChange("SmartBrush")
            }

            Button {
                id: chanVese

                text: "chanVese"

                onClicked: imageChange("ChanVese", [redInput.text, greenInput.text, blueInput.text, alphaInput.text])
            }

            Button {
                id: borderHighlightFilter

                text: "border"

                onClicked: applyFilter("Border")
            }

            Button {
                id: medianFilter

                text: "median"

                onClicked: applyFilter("Median")
            }

            Button {
                id: gaussFilter

                text: "gauss"

                onClicked: applyFilter("Gauss")
            }

            Button {
                id: meanFilter

                text: "mean"

                onClicked: applyFilter("Mean")
            }
        }

        Image {
            id: imageCapture
            source: ""

            property string curTool

            WheelHandler {
                onWheel: (wheel) => {
                    if (wheel.angleDelta.y > 0)
                        imageChange("Zoom", ["ZoomIn"]);
                    else if (wheel.angleDelta.y < 0)
                        imageChange("Zoom", ["ZoomOut"]);
                 }
            }

            MouseArea {
                id: imageMouseArea

                anchors.fill: parent

                property int clickX: 0
                property int clickY: 0

                onMouseXChanged: {
                    if (parent.curTool == "Hand")
                        imageChange(parent.curTool, ["offsetX", clickX - mouseX]);

                }
                onMouseYChanged: {
                    if (parent.curTool == "Hand")
                        imageChange(parent.curTool, ["offsetY", clickY - mouseY]);
                }

                onPressed: {
                    clickX = mouseX;
                    clickY = mouseY;

                    if (parent.curTool == "Pen" || parent.curTool == "SmartBrush")
                        imageChange(parent.curTool, [clickX, clickY, redInput.text, greenInput.text, blueInput.text, alphaInput.text]);
                }

            }
        }
    }

    function imageChange(tool, params) {
        console.time("Tool working time")
        imageProcessing.process(tool, params);
        console.timeEnd("Tool working time")
        imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
    }

    function toolChange(tool) {
        imageCapture.curTool = tool
    }

    function applyFilter(filter) {
        console.time("Filter applying");
        imageProcessing.applyFilter(filter);
        console.timeEnd("Filter applying");
        imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
    }
}

