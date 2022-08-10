#include "FileSystem.h"
#include "group.cpp"
#include "iNode.cpp"

FileSystem::FileSystem()
{
    linerPirme();

    home = new User("home", getHash("123456"));
    User* user = new User("Jifancy", getHash("123456"));  // 初始化inode
    iNode* nod = user->getiNode(); // 拿到user构造的inode节点指针
    users["Jifancy"] = user;  home->makeiNode("Jifancy", nod);
    user = new User("Alice", getHash("123456"));  
    nod = user->getiNode();
    users["Alice"] = user; home->makeiNode("Alice", nod);
    user = new User("Bob", getHash("123456")); 
    nod = user->getiNode();
    users["Bob"] = user; home->makeiNode("Bob", nod);
    user = new User("Cindy", getHash("123456")); 
    nod = user->getiNode();
    users["Cindy"] = user; home->makeiNode("Cindy", nod);

    groups["root"] = new Group("root");
    groups["root"]->addGroupMember("Jifancy");

    users["Jifancy"]->makeFloder("floder1", "Jifancy");
    users["Jifancy"]->touchFile("file1", "Jifancy");
    iNode* now = users["Jifancy"]->changeFloder("floder1");
    now->makeFloder("floder2", "Jifancy");
    now->touchFile("file1", "Jifancy");
    users["Alice"]->makeFloder("floder1", "Alice");
    users["Bob"]->makeFloder("floder1", "Bob");
}

void FileSystem::linerPirme()
{
    const int n = 1e5;
    
    vector<int> v(n+7, 0);
    for (int i = 2; i <= n; i ++) {
        if (v[i] == 0) { v[i] = i; prime.push_back(i); }
        int m = prime.size();
        for (int j = 0; j < m && i*prime[j] <= n; j ++) {
            v[i * prime[j]] = v[i];
            if (i % prime[j] == 0) break;
        }
    }
    srand(time(nullptr));

    int m = prime.size();

    auto random = [&m]() -> ll { 
        return (m-6000)+rand()%6000;
    };

    A = random();
}

ll FileSystem::getHash(string s)
{
    int n = s.length(), m = prime.size(), mod = 1e9+7;

    auto random = [&m]() -> ll { 
        return (m-6000)+rand()%6000;
    };

    ll res = 0;
    for (int i = 0; i < n; i ++) 
        res = (res * A + s[i]) % mod;

    return res;
}

string FileSystem::getPassword()
{
    string passwd;     
    system("stty -echo");

    char ch = ' ';// cin.clear();
    while (ch = getchar(), (ch == '\n' || ch == '\r'));
    while (ch) {
        //printf("|%d %d %d|", (int)ch, (int)'\n', (ch == '\n'));
        if (ch == '\n' || ch == '\r') break;
        else if (ch == 127) passwd = passwd.substr(0, passwd.length()-1);
        else passwd.append(1, ch);
        ch = getchar();
    }

    system("stty echo");

    return passwd;
}

void FileSystem::allocationMemory()
{
    iNode* nod = linkiNode.back()->changeFile(s[1]);
    nod->editFile(s[1]);

    // unique identification
    string t = absPath + s[1]; // absPaht + fileName

    for (auto &it : fileContent[t])
        freeBlock.insert(it);

    for (auto &it : s[1]) {
        block[*freeBlock.begin()] = it;
        freeBlock.erase(freeBlock.begin());
    }
}

void FileSystem::recoveryMemory()
{
    if (linkiNode.back()->removeFile(s[1])) {
        string t = absPath + s[1];
        for (auto &it : fileContent[t])
            freeBlock.insert(it);
    }
}


string FileSystem::logFileSystem()
{
    cin.clear(); string name;;
    
    while (true) {
        cout << "please input your username: ";
        cin >> name;
        if (users[name] == nullptr)
            cout << "The user does not exist in file system" << "\n"; 
        else 
            break;
    }

    while (true) {
        cout << "Enter your password: "; 
        string t = getPassword();
        ll passwd = getHash(t);
        if (users[name]->getPasswd() != passwd)
            cout << "your password is wrong" << "\n";
        else 
            break;
    }
    return name;
}

