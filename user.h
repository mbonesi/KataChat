#ifndef USER_H
#define USER_H

#include <QObject>
#include <QVector>

#include "userpost.h"

class User
{    
    friend class KataChat; //I know using friends is discouraged but in this case I think it's appropriate

    QString _name;
    quint32 _userId;
    QVector<quint32> _leaders;    //users I follow
    QVector<quint32> _followers;  //users who follow me

protected:
    quint32 GetUserId() {return _userId;}
    QString &GetUserName() {return _name;}
    QVector<quint32> &GetLeaders() { return _leaders;}

public:
    explicit User(QString &userName, quint32 userId);

    //as leader
    bool RequestPermissionToFollow(quint32 follower);
    void RemoveFromFollowersList(quint32 follower);

    //as follower
    void Follow(quint32 leader);
    void Unfollow(quint32 leader);
};

#endif // USER_H
