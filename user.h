#pragma once

#ifndef USER_H
#define  USER_H

#include "iNode.h"

typedef long long ll;
#define USER_NAME_SIZE 32
#define USER_PASSWD_SIZE 16
#define BUFFER_SIZE 99

// User node
class User : public iNode
{
private:
    // string passwd_; // 用户密码
    ll passwd_; // 用户密码    
    set<string> joinedGroups_; // 该用户加入的组
public:
    User(string name, int passwd) : iNode(iNodeType::DIRECTORY, name, name, "2022.5.23") {
        passwd_ = passwd;
    }
    ll getPasswd() { return passwd_; }
    bool checkJoinedGroups(string groupName) {
        if (joinedGroups_.find(groupName) == joinedGroups_.end())
            return false;
        else 
            return true;
    }
};

#endif