#include "user.h"

User::User(std::string &userName, uint userId):
    _name(userName),
    _userId(userId)
{
}

bool User::RequestPermissionToFollow(uint follower)
{
    //here is where I can choose to accept or deny follow request

    //I want to know who is following me
    _followers.push_back(follower);

    //I deliberately choose to be nice to everyone
    return true;
}

void User::RemoveFromFollowersList(uint follower)
{
    _followers.erase(std::remove(_followers.begin(), _followers.end(), follower), _followers.end());
}

void User::Follow(uint leader)
{
    _leaders.push_back(leader);
}

void User::Unfollow(uint leader)
{
    _leaders.erase(std::remove(_leaders.begin(), _leaders.end(), leader), _leaders.end());
}
