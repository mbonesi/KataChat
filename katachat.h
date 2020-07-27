#ifndef KATACHAT_H
#define KATACHAT_H

#include <QObject>
#include <QVector>
#include <QTextStream>

#include "user.h"

#define USERLIST    "users"
#define ADDUSER     "add"
#define REMOVEUSER  "remove"
#define FOLLOW      "follows"
#define UNFOLLOW    "unfollows"
#define POST        "->"
#define WALL        "wall"
#define QUIT        "quit"

class KataChat
{
    QList<User *> _users;
    QVector<UserPost> _board;

    QTextStream &_outStream;

    User    *findUser(QString &UserName);
    User    *findUser(quint32 userId);

    void    print(QString &poster, UserPost &post);
public:
    KataChat(QTextStream &out);

    //users management
    void    AddUser(QString &userName);
    void    RemoveUser(QString &userName);
    void    ListUsers();

    //operations
    quint32 Post(QString &userName, QString msg);
    void    Follow(QString &follower, QString &leader);
    void    Unfollow(QString &follower, QString &leader);
    void    Read(QString &userName);
    void    Wall(QString &userName);
};

#endif // KATACHAT_H

