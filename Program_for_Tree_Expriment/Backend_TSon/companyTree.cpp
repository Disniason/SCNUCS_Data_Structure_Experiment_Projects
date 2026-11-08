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
TSonNode::TSonNode(){
    root = false;
    leaf = false;
    name = "";
    num = -1;
    parent = std::weak_ptr<TSonNode>();
    sons.clear();
}
TSonNode::TSonNode(std::string n){
    root = false;
    leaf = false;
    name = n;
    num = -1;
    parent = std::weak_ptr<TSonNode>();
    sons.clear();
}
TSonNode::TSonNode(bool r,bool l,std::string n,int nu){
    root = r;
    leaf = l;
    name = n;
    num = nu;
    parent = std::weak_ptr<TSonNode>();
    sons.clear();
}
TSonNode::~TSonNode(){}
TSonNode::TSonNode(const TSonNode & other){
    root = other.root;
    leaf = other.leaf;
    name = other.name;
    num = other.num;
    parent = other.parent;//由于是树的结点，所以这里采用浅拷贝
    sons = other.sons;
}
TSonNode & TSonNode::operator =(const TSonNode & other){
    if (this!=&other){
        root = other.root;
        leaf = other.leaf;
        name = other.name;
        num = other.num;
        parent = other.parent;//由于是树的结点，所以这里采用浅拷贝
        sons = other.sons;
    }
    return *this;
}
int TSonNode::getSonNum(){
    //if (leaf) cout<<"注意该结点为叶子结点!"<<endl;
    return sons.size();
}
void TSonNode::toLeaf(int nu){
    root = false;
    leaf = true;
    num = nu;
    for(auto & son : sons) {
        son->parent.reset();
    }
    sons.clear();
    return;
}
bool TSonNode::destroy(){
    bool destroySucc = selfDestroy();
    shared_ptr <TSonNode> pw = parent.lock();
    if (pw) destroySucc = destroySucc && pw->removeSon(shared_from_this());
    return destroySucc;
}
bool TSonNode::selfDestroy(){
    bool destroySucc = true;
    for(auto & son : sons){
        destroySucc = destroySucc && son->selfDestroy();
        son->parent.reset();
    }
    sons.clear();
    return destroySucc;
}
bool TSonNode::removeSon(std::shared_ptr <TSonNode> son){
    auto it = std::find(sons.begin(), sons.end(), son);
    if (it != sons.end()){
        sons.erase(it);
        return true;
    }
    return false;
}
shared_ptr <TSonNode> TSonNode::deepCopy(){
    shared_ptr <TSonNode> newNode = make_shared <TSonNode>(root, leaf, name, num);
    for(const auto & son : sons){
        shared_ptr <TSonNode> newSon = son->deepCopy();
        newSon->parent = newNode;
        newNode->sons.push_back(newSon);
    }
    return newNode;
}

