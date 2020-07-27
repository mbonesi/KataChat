#ifndef USER_H
#define USER_H

#include <vector>

#include "userpost.h"

class User
{    
    friend class KataChat; //I know using friends is discouraged but in this case I think it's appropriate

    std::string _name;
    uint _userId;
    std::vector<uint> _leaders;    //users I follow
    std::vector<uint> _followers;  //users who follow me

protected:
    uint GetUserId() {return _userId;}
    std::string &GetUserName() {return _name;}
    std::vector<uint> &GetLeaders() { return _leaders;}

public:
    explicit User(std::string &userName, uint userId);

    //as leader
    bool RequestPermissionToFollow(uint follower);
    void RemoveFromFollowersList(uint follower);

    //as follower
    void Follow(uint leader);
    void Unfollow(uint leader);
};

#endif // USER_H
