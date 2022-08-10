#pragma once

#ifndef GROUP_H
#define GROUP_H

#include <iostream>
#include <string>
#include <set>

using std::string;
using std::set;
using std::cout;
using std::cin;
using std::endl;

class Group
{
private:
    string name_; // 组名
    set<string> groupMember; // 组内成员 
public:
    Group() {}
    Group(string name) : name_(name) { groupMember.clear(); }
    string getName() { return name_; } // 返回当前组名
    void addGroupMember(string userName); // 添加组内成员
    void delGroupMember(string userName); // 删除组内成员
    bool checkMember(string userName); // 查看当前组中有无指定成员
};

#endif