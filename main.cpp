#include <QGuiApplication>
#include <QQmlApplicationEngine>
// #include <opencv2/opencv.hpp>
#include "camerathread.h"
#include "cameraimageprovider.h"
#include <QQmlContext>
#include <QPermissions>
#include "timemanager.h"
#include "serialportmanager.h"
#include "timethread.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // 必须在 QApplication 创建之前设置
    // qputenv("OPENCV_AVFOUNDATION_SKIP_AUTH", "1");

    QGuiApplication app(argc, argv);

    // qmlRegisterType<TimeManager>("timeMoudle",1,0,"TimeManager");
    QQmlApplicationEngine engine;

    TimeManager *timeManager = new TimeManager(&app);
    engine.rootContext()->setContextProperty("timeManager",timeManager);

    SerialPortManager *serialManager =new SerialPortManager(&app);
    engine.rootContext()->setContextProperty("serialManager",serialManager);

    //1.创建ImageProvider实例，并注册到QML引擎
    CameraImageProvider *imageProvider = new CameraImageProvider();
    engine.addImageProvider("camera",imageProvider);

    engine.rootContext()->setContextProperty("cameraProvider", imageProvider);
    //2.创建摄像头采集线程
    CameraThread *cameraThread = new CameraThread(imageProvider,timeManager);



    //3.连接调试信号，用于在控制台输出状态

    QObject::connect(cameraThread,&CameraThread::statusChanged,
                     [](const QString &msg){ qDebug()<<msg ;});



    QCameraPermission cameraPermission;
    qApp->requestPermission(cameraPermission, [cameraThread](const QPermission &permission) {
        if (permission.status() == Qt::PermissionStatus::Granted) {
            qDebug() << "权限已授予，现在启动摄像头线程";
            // 4.启动采集线程
            cameraThread->start();   // ✅ 授权后才启动
        } else {
            qDebug() << "权限被拒绝";
        }
    });

    timeThread *timethread = new timeThread(timeManager);
    timethread->start();

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QCoreApplication::exec();
}