void FileSystem::initFileSystem()
{

    system("clear");

    for (int i = 1; i <= TOTALBLOCKSIZE; i ++)
        freeBlock.insert(i);

    cout << "  Welcome to Ocean file system. This is my Design :" << "\n";
    cout << "  A drop in the ocean" << "\n";
    cout << "  A change in the weather" << "\n";
    cout << "  I was praying that you and me might end up together" << "\n";
    cout << "                                     Author : Jifancy" << "\n";
    cout << "                                     Date : 2022.05.23" << "\n";

    helpCommand = "    USER RELATED COMMAND: \n" \
                    "[sudo] groupadd groupname ---- add group to file system  \n" \
                    "[sudo] groupdel groupname ---- del group from file system  \n" \
                	"[sudo] userdel username [-g] [groupname] ---- add user to file system | add user to group \n" \
                    "[sudo] useradd username [-g] [groupname] ---- delete user from file system | delete user from group\n" \
                    "userch username ---- change current log user \n" \
                    "usermod filename [public | private | groupname] ---- change permission of file and directory  \n" \
                    "    DIRECTORY RELATED COMMAND: \n" \
                    "mkdir flodername ---- make new directory\n" \
                    "rmdir flodername ---- delete exist directory \n" \
                    "cd [..] [flodername]  ---- change current directory \n"  \
                    "ls [-l -g -u]   ---- list all the file in now directory | all the groups || all the users \n" \
                    "    FILE RELATED COMMAND: \n" \
                    "touch filename ---- make new file \n" \
                    "rm filename ---- remove file from current directoty \n" \
                    "cat filename ---- concatentate file \n" \
                    "edit filename ---- edit file \n";

    nowUser = logFileSystem();
    host = "@Ocean: ";
    absPath = "/home/" + nowUser; 
    linkiNode.push_back(home);
    linkiNode.push_back(users[nowUser]);
}

