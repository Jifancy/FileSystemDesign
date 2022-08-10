#pragma once

#ifndef FILESYSTEM_H
#define  FILESYSTEM_H

#include "iNode.h"
#include "group.h"
#include "user.h"

#include <vector>
#include <sstream>
// #include <conio.h>
#include <ctime>
#include <string.h>
#include <stdlib.h>

#define BLOCK_SIZE 10

const int TOTALBLOCKSIZE = 1000;

char block[TOTALBLOCKSIZE];

using std::stringstream;
using std::vector;

class  FileSystem
{
private:
    vector<int> prime; // 1e5内所有质数
    int A; // 当前选取的哈希值
    map<string, User*> users; // 文件系统中所有用户
    map<string, Group*> groups; // 文件系统中所有组 
    User* home; // 根目录
    set<int> freeBlock; // 文件系统中空闲分区块号
    map<string, vector<int>> fileContent; // 
    
    stringstream lineStream; // command 输入流
    string s[5]; // 命令解析

    string nowUser; // 当前用户
    string host = "@Ocean:";
    // 若在当前用户目录下， 则为~/...
    // 若在根目录下， 则为home/username/...
    // 若当前用户访问其他用户，则为/home/otherusername/...
    string absPath; // 绝对路径
    vector<iNode*> linkiNode; // 绝对路径中所包含的节点

    string helpCommand; // 命令提示
    
public: 
    // 构造函数，轻松所有数据
    FileSystem(); 

    // 初始化四个成员, Jifancy(Admin), Alice, Bob, Cindy
    // 在管理员组中的成员即为管理员
    // 管理员使用管理员句柄 sudo 实现管理员操作
    // Jifancy, Alice, Bob 目录下建立多级文件夹
    void linerPirme(); // 获取 1e5内的所有质数
    ll getHash(string); // 获取字符串哈希 
    string getPassword(); // 获取登录密码
    void allocationMemory(); // 
    void recoveryMemory(); // 
    string logFileSystem(); // 登录系统
    void initFileSystem(); // 初始化系统
    void runFileSystem(); // 运行系统
    void userAdd(string userName); // 添加用户
    void userDel(string userName); // 删除用户
    void userChange(string userName); // 改变当前用户
    void userMod(string userName, string perType); // 改变文件权限
    bool checkAdmin(); // 检查当前用户是否为管理员 
    bool checkPermission(string fileName); // 检查当前用户权限
    void groupAdd(string groupName); // 新建团队
    void groupDel(string groupName); // 删除团队
    void groupAddMember(string groupName, string userName); // 团队中新建成员
    void groupDelMember(string groupName, string userName); // 团队中删除成员
    void changeUppderFloder(); // 修改当前目录为上一级目录
    void changeLowerFloder(string floderName); // 修改当前目录为下一级目录
    void listG(); // list command -g parameter 展示当前系统所有组
    void listU(); // list command -u parameter 展示当前系统所有用户
};  

#endif