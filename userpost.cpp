#include "userpost.h"

UserPost::UserPost(QString txt, quint32 userId, quint32 postId):
    _text(txt),
    _user(userId),
    _id(postId)

{
    _postedTime = QDateTime::currentDateTime();
}
