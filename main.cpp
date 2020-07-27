#include <iostream>
#include "katachat.h"

using namespace std;

/* courtesy ofstackoverflow */
vector<string> split(const string& str, int delimiter(int) = ::isspace){
  vector<string> result;
  auto e=str.end();
  auto i=str.begin();
  while(i!=e){
    i=find_if_not(i,e, delimiter);
    if(i==e) break;
    auto j=find_if(i,e, delimiter);
    result.push_back(string(i,j));
    i=j;
  }
  return result;
}

void Usage(ostream &out)
{
    out << "======================== .:: K A T A C H A T ::. ==============================" << endl
        << " valid commands are: " << endl
        << "'users' : shows active users list" << endl
        << "'[user] add' : creates a user with the given name"<< endl
        << "'[user] remove' : deletes the user wiht the given name" << endl
        << "'[user]' : shows the timeline for given user '" << endl
        << "'[user] -> message' : posts a message to the user timeline" << endl
        << "'[user] wall ': shows the user wall (incl.subrscriptions)" << endl
        << "'[user1] follows [user2]' : subscribes user1 to posts of user2" << endl
        << "'[user1] unfollows [user2]' : removes user1 from subscribers to posts of user2" << endl
        << "'quit': quits KataChat" << endl
        << "==============================================================================" << endl;
}

int main(int, char **)
{
    KataChat chat(cout);
    Usage(cout);

    chat.ListUsers();

    bool bChatRunning = true;

    string line;
    vector<std::string> lineItems;

    //the parsing loop (Yes! we could have used a more elegant solutions but we're down to the nitty-gritty here)
    while(bChatRunning)
    {
        getline(cin, line);
        lineItems = split(line);

        switch(lineItems.size())
        {
        case 1:
            //one item commands
            if(0 == lineItems[0].compare(USERLIST))
                chat.ListUsers();
            else if(0 == lineItems[0].compare(QUIT))
                bChatRunning = false;
            else
                chat.Read(lineItems[0]);
            break;

        case 2:
            //two items commands
            if (0 == lineItems[1].compare(WALL))
                chat.Wall(lineItems[0]);
            else if (0 == lineItems[1].compare(ADDUSER))
                chat.AddUser(lineItems[0]);
            else if (0 == lineItems[1].compare(REMOVEUSER))
                chat.RemoveUser(lineItems[0]);
            else
                Usage(cout);
            break;

        case 3:
            //3 items commands
            if(0 == lineItems[1].compare(FOLLOW))
            {
                chat.Follow(lineItems[0], lineItems[2]);
                break;
            }
            else if(0 == lineItems[1].compare(UNFOLLOW))
            {
                chat.Unfollow(lineItems[0], lineItems[2]);
                break;
            }
            //nobreak is ok as it will flowthrough on no match

        default:
            //more than 3 items, will accept only if it's a POST command of a message with spes in it
            int idx = line.find_first_of(POST);
            if(idx > 0)
                chat.Post(lineItems[0], line.substr(idx + sizeof(POST)));
            else
                Usage(cout);
            break;
        }
        cout << endl;
    }

    cout << "Thank your for playing .:: K A T A C H A T ::." << endl;

    return 0;
}
