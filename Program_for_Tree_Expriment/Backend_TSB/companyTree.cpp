#include "companyTree.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>
#include <cctype>
using namespace std;

std::filesystem::path absolutePath(const string & file){//将相对路径转换为绝对路径, 防止因工作路径改变导致文件无法打开
    char exePathBuf[MAX_PATH];
    GetModuleFileName(NULL,exePathBuf,MAX_PATH);
    std::filesystem::path exePath(exePathBuf);
    std::filesystem::path tempFilePath = exePath.parent_path() / file;
    //cout<<tempFilePath<<endl;
    return tempFilePath;
}
bool isAllDigits(const std::string & str){//判断字符串是否完全由数字构成
    if (str.empty()) return false;
    return std::all_of(str.begin(),str.end(),[](unsigned char c){return std::isdigit(c);});
}

//TSonNode
TSBNode::TSBNode(){
    root = false;
    leaf = false;
    name = "";
    num = -1;
    parent = std::weak_ptr<TSBNode>();
    nextBrother = nullptr;
    firstSon = nullptr;
    sonNum = 0;
}
TSBNode::TSBNode(std::string n){
    root = false;
    leaf = false;
    name = n;
    num = -1;
    parent = std::weak_ptr<TSBNode>();
    nextBrother = nullptr;
    firstSon = nullptr;
    sonNum = 0;
}
TSBNode::TSBNode(bool r,bool l,std::string n,int nu){
    root = r;
    leaf = l;
    name = n;
    num = nu;
    parent = std::weak_ptr<TSBNode>();
    nextBrother = nullptr;
    firstSon = nullptr;
    sonNum = 0;
}
TSBNode::~TSBNode(){}
TSBNode::TSBNode(const TSBNode & other){
    root = other.root;
    leaf = other.leaf;
    name = other.name;
    num = other.num;
    parent = other.parent;//由于是树的结点，所以这里采用浅拷贝
    nextBrother = other.nextBrother;
    firstSon = other.firstSon;
    sonNum = other.sonNum;
}
TSBNode & TSBNode::operator =(const TSBNode & other){
    if (this!=&other){
        root = other.root;
        leaf = other.leaf;
        name = other.name;
        num = other.num;
        parent = other.parent;//由于是树的结点，所以这里采用浅拷贝
        nextBrother = other.nextBrother;
        firstSon = other.firstSon;
        sonNum = other.sonNum;
    }
    return *this;
}
int TSBNode::getSonNum(){
    //if (leaf) cout<<"注意该结点为叶子结点!"<<endl;
    return sonNum;
}
void TSBNode::toLeaf(int nu){
    root = false;
    leaf = true;
    num = nu;
    firstSon = nullptr;
    sonNum = 0;
    return;
}
bool TSBNode::destroy(map <string, shared_ptr <TSBNode>> & allNode){
    bool destroySucc = selfDestroy(allNode);
    shared_ptr <TSBNode> pw = parent.lock();
    if (pw) destroySucc = destroySucc && pw->removeSon(shared_from_this());
    parent.reset();
    return destroySucc;
}
bool TSBNode::selfDestroy(map <string, shared_ptr <TSBNode>> & allNode){
    bool destroySucc = true;
    if (firstSon){
        auto curr = firstSon;
        while (curr){
            destroySucc = destroySucc && curr->selfDestroy(allNode);
            curr->parent.reset();
            curr = curr->nextBrother;
        }
        firstSon = nullptr;
    }
    sonNum = 0;
    allNode.erase(name);
    //cout<<"已删除: "<<name<<endl;
    return destroySucc;
}
bool TSBNode::removeSon(std::shared_ptr <TSBNode> son){
    if (!firstSon) return true;
    if (firstSon == son){
        auto toDelete = firstSon;
        firstSon = toDelete->nextBrother;
        sonNum -= 1;
        return true;
    }
    auto curr = firstSon;
    while (curr->nextBrother){
        if (curr->nextBrother == son){
            auto toDelete = curr->nextBrother;
            curr->nextBrother = toDelete->nextBrother;
            sonNum -= 1;
            return true;
        }
        curr = curr->nextBrother;
    }
    return false;
}
shared_ptr <TSBNode> TSBNode::deepCopy(){
    shared_ptr <TSBNode> newNode = make_shared <TSBNode>(root, leaf, name, num);
    newNode->sonNum = sonNum;
    if (firstSon){
        shared_ptr <TSBNode> newFirstSon = firstSon->deepCopy();
        newNode->firstSon = newFirstSon;
        newFirstSon->parent = newNode;
    }
    if (nextBrother) {
        shared_ptr <TSBNode> newNextBrother = nextBrother->deepCopy();
        newNode->nextBrother = newNextBrother;
        newNextBrother->parent = parent;
    }
    return newNode;
}
void TSBNode::addSon(std::shared_ptr <TSBNode> p){
    if (!firstSon){
        firstSon = p;
        p->parent = weak_from_this();
        sonNum = 1;
        return;
    }
    shared_ptr <TSBNode> pn = firstSon;
    while (pn->nextBrother){
        pn = pn->nextBrother;
    }
    pn->nextBrother = p;
    p->parent = weak_from_this();
    sonNum += 1;
    return;
}

