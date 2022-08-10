#include "iNode.h"

void iNode::changeModel(MODEL mod, string groupName)
{
    string per = "d------";

    if (mod == MODEL::PRIVATE) 
        per = "drw----";
    else if (mod == MODEL::GROUP) 
        per = "drwrw--";
    else 
        per = "drwrwrw";
    
    if (this->type_ == iNodeType::FILE) per[0] = 'f';
    permission_ = per;
    group_ = groupName;
}

string iNode::getSystemTime()
{

    FILE *fp = popen("date","r");
    int ch; 

    string s = "";
    while((ch = fgetc(fp)) != EOF) 
        s += ch;
    
    if (s[s.length()-1] == '\n') s.pop_back();

    return s;
}

void iNode::makeFloder(string floderName, string owner)
{
    this->dirTree_[floderName] = new iNode(iNodeType::DIRECTORY, floderName, owner, getSystemTime());
}

void iNode::removeFloderRecursive(string floderName)
{
    if (this->dirTree_[floderName] == nullptr) {
        cout << "No such floder in current directory\n";
        return;
    }

    function<void(iNode*&)> dfs = [&](iNode *&now) {
        if (now->dirTree_.empty()) {
            now->fileTree_.clear();
            return;
        }

        for (auto &[name, son] : now->dirTree_)
            dfs(son);

        now->dirTree_.clear();
        now->fileTree_.clear();
    };

    dfs(this->dirTree_[floderName]);
    this->dirTree_.erase(floderName);
}

iNode* iNode::changeFloder(string floderName)
{
    if (dirTree_.count(floderName) == 0)
        return nullptr;
    return dirTree_[floderName];
}

iNode* iNode::changeFile(string fileName)
{
    if (fileTree_.count(fileName) == 0)
        return nullptr;

    return fileTree_[fileName];
}

void iNode::listFloder()
{
    cout << "DIRECTORY: ";
    for (auto [name, son] : this->dirTree_)
        cout << name << " ";
    cout << "\n";
    cout << "FILE: ";
    for (auto [name, son] : this->fileTree_)
        cout << name << " ";
    cout << "\n";
}

void iNode::listFloderL()
{
    for (auto [name, son] : this->dirTree_)
        cout << son->getPermission() << " " << name << " " << son->getOwner() << " " << son->getLastUpdateTime() << "\n";
    for (auto [name, son] : this->fileTree_)
        cout << son->getPermission() << " " << name << " " << son->getOwner() << " " << son->getLastUpdateTime() << "\n";
}

void iNode::touchFile(string fileName, string owner)
{
    fileTree_[fileName] = new iNode(iNodeType::FILE, fileName, owner, getSystemTime());
}

bool iNode::removeFile(string fileName)
{
    if (fileTree_.find(fileName) == fileTree_.end()) {
        cout << "No such file in current directory\n";
        return false;
    }

    fileTree_.erase(fileName);
    return true;
}

void iNode::concatenateFile(string fileName)
{
    // if (fileTree_[fileName] == nullptr) {
    //     cout << "No such file in current directory\n";
    //     return;
    // }

    // cout << (fileTree_[fileName]->content_) << "\n";

    cout << this->content_ << "\n";
}

void iNode::copyFile(string fileName1, string fileName2, string owner)
{
    fileTree_[fileName2] = new iNode(iNodeType::FILE, fileName2, owner, getSystemTime());
    fileTree_[fileName2]->content_ = fileTree_[fileName1]->content_;
}

void iNode::editFile(string fileName)
{
    cout << this->content_;
    cin.clear();
    string extra; cin >> extra;

    char ch = ' ';
    while (ch = getchar()) {
        if (ch == 1) break;
        else if (ch == 127) {
            cout << "\b \b";
            extra = extra.substr(0, extra.length()-1);
        } else 
            extra += ch;
    }

    if (ch == 1 || ch == 10) ch = getchar();

    lastUpdateTime_ = getSystemTime();
    this->content_ += extra;
}