//companyTree
std::shared_ptr <TSonNode> companyTree::searchNode(std::shared_ptr <TSonNode> p,std::string n){
    if (!p) return p;
    if (p->name == n) return p;
        else{
            for(int i=0;i<p->getSonNum();i++){
                shared_ptr <TSonNode> ps = searchNode(p->sons[i],n);
                if (ps) return ps;
            }
        }
    return nullptr;
}
std::shared_ptr <TSonNode> companyTree::searchNode(std::string n){
    return searchNode(head,n);
}
int companyTree::getHeight(std::shared_ptr <TSonNode> p){
    //if (p->leaf) return 1;
    int maxHeight = 0;
    for(int i=0;i<p->getSonNum();i++){
        int h = getHeight(p->sons[i]);
        if (h>maxHeight) maxHeight = h;
    }
    return 1 + maxHeight;
}
int companyTree::getGrandsonNum(std::shared_ptr <TSonNode> p){
    if (p->leaf) return 1;
        else{
            int num = 0;
            for(int i=0;i<p->getSonNum();i++){
                num += getGrandsonNum(p->sons[i]);
            }
            return num;
        }
}
int companyTree::getAllSonNum(std::shared_ptr <TSonNode> p){
    int num = 1;
    for(int i=0;i<p->getSonNum();i++){
        num += getAllSonNum(p->sons[i]);
    }
    return num;
}
int companyTree::getAllNum(std::shared_ptr <TSonNode> p){
    if (p->leaf) return p->num;
        else{
            int num = 0;
            for(int i=0;i<p->getSonNum();i++){
                num += getAllNum(p->sons[i]);
            }
            return num;
        }
}
void companyTree::generalizedList(std::shared_ptr <TSonNode> p,int h,std::ostream & out){
    string table = "";
    for(int i=0;i<h;i++){
        table += "\t";
    }
    out<<table<<p->name;
    if (p->getSonNum()==0) out<<"()["<<p->num<<"]";
        else{
            out<<"("<<endl;
            for(int i=0;i<p->getSonNum();i++){
                generalizedList(p->sons[i],(h+1),out);
                if (i==(p->getSonNum() - 1)) out<<endl;
                    else out<<","<<endl;
            }
            out<<table<<")";
        }
    return;
}
companyTree::companyTree(){
    head = make_shared <TSonNode>(true,false,"TC公司",-1);
    nodeNum = 1;
    employeeNum = 0;
    ifstream incompany(absolutePath("./company.txt"),ios::in);
    vector <istringstream> isss;
    for(string temp;getline(incompany,temp);){
        if (temp!="部门\t下级子部门或人数") isss.push_back(istringstream(temp));
    }
    incompany.close();
    map <shared_ptr <TSonNode>,string> nopare_son;
    for(int i=0;i<isss.size();i++){
        string name;
        string son_or_num;
        int nu = -1;
        string sonname = "";
        isss[i]>>name>>son_or_num;
        if (isAllDigits(son_or_num)){
            nu = stoi(son_or_num);
            shared_ptr <TSonNode> self = searchNode(name);
            self->toLeaf(nu);
            employeeNum += nu;
        }
            else{
                sonname = son_or_num;
                shared_ptr <TSonNode> son = make_shared <TSonNode>(false,false,sonname,nu);
                nodeNum += 1;
                shared_ptr <TSonNode> pare = searchNode(name);
                if (pare){
                    pare->sons.push_back(son);
                    son->parent = pare;
                }
                    else{
                        cout<<"找不到该结点的父结点!"<<endl;
                        nopare_son.emplace(son,name);
                    }
            }
    }
    while (!nopare_son.empty()){//处理第一次生成树时没有找到父结点的结点
        auto nopare = nopare_son.begin();
        while (nopare != nopare_son.end()){
            shared_ptr <TSonNode> son = nopare->first;
            shared_ptr <TSonNode> pare = searchNode(nopare->second);
            if (pare){
                pare->sons.push_back(son);
                son->parent = pare;
                nopare = nopare_son.erase(nopare);
            }
                else ++nopare;
        }
    }
    cout<<"公司总部门数: "<<nodeNum<<endl;
    cout<<"公司员工总数: "<<employeeNum<<endl;
    cout<<"公司树的总高度: "<<getHeight()<<endl;
}
companyTree::~companyTree(){
    destroy();
}
companyTree::companyTree(const companyTree & other){
    if (other.head) head = other.head->deepCopy();
        else head = nullptr;
    nodeNum = other.nodeNum;
    height = other.height;
    employeeNum = other.employeeNum;
}
companyTree & companyTree::operator =(const companyTree & other){
    if (this!=&other){
        companyTree temp(other);
        swap(head,temp.head);
        swap(nodeNum,temp.nodeNum);
        swap(height,temp.height);
        swap(employeeNum,temp.employeeNum);
    }
    return *this;
}
int companyTree::getHeight(){
    height = getHeight(head);
    return height;
}
int companyTree::getSonNum(string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    if (p->leaf) cout<<"注意该结点为叶子结点!"<<endl;
    return p->getSonNum();
}
int companyTree::getGrandSonNum(string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getGrandsonNum(p);
}
int companyTree::getAllSonNum(std::string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getAllSonNum(p) - 1;
}
int companyTree::getAllNum(std::string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return 0;
    }
    return getAllNum(p);
}
std::shared_ptr <TSonNode> companyTree::getParent(std::string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return nullptr;
    }
    shared_ptr <TSonNode> pp = p->parent.lock();
    if (!pp){
        cout<<"该部门无父结点!"<<endl;
        return nullptr;
    }
    return pp;
}
int companyTree::getNodeNum(){
    nodeNum = getAllSonNum(head);
    return nodeNum;
}
int companyTree::getEmployeeNum(){
    employeeNum = getAllNum(head);
    return employeeNum;
}
int companyTree::getLeafNum(){
    return getGrandsonNum(head);
}
bool companyTree::destroy(){
    if (!head) return true;
    bool destroySucc = head->destroy();
    head->parent.reset();
    head.reset();
    nodeNum = 0;
    height = 0;
    employeeNum = 0;
    return destroySucc && !head;
}
bool companyTree::destroyTree(std::string n){
    shared_ptr <TSonNode> p = searchNode(n);
    if (!p){
        cout<<"没有找到该部门!"<<endl;
        return true;
    }
    bool destroySucc = p->destroy();
    p->parent.reset();
    p.reset();
    nodeNum = getNodeNum();
    height = getHeight();
    employeeNum = getEmployeeNum();
    return destroySucc && !p;
}
void companyTree::generalizedList(ostream & out){
    if (!head){
        cout<<"树是空的!"<<endl;
        return;
    }
    generalizedList(head,0,out);
    out<<endl;
    return;
}
void companyTree::generalizedList(std::string n,std::ostream & out){
    shared_ptr <TSonNode> p = searchNode(n);
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

