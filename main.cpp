#include <QVector>

#include "katachat.h"

void Usage(QTextStream &out)
{
    out << "======================== .:: K A T A C H A T ::. ==============================" << endl
        << " valid commands are: " << endl
        << "'users' : shows active users list" << endl
        << "'[user] add' : creates a user with the given name"<< endl
        << "'[user] remove' : deletes the user with the given name" << endl
        << "'[user]' : shows the timeline for given user '" << endl
        << "'[user] -> message' : posts a message to the user timeline" << endl
        << "'[user] wall ': shows the user wall (own plus subscribed users' content)" << endl
        << "'[user1] follows [user2]' : subscribes user1 to timeline of user2" << endl
        << "'[user1] unfollows [user2]' : removes user1 from subscribers to timeline of user2" << endl
        << "'quit': quits KataChat" << endl
        << "==============================================================================" << endl;
}

int main(int argc, char *argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QTextStream in(stdin);
    QTextStream out(stdout);

    KataChat chat(out);
    Usage(out);

    chat.ListUsers();

    bool bChatRunning = true;

    QString line;
    QStringList lineItems;

    //the parsing loop (Yes! we could have used a more elegant solutions but we're down to the nitty-gritty here)
    while(bChatRunning)
    {
        line = in.readLine();
        lineItems = line.split(" "); //we spearate each field by a space

        switch(lineItems.length())
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
                Usage(out);
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
            int idx = line.indexOf(POST);
            if(idx > 0)
                chat.Post(lineItems[0], line.right(line.count() - (idx + sizeof(POST))));
            else
                Usage(out);
            break;
        }
        out << endl;
    }
    /*
     * Yes, the "Command" design patterm would have showcased more elegance at a cost (desing and coding time)
     */


    out << "Thank your for playing .:: K A T A C H A T ::." << endl;

    return 0;
}
