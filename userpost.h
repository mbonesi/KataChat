#ifndef USERPOSTS_H
#define USERPOSTS_H

#include <time.h>
//in order to be fully compiler agnostic you should deal with this include and ifx all the errors (will try on Microsoft tho)
#include <QObject>

class UserPost
{
    time_t _postedTime;

    std::string _text;
    uint _user;
    uint _id;

public:
    UserPost() = default;
    UserPost(std::string txt, uint userId, uint postId);
    const std::string &GetText() {return _text;}
    uint GetOwner() {return _user;}
    uint GetId() {return _id;}

    long long  SecondsSinceEpoch() {return _postedTime;}
};


#endif // USERPOSTS_H
