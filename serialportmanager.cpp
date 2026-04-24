#include "serialportmanager.h"
#include <QDebug>
#include <QSettings>
#include <QDateTime>
#include <QDir>
#include <QStandardPaths>
#include <QUrl>

SerialPortManager::SerialPortManager(QObject *parent)
    : QObject{parent},m_serial(new QSerialPort(this))
{
    bool ok = connect(m_serial,&QSerialPort::readyRead,this,&SerialPortManager::handleReadyRead);
    // connect(m_serial,&QSerialPort::errorOccurred,this,&SerialPortManager::handleError);
    qDebug() << "connect result:" << ok;
}
bool SerialPortManager::openPort(const QString &portName,int baudRate)
{
    qDebug()<<m_serial->isOpen()<<"打开串口前串口的状态";
    if(m_serial->isOpen()){
        qDebug()<<"串口关闭";
        qDebug()<<m_serial->isOpen();
        m_serial->close();

    }
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    m_serial->setPortName(portName);
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);
    // qDebug()<<"打开串口"<<m_serial->open(QIODeviceBase::ReadWrite);
    if(m_serial->open(QIODeviceBase::ReadWrite)){
        qDebug()<<"串口打开成功"<<portName;
        saveSettings(portName,baudRate);

        return true;
    }else{
        qDebug()<<"串口打开失败"<<portName;
        emit errorOccurred("串口打开失败："+m_serial->errorString());
        return false;
    }

}
void SerialPortManager::saveSettings(const QString &portName,int baudRate)
{
    QSettings settings;
    settings.setValue("serial/port",portName);
    settings.setValue("serial/baud",baudRate);
    qDebug()<<"配置已保存"<<portName<<baudRate;

}

QVariantMap SerialPortManager::loadSettings()
{
    QSettings settings;
    QVariantMap map;

    map["port"] =settings.value("serial/port","").toString();
    map["baud"] = settings.value("serial/baud",9600).toInt();
    qDebug()<<"加载配置"<<map["port"]<<map["baud"];
    return map;
}


void SerialPortManager::closePort()
{
    if(m_serial->isOpen()){
        m_serial->close();
        qDebug()<<"串口已关闭";
    }
}




void SerialPortManager::handleReadyRead()
{
    QByteArray data = m_serial->readAll();
    m_lastData = QString::fromUtf8(data);
    emit dataReceived(m_lastData);
    qDebug()<<"收到数据"<<m_lastData;
    appendLog(m_lastData);
}
void SerialPortManager::handleError(QSerialPort::SerialPortError error)
{
    if(error!=QSerialPort::NoError){
        emit errorOccurred("串口打开失败："+m_serial->errorString());
    }
}

bool SerialPortManager::startLogging(const QString &filePath)
{
    if(m_loggingEnabled){
        stopLogging();
    }
    m_logFile.setFileName(filePath);
    if(!m_logFile.open(QIODeviceBase::WriteOnly|QIODeviceBase::Text)){

        emit errorOccurred("无法创建日志文件"+filePath);
        return false;
    }


    QTextStream out(&m_logFile);
    out <<"时间,温度,湿度\n";
    out.flush();

    m_loggingEnabled = true;
    emit loggingChanged(true);
    qDebug()<<"开始记录日志";
    return true;
}

void SerialPortManager::stopLogging()
{

    if(m_loggingEnabled){
        m_logFile.close();
        m_loggingEnabled = false;
        emit loggingChanged(false);
        qDebug()<<"停止记录日志";
    }
}

void SerialPortManager::appendLog(const QString &data)
{
    if(!m_loggingEnabled || !m_logFile.isOpen()) return;

    QStringList parts = data.split(',');

    if(parts.size() < 2) return;

    QString temp = parts[0].mid(2);
    QString humi = parts[1].mid(2);

    QTextStream out(&m_logFile);

    out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") << ","
        <<temp <<","<<humi <<"\n";

    //立即写入磁盘，防止程序崩溃时的丢失数据
    out.flush();
}

QString SerialPortManager::getFileName()
{
    qDebug()<<"那名称";
    QString dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (dir.isEmpty()) {
        dir = QDir::homePath();  // 兜底：用用户主目录
    }
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd-hhmmss");

    QString filePath = QString("%1/data_log_%2.csv").arg(dir,timestamp);


    QString fileUrl = QUrl::fromLocalFile(filePath).toString();
    qDebug()<<fileUrl;
    return fileUrl;
}

SerialPortManager::~SerialPortManager(){
    stopLogging();
}