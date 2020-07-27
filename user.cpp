#include "user.h"

#include <QDebug>


User::User(QString &userName, quint32 userId) :
    _name(userName),
    _userId(userId)
{
}

bool User::RequestPermissionToFollow(quint32 follower)
{
    //here is where I can choose to accept or deny follow request

    //I want to know who is following me
    _followers << follower;

    //I deliberately choose to be nice to everyone
    return true;
}

void User::RemoveFromFollowersList(quint32 follower)
{
    _followers.removeAll(follower);
}

void User::Follow(quint32 leader)
{
    _leaders << leader;
}

void User::Unfollow(quint32 leader)
{
    _leaders.removeAll(leader);
}
