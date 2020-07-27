#ifndef USERPOSTS_H
#define USERPOSTS_H

#include <QObject>
#include <QTextStream>
#include <QDateTime>

class UserPost
{
    QDateTime _postedTime;

    QString _text;
    quint32 _user;
    quint32 _id;
public:

    UserPost() = default;
    UserPost(QString txt, quint32 userId, quint32 postId);
    const QString &GetText() {return _text;}
    quint32 GetOwner() {return _user;}
    quint32 GetId() {return _id;}

    qint64 SecondsSinceEpoch() {return _postedTime.toSecsSinceEpoch();}
};


#endif // USERPOSTS_H
