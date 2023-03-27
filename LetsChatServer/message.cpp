#include "message.h"
#include <QIODevice>

Message::Message()
{
    this->type = 0;
    this->sender = "sender";
    this->receiver = "receiver";
    this->content = "content";
    this->timestamp = QDateTime::currentDateTime();
}

Message::Message(int type, QString sender, QString receiver, QString content)
{
    this->type = type;
    this->sender = sender;
    this->receiver = receiver;
    this->content = content;
    this->timestamp = QDateTime::currentDateTime();
}

QByteArray Message::serializeMessage()
{
    QByteArray data;
    QDataStream out(&data, QIODevice::WriteOnly);

    // 消息体长度，先写入0占位
    // out << (quint32)0;

    // 消息内容
    out << this->type << this->sender << this->receiver << this->content << this->timestamp;

    // 计算消息体长度并回填
    // out.device()->seek(0);
    // out << (quint32)(data.size() - sizeof(quint32));

    return data;
}

void Message::deserializeMessage(const QByteArray &data)
{
    QDataStream in(data);
    in.setVersion(QDataStream::Qt_6_4);

    in >> this->type >> this->sender >> this->receiver >> this->content >> this->timestamp;
}

QString Message::toQString()
{
    QString qString;
    qString = this->sender + ' ' + this->receiver + ' ' + this->content + ' ' + this->timestamp.toString();
    return qString;
}

QString Message::toChat()
{
    QString qString;
    qString = this->sender + ": " + this->content;
    return qString;
}

QString Message::getReceiver()
{
    return this->receiver;
}

int Message::getType()
{
    return this->type;
}