//companyTree
std::shared_ptr <TSBNode> companyTree::searchNode(std::string n){
    return allNode[n];
}
int companyTree::getHeight(std::shared_ptr <TSBNode> p){
    //if (p->leaf) return 1;
    int maxHeight = 0;
    shared_ptr <TSBNode> curr = p->firstSon;
    while (curr){
        int h = getHeight(curr);
        if (h > maxHeight) maxHeight = h;
        curr = curr->nextBrother;
    }
    return 1 + maxHeight;
}
int companyTree::getGrandsonNum(std::shared_ptr <TSBNode> p){
    if (p->leaf) return 1;
        else{
            int num = 0;
            shared_ptr <TSBNode> curr = p->firstSon;
            while (curr){
                num += getGrandsonNum(curr);
                curr = curr->nextBrother;
            }
            return num;
        }
}
int companyTree::getAllSonNum(std::shared_ptr <TSBNode> p){
    int num = 1;
    shared_ptr <TSBNode> curr = p->firstSon;
    while (curr){
        num += getAllSonNum(curr);
        curr = curr->nextBrother;
    }
    return num;
}
int companyTree::getAllNum(std::shared_ptr <TSBNode> p){
    if (p->leaf) return p->num;
        else{
            int num = 0;
            shared_ptr <TSBNode> curr = p->firstSon;
            while (curr){
                num += getAllNum(curr);
                curr = curr->nextBrother;
            }
            return num;
        }
}
void companyTree::generalizedList(std::shared_ptr <TSBNode> p,int h,std::ostream & out){
    string table = "";
    for(int i=0;i<h;i++){
        table += "\t";
    }
    out<<table<<p->name;
    if (p->getSonNum()==0) out<<"()["<<p->num<<"]";
        else{
            out<<"("<<endl;
            shared_ptr <TSBNode> curr = p->firstSon;
            while (curr){
                generalizedList(curr,(h+1),out);
                curr = curr->nextBrother;
                if (curr) out<<","<<endl;
                    else out<<endl;
            }
            out<<table<<")";
        }
    return;
}
companyTree::companyTree(){
    allNode.emplace("TC公司", make_shared <TSBNode>(true,false,"TC公司",-1));
    employeeNum = 0;
    ifstream incompany(absolutePath("./company.txt"),ios::in);
    vector <istringstream> isss;
    for(string temp;getline(incompany,temp);){
        if (temp!="部门\t下级子部门或人数") isss.push_back(istringstream(temp));
    }
    incompany.close();
    map <shared_ptr <TSBNode>, string> nopare_son;
    for(int i=0;i<isss.size();i++){
        string name;
        string son_or_num;
        int nu = -1;
        string sonname = "";
        isss[i]>>name>>son_or_num;
        if (isAllDigits(son_or_num)){
            nu = stoi(son_or_num);
            shared_ptr <TSBNode> self = searchNode(name);
            self->toLeaf(nu);
            employeeNum += nu;
        }
            else{
                sonname = son_or_num;
                shared_ptr <TSBNode> son = make_shared <TSBNode>(false,false,sonname,nu);
                allNode.emplace(sonname,son);
                shared_ptr <TSBNode> pare = searchNode(name);
                if (pare) pare->addSon(son);
                    else{
                        //cout<<"找不到该结点的父结点!"<<endl;
                        nopare_son.emplace(son,name);
                    }
            }
    }
    while (!nopare_son.empty()){//处理第一次生成树时没有找到父结点的结点
        auto nopare = nopare_son.begin();
        while (nopare != nopare_son.end()){
            shared_ptr <TSBNode> son = nopare->first;
            shared_ptr <TSBNode> pare = searchNode(nopare->second);
            if (pare){
                pare->addSon(son);
                nopare = nopare_son.erase(nopare);
            }
                else ++nopare;
        }
    }
    cout<<"公司总部门数: "<<getNodeNum()<<endl;
    cout<<"公司员工总数: "<<employeeNum<<endl;
    cout<<"公司树的总高度: "<<getHeight()<<endl;
}
companyTree::~companyTree(){
    destroy();
}
companyTree::companyTree(const companyTree & other){
    employeeNum = other.employeeNum;
    height = other.height;
    if (!other.allNode.empty()) {
        shared_ptr <TSBNode> root = other.allNode.at("TC公司");
        if (root){
            shared_ptr <TSBNode> newRoot = root->deepCopy();
            allNode.emplace("TC公司", newRoot);
            for (const auto & pair : other.allNode) {
                string nodeName = pair.first;
                if (nodeName != "TC公司"){
                    shared_ptr <TSBNode> originalNode = pair.second;
                    shared_ptr <TSBNode> newNode = originalNode->deepCopy();
                    allNode.emplace(nodeName, newNode);
                }
            }
        }
    }
}
companyTree & companyTree::operator =(const companyTree & other){
    if (this!=&other){
        employeeNum = other.employeeNum;
        height = other.height;
        if (!other.allNode.empty()) {
            shared_ptr <TSBNode> root = other.allNode.at("TC公司");
            if (root){
                shared_ptr <TSBNode> newRoot = root->deepCopy();
                allNode.emplace("TC公司", newRoot);
                for (const auto & pair : other.allNode) {
                    string nodeName = pair.first;
                    if (nodeName != "TC公司"){
                        shared_ptr <TSBNode> originalNode = pair.second;
                        shared_ptr <TSBNode> newNode = originalNode->deepCopy();
                        allNode.emplace(nodeName, newNode);
                    }
                }
            }
        }
    }
    return *this;
}
int companyTree::getHeight(){
    shared_ptr <TSBNode> head = allNode["TC公司"];
    if (head) height = getHeight(allNode["TC公司"]);
        else height = 0;
    return height;
}
int companyTree::getSonNum(string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    if (p->leaf) cout<<"注意该结点为叶子结点!"<<endl;
    return p->getSonNum();
}
int companyTree::getGrandSonNum(string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getGrandsonNum(p);
}
int companyTree::getAllSonNum(std::string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getAllSonNum(p) - 1;
}
int companyTree::getAllNum(std::string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getAllNum(p);
}
std::shared_ptr <TSBNode> companyTree::getParent(std::string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return nullptr;
    }
    shared_ptr <TSBNode> pp = p->parent.lock();
    if (!pp){
        cout<<"该部门无父结点!"<<endl;
        return nullptr;
    }
    return pp;
}
int companyTree::getNodeNum(){
    return allNode.size();
}
int companyTree::getEmployeeNum(){
    shared_ptr <TSBNode> head = allNode["TC公司"];
    if (head) employeeNum = getAllNum(allNode["TC公司"]);
        else employeeNum = 0;
    return employeeNum;
}
int companyTree::getLeafNum(){
    shared_ptr <TSBNode> head = allNode["TC公司"];
    if (head) return getGrandsonNum(allNode["TC公司"]);
        else return 0;
}
bool companyTree::destroy(){
    shared_ptr <TSBNode> head = allNode["TC公司"];
    if (!head) return true;
    bool destroySucc = head->destroy(allNode);
    head.reset();
    allNode.clear();
    height = 0;
    employeeNum = 0;
    return destroySucc && !head;
}
bool companyTree::destroyTree(std::string n){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return true;
    }
    bool destroySucc = p->destroy(allNode);
    p.reset();
    height = getHeight();
    employeeNum = getEmployeeNum();
    return destroySucc && !p;
}
void companyTree::generalizedList(ostream & out){
    shared_ptr <TSBNode> head = allNode["TC公司"];
    if (!head){
        cout<<"树是空的!"<<endl;
        return;
    }
    generalizedList(head,0,out);
    out<<endl;
    return;
}
void companyTree::generalizedList(std::string n,std::ostream & out){
    shared_ptr <TSBNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return;
    }
    generalizedList(p,0,out);
    out<<endl;
    return;
}
void companyTree::outputGeneralizedList(std::string filename){
    for(int i=0;i<filename.size();i++){
        if (filename[i]==' '){
            cout<<"路径中不能包含空格!"<<endl;
            return;
        }
    }
    filename += "\\companyTree.txt";
    ofstream outfile(filename,ios::out);
    if (!outfile){
        cout<<"文件打开失败!"<<endl;
        return;
    }
    generalizedList(outfile);
    outfile.close();
    return;
}
void companyTree::outputGeneralizedList(std::string n,std::string filename){
    for(int i=0;i<filename.size();i++){
        if (filename[i]==' '){
            cout<<"路径中不能包含空格!"<<endl;
            return;
        }
    }
    filename += "\\" + n + "Tree.txt";
    ofstream outfile(filename,ios::out);
    if (!outfile){
        cout<<"文件打开失败!"<<endl;
        return;
    }
    generalizedList(n,outfile);
    outfile.close();
    return;
}

