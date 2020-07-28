#include "katachat.h"
#include "userpost.h"

#include <QRandomGenerator>
#include <QVector>

#define HALLO_WORLD "Hallo World!"

User *KataChat::findUser(QString &UserName)
{
    foreach (User *u, _users)
    {
        if (0 == u->GetUserName().compare(UserName))
        {
            return u;
        }
    }
    return nullptr; //not found
}

User *KataChat::findUser(quint32 userId)
{
    foreach (User *u, _users)
    {
        if (userId == u->GetUserId())
        {
            return u;
        }
    }
    return nullptr; //not found
}

void KataChat::print(QString &poster, UserPost &post)
{
    qint64 secs=QDateTime::currentDateTime().toSecsSinceEpoch() -  post.SecondsSinceEpoch();

    QString timeOffsetString;

    if(secs < 60)
    {
        timeOffsetString = QString(" (%0 seconds ago)").arg(secs);
    }
    else
    {
        int mins = secs/60;
        if(mins < 60)
        {
            timeOffsetString = QString(" (%0 minutes ago)").arg(mins);
        }
        else
        {
            int hrs = secs/3600;
            if(hrs < 24)
            {
                timeOffsetString = QString(" (%0 hours ago)").arg(hrs);
            }
            else
            {
                int days = hrs/24;
                if(days < 365)
                    timeOffsetString = QString(" (%0 days ago)").arg(hrs/24);
                else
                    timeOffsetString = QString(" (%0 yearsago)").arg(days/365);
            }
        }
    }
    _outStream << poster << " - " << post.GetText() << timeOffsetString << endl;
}

KataChat::KataChat(QTextStream &out) :
    _outStream(out)
{    
}

void KataChat::AddUser(QString &userName)
{
    quint32 userId = QRandomGenerator::system()->generate();
    User *pNewUser = new User(userName, userId);
    _users << pNewUser;
    _outStream << "User " << userName << " [id=" << userId << "] was added to the system." << endl;

    Post(userName, HALLO_WORLD);
}

void KataChat::RemoveUser(QString &userName)
{
    User *pUser = findUser(userName);
    if(pUser)
    {
        for (int i = 0; i < _users.size(); ++i)
        {
            if (_users.at(i)->GetUserId() == pUser->GetUserId())
                _users.removeAt(i);
        }
        _outStream << "User " << userName << " was removed from system." << endl;

        if(_users.size() == 0)
            ListUsers();
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << endl;
    }
}

void KataChat::ListUsers()
{
    if(_users.count())
    {
        _outStream << "The following users are active in the system so far: ";
        foreach (User *u, _users)
        {
            _outStream << u->GetUserName() << " ";
        }
        _outStream << endl;
    }
    else
    {
        _outStream << "there are no users in the system" << endl;
    }
}


//KataChat operations
quint32 KataChat::Post(QString &userName, QString msg)
{
    quint32 postId = QRandomGenerator::system()->generate();
    User *pUser = findUser(userName);
    if(pUser)
    {
        //I need a certain chronologicaly ordering on content dump (hence "push_front")
        _board.push_front(UserPost(msg, pUser->GetUserId(), postId));
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << endl;
    }
    return postId;
}

void KataChat::Follow(QString &follower, QString &leader)
{
    User *pFollower = findUser(follower);
    User *pLeader   = findUser(leader);
    if(pFollower && pLeader)
    {
        if(pLeader->RequestPermissionToFollow(pFollower->GetUserId()))
        {
            pFollower->Follow(pLeader->GetUserId());
            _outStream << follower << " is now following " << leader << endl;
        }
        else
        {
            _outStream<< leader << " denied follow request from " << follower << endl;
        }
    }
    else
    {
        _outStream << "Either one (or both) of " << follower << ", " << leader<< " are not active users in the system,  Sorry." << endl;
    }
}

void KataChat::Unfollow(QString &follower, QString &leader)
{
    User *pFollower = findUser(follower);
    User *pLeader   = findUser(leader);
    if(pFollower && pLeader)
    {
        pFollower->Unfollow(pLeader->GetUserId());
        pLeader->RemoveFromFollowersList(pFollower->GetUserId());
        _outStream << follower << " is not following " << leader << " anymore" << endl;
    }
    else
    {
        _outStream << "Either one (or both) of " << follower << ", " << leader<< " are not active users in the system,  Sorry." << endl;
    }
}

void KataChat::Read(QString &userName)
{
    User *pUser = findUser(userName);    
    if(pUser)
    {
        quint32 userId = pUser->GetUserId();
        foreach (UserPost p, _board) {
            if(p.GetOwner() == userId)
                print(userName, p);
        }
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << endl;
    }
}

void KataChat::Wall(QString &userName)
{
    User *pUser = findUser(userName);
    if(pUser)
    {
        QVector<quint32> matchers;
        matchers << pUser->GetUserId();
        matchers << pUser->GetLeaders();

        foreach (UserPost p, _board)
        {
            if(matchers.contains(p.GetOwner()))
            {
                //must get the username here (space for improvement!!)
                pUser = findUser(p.GetOwner());
                if(pUser)
                {
                    //only if user is still acitve in the system
                    print(pUser->GetUserName(), p);                    
                    /*
                     * actually here I envision a good place to apply the visitor design pattern
                     * although it should be coded in wisley to prevent endless loops...
                     */
                }
            }
        }
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << endl;
    }
}
