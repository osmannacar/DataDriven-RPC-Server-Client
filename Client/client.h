#ifndef RPCCLIENT_H
#define RPCCLIENT_H
#include <QObject>
#include <QSharedPointer>
#include "Rpc/templatereaderswriters.h"
#include "Network/tcpstreamsocket.h"
#include "Network/networkreader.h"
#include "Network/networkwriter.h"
#include "Serilization/templatereaderswriters.h"
#include "QCryptographicHash"
#include <QDebug>
#include <QDateTime>

class Client : public QObject
{
    Q_OBJECT
    TcpStreamSocket m_tcpSocket;
    QString m_user_info;
    QString m_server_name;
    NetworkReader *m_reader = nullptr;
    NetworkWriter *m_writer  = nullptr;
    TemplateReadersWriters *m_templatereaderWriter = nullptr;
public:
    Client(const QString &_ip, const ushort & _port)
    {
        if(!this->m_tcpSocket.createSocket()){
            qDebug() << "socket did not create";
            exit(EXIT_FAILURE);
        }
        if(!this->m_tcpSocket.connect(_ip, _port)){
            qDebug() << "client did not connect to server check ip or port";
            exit(EXIT_FAILURE);
        }
        this->m_reader = new NetworkReader(&this->m_tcpSocket);
        this->m_writer = new NetworkWriter(&this->m_tcpSocket);

        this->m_user_info = QString(QCryptographicHash::hash(QDateTime::currentDateTime().toString("yyyyMMdd hh:mm:ss").toUtf8(), QCryptographicHash::Md5).toHex());

        this->m_reader->read(this->m_server_name);
        qDebug() << "Server:" << this->m_server_name;
        this->m_writer->write(QString("login"));
        this->m_writer->write(this->m_user_info);
        QString l_connection_status;
        this->m_reader->read(l_connection_status);
        qDebug() << "Connection Status:" << l_connection_status;

        this->m_templatereaderWriter = new TemplateReadersWriters(this->m_writer, this->m_reader);
    }
    virtual ~Client(){
        this->m_tcpSocket.close();
        if(this->m_reader != nullptr){
            delete this->m_reader;
        }
        if(this->m_writer != nullptr){
            delete this->m_writer;
        }
        if(this->m_templatereaderWriter != nullptr){
            delete this->m_templatereaderWriter;
        }
    }
    template<typename ...T>  bool beginCall(const QString &_service_name, const QString &_method_name, const T &...args){
        return this->m_templatereaderWriter->marshall(QString("rpc"), _service_name, _method_name, args...);
    }
    template<typename ...T>  bool endCall(bool &result, QString &hata, T &...args){
        return this->m_templatereaderWriter->unMarshall(result, hata, args...);
    }

};

#endif // RPCCLIENT_H
