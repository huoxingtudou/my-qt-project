#ifndef SERIALPORTMANAGER_H
#define SERIALPORTMANAGER_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QVariantMap>
#include <QFile>

class SerialPortManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_lastData READ lastData  NOTIFY dataReceived)


    Q_PROPERTY(bool logging READ islogging NOTIFY loggingChanged)

    // Q_PROPERTY(QString fileName READ setFileName NOTIFY fileNameChanged FINAL)
public:
    explicit SerialPortManager(QObject *parent = nullptr);
    ~SerialPortManager();

    Q_INVOKABLE bool openPort(const QString &portName,int baudRate);

    Q_INVOKABLE void closePort();

    QString lastData()const{return m_lastData;};

    //新增保存配置
    Q_INVOKABLE void saveSettings(const QString &portName,int baudRate);

    //新增加载配置
    Q_INVOKABLE QVariantMap loadSettings();

    Q_INVOKABLE bool startLogging(const QString &filePath);

    Q_INVOKABLE void stopLogging();

    bool islogging() const{ return m_loggingEnabled; }

    Q_INVOKABLE QString getFileName();


signals:
    void dataReceived(const QString &data);
    void errorOccurred(const QString &error);

    void loggingChanged(bool logging);
    // void fileNameChanged();

private:
    void handleReadyRead();
    void handleError(QSerialPort::SerialPortError error);

private:
    QString m_lastData;
    QSerialPort *m_serial=nullptr;

    bool m_loggingEnabled;
    QFile m_logFile;
    QString m_fileName;

    void appendLog(const QString &data); //内部可以追加写入函数
    // bool logging;
};

#endif // SERIALPORTMANAGER_H
