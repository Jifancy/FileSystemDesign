#include "group.h"

void Group::addGroupMember(string userName)
{
    // if (groupMember.find(userName) != groupMember.end()) 
    //     cout << userName << " is already in this group" << endl;
    // else 
    groupMember.insert(userName);
}

void Group::delGroupMember(string userName)
{
    // if (groupMember.find(userName) == groupMember.end())
    //     cout << userName << " is not exist int this group" << endl;
    // else 
    groupMember.erase(userName);
}

bool Group::checkMember(string userName)
{
    if (groupMember.find(userName) == groupMember.end())
        return false;
    else 
        return true; 
}