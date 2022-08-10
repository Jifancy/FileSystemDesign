#ifndef INODE_H
#define INODE_H

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <functional>
#include <vector>

enum class iNodeType{ FILE, DIRECTORY };
enum class MODEL {PRIVATE, GROUP, PUBLIC };

#define PERMISSION_SIZE

using std::string;
using std::map;
using std::set;
using std::cout;
using std::cin;
using std::endl;
using std::function;
using std::vector;

class iNode 
{
private:
    iNodeType type_; // inode type
    string name_; // inode name
    string owner_; // inode owner name
    string group_; // inode 所属组
    string lastUpdateTime_; // last modification time 
    // a directory iNode : d rw rw rw // 私有权限，团队权限，其他人权限
    // a file iNode : f rw rw rw 
    // 详细阐述：对于管理员而言可以访问文件系统任何成员的所有文件, 无视当前文件任何权限
    // 对于普通用户而言，可以访问当前团队内普通成员的共享文件
    // 但无法访问当前同一团队普通成员的私有文件
    string permission_; // permission of reading or writing this file or directory
private: // directory
    map<string, iNode*> fileTree_;
    map<string, iNode*> dirTree_;
private: // file
    string content_; // 保存文件内容，后续扩展为FCB (file control block)
    set<int> block; // 
public:
    iNode() {};
    iNode(iNodeType type, string name, string owner, string lastUpdateTime) :
        type_(type), name_(name), owner_(owner), lastUpdateTime_(lastUpdateTime) {
            if (type == iNodeType::DIRECTORY) {
                fileTree_.clear();
                dirTree_.clear();
                permission_ = "drw----";
            } else if (type == iNodeType::FILE) {
                permission_ = "frw----";
                content_ = "";
            }
        }
    iNode* getiNode() { return this; } // 返回父类指针
    void makeiNode(string name, iNode *&inod) { dirTree_[name] = inod; } // 利用inode节点建立目录
    iNodeType getType() {return type_; } // 获取当前文件类型
    string getName() { return name_; } // 获取文件名称
    string getOwner() { return owner_; } // 获取文件所属人
    string getPermission() { return permission_; } // 获取文件权限
    string getLastUpdateTime() { return lastUpdateTime_; } // 获取文件上次修改时间
    string getContent() { return content_; } // return file content
    // chmod, change model to private, group, public
    void changeModel(MODEL mod, string groupName); 
    string getSystemTime(); // get now system time
    // 目录相关操作 
public:
    // mkdir : contructor delfalt private directory
    void makeFloder(string floderName, string owner);
    // rmdir : -r parameters
    bool floderEmpty() { return dirTree_.empty() && fileTree_.empty(); }
    void removeFloderRecursive(string floderName);
    // cd .. operate out of this class
    // cd filename 注意权限问题，普通用户无法进入私有目录
    iNode* changeFloder(string floderName);
    iNode* changeFile(string flieName); // 切入至当前文件
    // ls : -l parameters
    void listFloder();
    void listFloderL();
    // 当前文件夹下有无特定文件
// 文件相关操作
public:
    // touch filename, defalut private file
    void touchFile(string fileName, string owner);
    // rm filename
    bool removeFile(string fileName);
    // cat filename  
    void concatenateFile(string fileName);
    // copy filename1 to filename2
    void copyFile(string fileName1, string fileName2, string owner);
    // edit filename
    void editFile(string fileName);
};

#endif