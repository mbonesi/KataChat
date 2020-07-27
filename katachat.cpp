#include <cstdarg>
#include <cstring>

#include "katachat.h"
#include "userpost.h"

#define HALLO_WORLD "Hallo world!"

/* courtesy stackoverflow.com */
std::string format(const std::string format, ...)
{
    va_list args;
    va_start (args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end (args);
    std::vector<char> vec(len + 1);
    va_start (args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end (args);
    return &vec[0];
}


User *KataChat::findUser(std::string  &UserName)
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

User *KataChat::findUser(uint userId)
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

void KataChat::print(std::string  &poster, UserPost &post)
{
    unsigned long long secs=time(0) -  post.SecondsSinceEpoch();

    std::string timeOffsetString;

    if(secs < 60)
    {
        timeOffsetString = format(" (%d seconds ago)", secs);
    }
    else
    {
        int mins = secs/60;
        if(mins < 60)
        {
            timeOffsetString = format(" (%d minutes ago)", mins);
        }
        else
        {
            int hrs = secs/3600;
            if(hrs < 24)
            {
                timeOffsetString = format(" (%d hours ago)", hrs);
            }
            else
            {
                int days = hrs/24;
                if(days < 365)
                    timeOffsetString = format(" (%d days ago)", hrs/24);
                else
                    timeOffsetString = format(" (%d years ago)", days/365);
            }
        }
    }
    _outStream << poster << " - " << post.GetText() << timeOffsetString << std::endl;
}

KataChat::KataChat(std::ostream &out) :
    _outStream(out)
{
    srand(time(0)); //seed the random numbers generator
}

void KataChat::AddUser(std::string &userName)
{
    uint userId = rand();
    User *pNewUser = new User(userName, userId);
    _users.push_back(pNewUser);
    _outStream << "User " << userName << " [id=" << userId << "] was added to the system." << std::endl;

    Post(userName, HALLO_WORLD);
}

void KataChat::RemoveUser(std::string &userName)
{
    User *pUser = findUser(userName);
    if(pUser)
    {
        std::list<User *>::iterator i = _users.begin();
        while (i != _users.end())
        {
            if((*i)->GetUserId() == pUser->GetUserId())
            {
                _users.erase(i++);
            }
            else
            {
                i++;
            }
        }
        _outStream << "User " << userName << " was removed from system." << std::endl;

        if(_users.size() == 0)
            ListUsers();
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << std::endl;
    }
}

void KataChat::ListUsers()
{
    if(_users.size())
    {
        _outStream << "The following users are active in the system so far: ";
        foreach (User *u, _users)
        {
            _outStream << u->GetUserName() << " ";
        }
        _outStream << std::endl;
    }
    else
    {
        _outStream << "there are no users in the system" << std::endl;
    }
}


//KataChat operations
uint KataChat::Post(std::string &userName, std::string msg)
{
    uint postId = rand();
    User *pUser = findUser(userName);
    if(pUser)
    {
        //I need a certain chronologicaly ordering on content dump (hence insert at beginning)
        _board.insert(_board.begin(), UserPost(msg, pUser->GetUserId(), postId));
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << std::endl;
    }
    return postId;
}

void KataChat::Follow(std::string &follower, std::string &leader)
{
    User *pFollower = findUser(follower);
    User *pLeader   = findUser(leader);
    if(pFollower && pLeader)
    {
        if(pLeader->RequestPermissionToFollow(pFollower->GetUserId()))
        {
            pFollower->Follow(pLeader->GetUserId());
            _outStream << follower << " is now following " << leader << std::endl;
        }
        else
        {
            _outStream<< leader << " denied follow request from " << follower << std::endl;
        }
    }
    else
    {
        _outStream << "Either one (or both) of " << follower << ", " << leader<< " are not active users in the system,  Sorry." << std::endl;
    }
}

void KataChat::Unfollow(std::string &follower, std::string &leader)
{
    User *pFollower = findUser(follower);
    User *pLeader   = findUser(leader);
    if(pFollower && pLeader)
    {
        pFollower->Unfollow(pLeader->GetUserId());
        pLeader->RemoveFromFollowersList(pFollower->GetUserId());
        _outStream << follower << " is not following " << leader << " anymore" << std::endl;
    }
    else
    {
        _outStream << "Either one (or both) of " << follower << ", " << leader<< " are not active users in the system,  Sorry." << std::endl;
    }
}

void KataChat::Read(std::string &userName)
{
    User *pUser = findUser(userName);    
    if(pUser)
    {
        uint userId = pUser->GetUserId();
        foreach (UserPost p, _board) {
            if(p.GetOwner() == userId)
                print(userName, p);
        }
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << std::endl;
    }
}

void KataChat::Wall(std::string &userName)
{
    User *pUser = findUser(userName);
    if(pUser)
    {
        std::vector<uint> matchers;
        matchers.push_back(pUser->GetUserId());
        matchers.insert(matchers.end(), pUser->GetLeaders().begin(), pUser->GetLeaders().end());

        foreach (UserPost p, _board)
        {
            for (uint i=0; i<matchers.size(); i++)
            {
                if(matchers[i] == p.GetOwner())
                {
                    //must get the username here (space for improvement!!)
                    pUser = findUser(p.GetOwner());
                    if(pUser)
                    {
                        //only if user is still acitve in the system
                        print(pUser->GetUserName(), p);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        _outStream << "User " << userName << " does not exist, Sorry." << std::endl;
    }
}
