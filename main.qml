import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls
// import timeMoudle 1.0

ApplicationWindow{
         visible: true
         width:640
         height:480
         title:"OpenCv摄像头画面"

         Image{
                  id:cameraDisplay
                  anchors {
                           top: parent.top
                           left: parent.left
                           right: parent.right
                           bottom: timeText.top   // 紧贴文字上方
                  }
                  fillMode: Image.PreserveAspectFit
                  cache: false //禁用缓存，保证实时刷新
                  source: "image://camera/stream"
                  onStatusChanged: {
                           if (status === Image.Loading) console.log("QML: 正在加载图像...")
                           else if (status === Image.Ready) console.log("QML: 图像已显示")
                           else if (status === Image.Error) console.log("QML: 图像加载错误")
                  }
                  // 强制刷新：每 50ms 在 URL 后追加一个递增的 query 参数
                  property int refreshCounter: 0
                  Timer {
                           interval: 50
                           running: true
                           repeat: true
                           onTriggered: {
                                    cameraDisplay.refreshCounter++
                                    cameraDisplay.source = "image://camera/stream?" + cameraDisplay.refreshCounter
                           }
                  }

         }
         // TimeManager{
         //          id:timeManager
         // }
         Text{
                  id:timeText
                  anchors{
                           bottom: parent.bottom
                           left: parent.left
                           right: parent.right
                           margins: 10
                  }
                  color: "red"
                  height: 40
                  font.pixelSize: 20
                  text:timeManager.m_time?timeManager.m_time.toString("hh:mm:ss.zzz"):'时间错误'
                  // text:"时间时间"
         }
}
