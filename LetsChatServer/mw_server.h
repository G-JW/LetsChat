#ifndef MW_SERVER_H
#define MW_SERVER_H

#include "message.h"
#include <QMainWindow>
#include <QtNetwork>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MW_Server;
}
QT_END_NAMESPACE

class MW_Server : public QMainWindow
{
    Q_OBJECT

public:
    MW_Server(QWidget *parent = nullptr);
    ~MW_Server();
    void showInfo(const QString &qString);
    void init();
    void start();
    void clear();
    void addClient();
    void reduceClient();
    void getMessage();
    void putMessage(Message message);

private:
    Ui::MW_Server *ui;
    QString hostName;
    QString ipAddress;
    qint16 port;
    QTcpServer tcpServer;
    QList<Message> msgList;
    QList<QTcpSocket *> tcpClientList;
    QSet<QString> userSet;
};
#endif // MW_SERVER_H
