#include "userpost.h"

#define DATETIME_FORMAT "hh:mm:ss"

UserPost::UserPost(std::string txt, uint userId, uint postId):
    _text(txt),
    _user(userId),
    _id(postId)

{
    _postedTime = time(0);
}
