#ifndef MESSAGE_H
#define MESSAGE_H

#include <QByteArray>
#include <QString>
#include <QDateTime>

class Message
{
public:
    Message();
    Message(int type, QString sender, QString receiver, QString content);
    QByteArray serializeMessage();
    void deserializeMessage(const QByteArray &data);
    QString toQString();
    QString toChat();
    int getType();
    QString getReceiver();

private:
    int type;   //2-LoginRequest
    QString sender;
    QString receiver;
    QString content;
    QDateTime timestamp;
};

#endif // MESSAGE_H
