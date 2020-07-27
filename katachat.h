#ifndef KATACHAT_H
#define KATACHAT_H

#include <iostream>
#include <vector>
#include <list>
#include <string>

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
    std::list<User *> _users;
    std::vector<UserPost> _board;

    std::ostream &_outStream;

    User    *findUser(std::string &UserName);
    User    *findUser(uint userId);

    void    print(std::string &poster, UserPost &post);

public:
    KataChat(std::ostream &out);

    //users management
    void    AddUser(std::string &userName);
    void    RemoveUser(std::string &userName);
    void    ListUsers();

    //operations
    uint    Post(std::string  &userName, std::string  msg);
    void    Follow(std::string  &follower, std::string  &leader);
    void    Unfollow(std::string  &follower, std::string  &leader);
    void    Read(std::string  &userName);
    void    Wall(std::string  &userName);
};

#endif // KATACHAT_H

