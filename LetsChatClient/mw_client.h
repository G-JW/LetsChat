#ifndef MW_CLIENT_H
#define MW_CLIENT_H

#include <QMainWindow>
#include <QtNetwork>
#include "message.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MW_Client; }
QT_END_NAMESPACE

class MW_Client : public QMainWindow
{
    Q_OBJECT

public:
    MW_Client(QWidget *parent = nullptr);
    ~MW_Client();
    Message toMessage(QString content);
    void connectServer();
    void connectSucceed();
    void sendMessage();
    void recvMessage();
    void disconnect();

private:
    Ui::MW_Client *ui;
    QString serverIP;
    qint16 serverPort;
    QTcpSocket tcpSocket;
    QString username;
    QString password;
    bool status;
};
#endif // MW_CLIENT_H
