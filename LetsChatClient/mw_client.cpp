#include "mw_client.h"
#include "./ui_mw_client.h"

MW_Client::MW_Client(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MW_Client)
{
    ui->setupUi(this);
    status = false;
    connect(&tcpSocket, &QTcpSocket::connected, this, &MW_Client::connectSucceed);
    connect(&tcpSocket, &QTcpSocket::disconnected, this, &MW_Client::disconnect);
    connect(&tcpSocket, &QTcpSocket::readyRead, this, &MW_Client::recvMessage);
    connect(ui->PB_Connect, &QPushButton::clicked, this, &MW_Client::connectServer);
    connect(ui->PB_Send, &QPushButton::clicked, this, &MW_Client::sendMessage);
}

MW_Client::~MW_Client()
{
    delete ui;
}

Message MW_Client::toMessage(QString content)
{
    return Message{1, username, "Lobby", content};
}

void MW_Client::connectServer()
{
    if (status == false)
    {
        serverIP = ui->LE_ServerIP->text();
        serverPort = ui->SB_ServerPort->value();
        username = ui->LE_Username->text();
        ui->centralwidget->setWindowTitle("LetsChatClient@" + username);
        tcpSocket.connectToHost(serverIP, serverPort);
    }
    else
    {
        Message message{1, username, "Lobby", "GoodBye"};
        tcpSocket.write(message.serializeMessage());
        tcpSocket.disconnectFromHost();
        status = false;
    }
}

void MW_Client::connectSucceed()
{
    status = true;
    ui->PB_Send->setEnabled(true);
    ui->LE_ServerIP->setEnabled(false);
    ui->SB_ServerPort->setEnabled(false);
    ui->LE_Username->setEnabled(false);
    ui->LE_Password->setEnabled(false);
    ui->PB_Connect->setText("disconnect");
    Message msg = toMessage("Hello");
    tcpSocket.write(msg.serializeMessage());
}

void MW_Client::sendMessage()
{
    if (ui->PT_Chat->toPlainText() == "")
        return;
    Message msg = toMessage(ui->PT_Chat->toPlainText());
    tcpSocket.write(msg.serializeMessage());
    ui->PT_Chat->clear();
}

void MW_Client::recvMessage()
{
    if (tcpSocket.bytesAvailable())
    {
        QByteArray recvBuf;
        recvBuf.resize(tcpSocket.bytesAvailable());
        tcpSocket.read(recvBuf.data(), recvBuf.size());
        Message msg;
        msg.deserializeMessage(recvBuf);
        ui->PT_Info->insertPlainText(msg.toChat() + '\n');
    }
}

void MW_Client::disconnect()
{
    ui->PB_Send->setEnabled(false);
    ui->LE_ServerIP->setEnabled(true);
    ui->SB_ServerPort->setEnabled(true);
    ui->LE_Username->setEnabled(true);
    ui->LE_Password->setEnabled(true);
    ui->PB_Connect->setText("Connect");
}
