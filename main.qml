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
                           if (showFilteredImage.checked)
                               filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                    }
                    nameFilters: ["Image Files (*.jpg *.png)"]
                }
            }

            Button {
                id: saveButton

                text: "save"

                onClicked: imageProcessing.saveImage(tagPath.text);
            }

            TextField {
                id: tagPath
                placeholderText: "tag"
                text: ""
            }
        }

        Row {
            Button {
                id: zoomIn

                text: "+"

                onClicked: {
                    imageProcessing.zooming(["ZoomIn"]);
                    imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                    if (showFilteredImage.checked)
                        filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                }
            }

            Button {
                id: zoomOut

                text: "-"

                onClicked: {
                    imageProcessing.zooming(["ZoomOut"]);
                    imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                    if (showFilteredImage.checked)
                        filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                }
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

            Column {
                Button {
                    id: smartBrush

                    text: "smartBrush"

                    onClicked: toolChange("SmartBrush")
                }

                SpinBox {
                    id: smartBrushSens
                    stepSize: 0.01
                    minimumValue: 0
                    maximumValue: 1
                    decimals: 2
                    value: 0.5
                }
            }

            Column {
                Button {
                    id: chanVese

                    text: "chanVese"

                    onClicked: {
                        let mode = chanVeseMode.checked ? "ChanVeseAnchors" : "ChanVese";
                        imageChange(mode, [redInput.text, greenInput.text, blueInput.text, alphaInput.text, chanVeseSens.value])
                    }
                }

                SpinBox {
                    id: chanVeseSens
                    stepSize: 0.01
                    minimumValue: 0
                    maximumValue: 1
                    decimals: 2
                    value: 0.5
                }

                CheckBox {
                    id: chanVeseMode
                    text: "anchorsMode"
                    checked: false
                }
            }

            Column {
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

            Button {
                id: removeRegionButton

                text: "removeRegion"

                onClicked: toolChange("RemoveRegion")
            }

            CheckBox {
                id: showFilteredImage
                text: "showFilters"
                checked: false

                onCheckedStateChanged: {
                    switch (checkedState) {
                        case Qt.Unchecked:
                            filteredImage.source = "";
                            break;
                        case Qt.Checked:
                            filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        Row {
            spacing: 20

            Image {
                id: imageCapture
                source: ""
    
                property string curTool
    
                WheelHandler {
                    onWheel: (wheel) => {
                        if (wheel.angleDelta.y > 0) {
                            imageProcessing.zooming(["ZoomIn"]);
                            imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                            if (showFilteredImage.checked)
                                filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                        }
                        else if (wheel.angleDelta.y < 0){
                            imageProcessing.zooming(["ZoomOut"]);
                            imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                            if (showFilteredImage.checked)
                                filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                        }
                     }
                }
    
                MouseArea {
                    id: imageMouseArea
    
                    anchors.fill: parent
    
                    property int clickX: 0
                    property int clickY: 0
    
                    onMouseXChanged: {
                        if (parent.curTool == "Hand") {
                            imageProcessing.shift(["offsetX", clickX - mouseX]);
                            imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                            if (showFilteredImage.checked)
                                filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                        }
    
                    }
                    onMouseYChanged: {
                        if (parent.curTool == "Hand") {
                            imageProcessing.shift(["offsetY", clickY - mouseY]);
                            imageCapture.source = "image://imageProvider/imageChanged?" + Math.random();
                            if (showFilteredImage.checked)
                                filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
                        }
                    }
    
                    onPressed: {
                        clickX = mouseX;
                        clickY = mouseY;
    
                        if (parent.curTool == "Pen")
                            imageChange(parent.curTool, [clickX, clickY, redInput.text, greenInput.text, blueInput.text, alphaInput.text]);
                        if (parent.curTool == "SmartBrush")
                            imageChange(parent.curTool, [clickX, clickY, redInput.text, greenInput.text,
                                                         blueInput.text, alphaInput.text, smartBrushSens.value]);
                        if (parent.curTool == "RemoveRegion")
                            imageChange(parent.curTool, [clickX, clickY]);
                    }
    
                }
            }

            Image {
                id: filteredImage
                source: ""
                visible: true
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
        if (showFilteredImage.checked)
            filteredImage.source = "image://imageProvider/filteredImage?" + Math.random();
    }
}

