import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
import QtQuick.Dialogs
import "./Components/Common"

// import timeMoudle 1.0

ApplicationWindow {
    visible: true
    width: 740
    height: 480
    title: "OpenCv摄像头画面"
    Component.onCompleted: {
        let settings = serialManager.loadSettings();
        if (settings.port !== "") {
            portCombo.currentIndex = portCombo.find(settings.port);
        }
        if (settings.baud !== 0) {
            baudCombo.currentIndex = baudCombo.find(settings.baud);
        }
    }

    Image {
        id: cameraDisplay
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: bottomRow.top   // 紧贴文字上方
            topMargin: 50
            // bottomMargin: 50
        }
        fillMode: Image.PreserveAspectFit
        cache: false //禁用缓存，保证实时刷新
        source: "image://camera/stream"
        onStatusChanged: {
            // if (status === Image.Loading) console.log("QML: 正在加载图像...")
            // else if (status === Image.Ready) console.log("QML: 图像已显示")
            // else if (status === Image.Error) console.log("QML: 图像加载错误")
        }
        // 强制刷新：每 50ms 在 URL 后追加一个递增的 query 参数
        property int refreshCounter: 0
        Timer {
            interval: 50
            running: true
            repeat: true
            onTriggered: {
                cameraDisplay.refreshCounter++;
                cameraDisplay.source = "image://camera/stream?" + cameraDisplay.refreshCounter;
            }
        }
    }

    Row {
        id: bottomRow
        spacing: 10
        // height:50
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            margins: 10
            topMargin: 50
        }

        Text {
            color: "red"
            // height: 40
            font.pixelSize: 20
            text: timeManager.m_time ? timeManager.m_time.toString("hh:mm:ss.zzz") : '时间错误'
            // text:"时间时间"
        }
        Button {
            text: serialManager.logging ? "停止记录" : "开始记录"
            onClicked: {
                if (serialManager.logging) {
                    serialManager.stopLogging();
                } else {
                    saveFileDialog.currentFile = serialManager.getFileName();
                    saveFileDialog.open();
                }
            }
        }
        Rectangle {
            width: 12
            height: 12
            radius: 6
            color: serialManager.logging ? "red" : "gray"
            anchors.verticalCenter: parent.verticalCenter
        }
        Text {
            text: serialManager.logging ? "记录中" : "未记录"
            color: "black"
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    FileDialog {
        id: saveFileDialog
        fileMode: FileDialog.SaveFile
        // currentFile: serialManager.getFileName()
        nameFilters: ["CSV 文件 (*.csv)"]
        onAccepted: {
            console.log(selectedFile.toString());
            let filepath = selectedFile.toString().replace("file://", "");
            serialManager.startLogging(filepath);
        }
    }
    Text {
        id: serialText
        text: serialManager.m_lastData || "等待数据..."
        // text:"等待数据"
        color: "yellow"
        height: 40
        font.pixelSize: 24
    }
    Row {
        anchors.left: serialText.right
        anchors.leftMargin: 20
        spacing: 10
        // anchors.centerIn: parent

        // 串口号选择
        ComboBox {
            id: portCombo
            width: 150
            model: ["/dev/ttys001", "/dev/ttys002", "/dev/ttys003", "/dev/ttys004", "/dev/ttys005"]
        }

        // 波特率选择
        ComboBox {
            id: baudCombo
            width: 100
            model: ["9600", "19200", "38400", "57600", "115200"]
        }

        Button {
            text: "打开串口"
            onClicked: {
                var port = portCombo.currentText;
                var baud = parseInt(baudCombo.currentText);
                serialManager.openPort(port, baud);
            }
        }
        Button {

            text: "关闭串口"
            onClicked: {
                serialManager.closePort();
                // serialText.text=""
            }
        }
        Button {

            text: "截图"
            onClicked: {
                fileDialog.open();
            }
        }
    }
    FileDialog {
        id: fileDialog
        fileMode: FileDialog.SaveFile
        nameFilters: ["PNG 图片 (*.png)", "JPEG 图片 (*.jpg)"]
        onAccepted: {
            cameraDisplay.grabToImage(function (result) {
                result.saveToFile(fileDialog.selectedFile);
                console.log("截图已保存");
            });
        }
    }
    ErrorDialog {
        id: errorDialg
    }
    Connections {
        target: serialManager
        function onErrorOccurred(error) {
            console.log(error);
            errorDialg.text = "串口错误";
            errorDialg.informativeText = "错误" + error;
            errorDialg.open();
        }
    }
}
