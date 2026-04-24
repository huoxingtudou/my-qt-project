QT += quick multimedia core serialport

QMAKE_CXXFLAGS += -Wno-error=implicit-function-declaration

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        cameraimageprovider.cpp \
        camerathread.cpp \
        main.cpp \
        serialportmanager.cpp \
        timemanager.cpp \
        timethread.cpp

RESOURCES += qml.qrc

macx {
    QMAKE_INFO_PLIST = $$PWD/Info.plist
}

macx {


    # 头文件路径
    INCLUDEPATH += /opt/homebrew/Cellar/opencv/4.13.0_7/include/opencv4

    # 库文件路径
    LIBS += -L/opt/homebrew/Cellar/opencv/4.13.0_7/lib \
            -lopencv_core \
            -lopencv_videoio \
            -lopencv_imgproc \
            -lopencv_imgcodecs
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    cameraimageprovider.h \
    camerathread.h \
    serialportmanager.h \
    timemanager.h \
    timethread.h

DISTFILES += \
    Info.plist
