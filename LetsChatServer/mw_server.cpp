#include "mw_server.h"
#include "./ui_mw_server.h"

MW_Server::MW_Server(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MW_Server)
{
    ui->setupUi(this);
    init();

    // QPushButton *PB_start = findChild<QPushButton*>("PB_start");
    // connect(PB_start,&QPushButton::clicked, this, &MW_Server::start);
    connect(ui->PB_start, &QPushButton::clicked, this, &MW_Server::start);
    connect(ui->PB_clear, &QPushButton::clicked, this, &MW_Server::clear);
    connect(&tcpServer, &QTcpServer::newConnection, this, &MW_Server::addClient);
}

MW_Server::~MW_Server()
{
    delete ui;
}

void MW_Server::showInfo(const QString &qString)
{
    ui->TB_info->insertPlainText(qString + '\n');
}

void MW_Server::init()
{
    // get local hostname and ip address
    hostName = QHostInfo::localHostName();
    QHostInfo hostInfo = QHostInfo::fromName(hostName);
    foreach (const QHostAddress &address, hostInfo.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback())
        {
            ipAddress = address.toString();
            break;
        }
    }
    // qDebug() << "Local host name: " << hostName;
    // qDebug() << "Local IP address: " << ipAddress;
    ui->LE_ipAddress->setText(ipAddress);

    /*
    Message message{1,"GJW","Lobby","Hello everyone!"};
    QByteArray byteArray = message.serializeMessage();
    message.deserializeMessage(byteArray);

    qDebug() << message.toQString() << Qt::endl;
    qDebug() << message.toChat() << Qt::endl;
    */
    showInfo("Server initialized successfully !");
}

void MW_Server::start()
{
    port = ui->SB_port->value();
    // qDebug() << port << Qt::endl;
    // showInfo(QString::number(port));
    tcpServer.listen(QHostAddress::Any, port);
    showInfo("Server started successfully !");
    ui->PB_start->setEnabled(false);
    ui->SB_port->setEnabled(false);
}

void MW_Server::clear()
{
    msgList.clear();
    ui->TB_info->clear();
}

void MW_Server::addClient()
{
    QTcpSocket *tcpClientSocket = tcpServer.nextPendingConnection();
    tcpClientList.append(tcpClientSocket);
    connect(tcpClientSocket, &QTcpSocket::readyRead, this, &MW_Server::getMessage);
    connect(tcpClientSocket, &QTcpSocket::disconnected, this, &MW_Server::reduceClient);
}

void MW_Server::reduceClient()
{
}

void MW_Server::getMessage()
{
    for (int i = 0; i < tcpClientList.count(); i++)
    {
        if (tcpClientList.at(i)->bytesAvailable())
        {
            QByteArray readbuf = tcpClientList.at(i)->readAll();
            Message message;
            message.deserializeMessage(readbuf);
            ui->TB_info->insertPlainText(message.toChat() + '\n');
            putMessage(message);
        }
    }
}

void MW_Server::putMessage(Message message)
{
    if (message.getReceiver() == "Lobby")
        for (int i = 0; i < tcpClientList.count(); i++)
        {
            tcpClientList.at(i)->write(message.serializeMessage());
        }
}