void FileSystem::runFileSystem()
{

    initFileSystem();

    bool flag = false;

    while (true) 
    {
        if (flag) cout << nowUser + host + absPath + "$ ";
        lineStream.clear();
        cin.clear(); flag = true;
        string line; getline(cin, line);
        lineStream.str(line);
        s[0] = s[1] = s[2] = s[3] = s[4] = "";

        lineStream >> s[0];
        lineStream >> s[1];
        lineStream >> s[2];
        lineStream >> s[3];
        lineStream >> s[4];

        if (s[0] == "help") {
            cout << helpCommand << "\n";
        } else if (s[0] == "systeminfo") {
            // +++
            // linux 加入系统时间
            cout << "linux@Ocean : Jifancy " << "\n";
        } else if (s[0] == "exit") {
            // ++ 系统退出
            // system("exit");
            // 为什么 system 不行
            return;
        } else if (s[0] == "sudo") {
            if (!checkAdmin()) {
                cout << nowUser << " do not have the permission to run this command" << "\n";
                continue;
            }

            if (s[1] == "useradd") {
                if (s[2] != "" && s[3] == "-g" && s[4] != "" ) {
                    groupAddMember(s[2], s[4]);
                } else if (s[2] != ""){
                    userAdd(s[2]);
                } else {
                    cout << s[0] << ": " << s[1] << ": " << "command not found" << "\n";
                }
            } else if (s[1] == "userdel") {
                if (s[2] != "" && s[3] == "-g" && s[4] != "" ) {
                    groupDelMember(s[2], s[4]);
                } else if (s[2] != ""){
                    userDel(s[2]);
                } else {
                    cout << s[0] << ": " << s[1] << ": " << "command not found" << "\n";
                }
            } else if (s[1] == "groupadd") {
                groupAdd(s[2]);
            } else if (s[1] == "groupdel") {
                groupDel(s[2]);
            } else {
                cout << s[0] << ": " << s[1] << ": " << "command not found" << "\n";
            }
        } else if (s[0] == "userch") {
            userChange(s[1]);
        } else if (s[0] == "usermod") {
            userMod(s[1], s[2]);
        } else if (s[0] == "mkdir") {
            if (s[1] != "") {
                linkiNode.back()->makeFloder(s[1], nowUser);
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "rmdir") {
            if (s[1] != "") {
                linkiNode.back()->removeFloderRecursive(s[1]);
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "cd") {
            if (s[1] == "..") {
                changeUppderFloder();
            } else if (s[1] != "") {
                changeLowerFloder(s[1]);
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "ls") {
            if (s[1] == "") {
                linkiNode.back()->listFloder();
            } else if (s[1] == "-l") {
                linkiNode.back()->listFloderL();
            } else if (s[1] == "-g") {
                listG();
            } else if (s[1] == "-u") {
                listU();
            }  else {
                cout << s[0] << ": " << "invalid option " << s[1] << "\n";
            }
        } else if (s[0] == "touch") {
            if (s[1] != "") {
                linkiNode.back()->touchFile(s[1], nowUser);
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "rm") {
            if (s[1] != "") {
                recoveryMemory();
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "cat") {
            if (s[1] != "") {
                if (checkPermission(s[1])) {
                    iNode* nod = linkiNode.back()->changeFile(s[1]);
                    nod->concatenateFile(s[1]);
                }
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "cp") {
            if (s[1] != "" && s[2] != "") {
                if (checkPermission(s[1])) {
                    iNode* nod = linkiNode.back()->changeFile(s[1]);
                    nod->copyFile(s[1], s[2], nowUser);
                }
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } else if (s[0] == "edit") {
            if (s[1] != "") {
                if (checkPermission(s[1])) {
                    // +++
                    allocationMemory();
                }
            } else {
                cout << s[0] << ": " << "missing operand" << "\n";
            }
        } 
    }
}

void FileSystem::userAdd(string userName)
{
    // 若已经存在用户
    if (users[userName] != nullptr) {
        cout << "user" << userName << " already exists" << "\n";
        return; 
    }

    // 当前用户不存在， 需要为其设置密码
    cout << "please set password for " << userName << ": \n"; 
    cin.clear(); ll passwd = getHash(getPassword());

    User *user = new User(userName, passwd);
    iNode* nod = user->getiNode();
    home->makeiNode(userName, nod);
    users[userName] = user;
}

void FileSystem::userDel(string userName)
{
    if (users[userName] == nullptr)
        { cout << "user " << userName << " does not exist" << "\n"; return ;}
    home->removeFloderRecursive(userName);
    users.erase(userName);
    // 考虑用户组删除

    for (auto [f, s] : groups) 
        s->delGroupMember(userName);
}

void FileSystem::userChange(string userName)
{
    if (users[userName] == nullptr)
        { cout << "user " << userName << " does not exist" << "\n"; return; }
    
    ll passwd; 
    if (!checkAdmin()) {
        while (true) {
            cout << "please enter password for " << userName << ": ";
            passwd = getHash(getPassword());
            if (passwd == users[userName]->getPasswd())
                break;
            cout << "\n";
        }
        cout << endl;
    }
    
    nowUser = userName;
    // 修改当前用户路径
    linkiNode.clear(); 
    linkiNode.push_back(home); 
    linkiNode.push_back(users[userName]);
    absPath = "/home/" + userName;
}

void FileSystem::userMod(string fileName, string perType)
{
    iNode* nod = linkiNode.back()->changeFile(fileName);
    if (nod == nullptr) {
        cout << fileName << " does not exist" << "\n";
        return;
    }

    if (perType == "public") nod->changeModel(MODEL::PUBLIC, "public");
    else if (perType == "private") nod->changeModel(MODEL::PRIVATE, "private");
    else {
        if (groups[perType] == nullptr) {
            cout << "file system does not exist group " << perType << "\n";
            return;
        }
        nod->changeModel(MODEL::GROUP, perType);
    }
}

bool FileSystem::checkAdmin()
{
    if (groups["root"]->checkMember(nowUser))
        return true;
    else 
        return false;
}

bool FileSystem::checkPermission(string fileName)
{
    iNode* nod = linkiNode.back()->changeFile(fileName);
    if (nod == nullptr) {
        cout << "No such file in current directory\n";
        return false; 
    }

    if (checkAdmin())
        return true;

    // 0123456
    // frwrwrw
    string per = nod->getPermission();
    if (per[5]=='r' && per[6]=='w') {
        return true;
    } else if (per[3]=='r' && per[4]=='w') {
        string fileOwner = nod->getOwner();
        for (auto &[f, group] : groups) {
            if (group->checkMember(fileOwner) && group->checkMember(nowUser))
                return true;
        }
        cout << "The current user does not have access to this file\n";
        return false;
    } else if (per[1]=='r' && per[2]=='w') {
        cout << "The current user does not have access to this file\n";
        return false;
    }
    return false;
}

void FileSystem::groupAdd(string groupName)
{
    if (groups[groupName] != nullptr) {
        cout << "group " << groupName << " already exists" << "\n";
        return; 
    }

    groups[groupName] = new Group(groupName);
    groups[groupName]->addGroupMember(nowUser);
}

void FileSystem::groupDel(string groupName)
{
    if (groups[groupName] == nullptr) {
        cout << "group " << groupName << " does not exist" << "\n";
        return;
    }   

    if (groupName == "root") {
        cout << "anyone do not have the permission to delete root";
        return;
    }

    groups.erase(groupName);
}

void FileSystem::groupAddMember(string userName, string groupName)
{
    if (groups[groupName] == nullptr) {
        cout << "group " << groupName << " does not exists" << "\n";
        return; 
    }
    if (users[userName] == nullptr) {
        cout << "user " << userName << "does not exists\n";
        return;
    }

    groups[groupName]->addGroupMember(userName);
}

void FileSystem::groupDelMember(string userName, string groupName)
{
    if (groups[groupName]->checkMember(userName)) {
        cout << "group " << groupName << "already exists " << userName << "\n";
        return;
    }

    groups[groupName]->delGroupMember(userName);
}

void FileSystem::changeUppderFloder()
{
    // 当在根目录时不允许返回上一级目录
    if (linkiNode.size() <= 1)
        return;
    while (absPath.back() != '/') absPath.pop_back();
    absPath.pop_back();
    linkiNode.pop_back();
}

void FileSystem::changeLowerFloder(string floderName)
{
    iNode* nod = linkiNode.back()->changeFloder(floderName);
    if (nod == nullptr) {
        cout << "The folder does not exist in the current directory\n";
        return;
    }

    absPath += "/" + nod->getName();
    linkiNode.push_back(nod);
}

void FileSystem::listG()
{
    for (auto [f, s] : groups)
        cout << f << " ";
    cout << "\n";
}

void FileSystem::listU()
{
    for (auto [f, s] : users)
        cout << f << " ";
    cout << "\n";
}

signed main()
{
    FileSystem *filesystem = new FileSystem();

    filesystem->runFileSystem();

    return 0;
}