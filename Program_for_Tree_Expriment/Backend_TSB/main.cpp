#include "companyTree.h"

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;

std::filesystem::path absolutePath(const string & file);
void testTree(companyTree & ct);
void saveCmd(string & cmd);

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    companyTree ct;
    //testTree(ct);
    string cmd = "";
    while (getline(cin,cmd)){
        istringstream iss(cmd);
        vector <string> row;
        for(string temp;getline(iss,temp,' ');){
            row.push_back(temp);
        }
        if ((row.size()>=2)&&(row[0]=="PTE")){
            saveCmd(cmd);
            if (row[1]=="exit") break;
                else if ((row.size()>=3)&&(row[1]=="display")){
                    if (row[2]=="total"){
                        ct.generalizedList(cout);
                        continue;
                    }
                        else{
                            ct.generalizedList(row[2],cout);
                            continue;
                        }
                }
                else if ((row.size()>=4)&&(row[1]=="output")){
                    if (row[2]=="total"){
                        ct.outputGeneralizedList(row[3]);
                        continue;
                    }
                        else{
                            ct.outputGeneralizedList(row[2],row[3]);
                            continue;
                        }
                }
                else if ((row.size()>=3)&&(row[1]=="sonnum")){
                    cout<<row[2]<<"的下属一级的子部门数为: "<<ct.getSonNum(row[2])<<endl;
                    continue;
                }
                else if ((row.size()>=3)&&(row[1]=="grandsonnum")){
                    cout<<row[2]<<"的最低一级的子部门数为: "<<ct.getGrandSonNum(row[2])<<endl;
                    continue;
                }
                else if ((row.size()>=3)&&(row[1]=="allsonnum")){
                    cout<<row[2]<<"的下属总部门数为: "<<ct.getAllSonNum(row[2])<<endl;
                    continue;
                }
                else if ((row.size()>=3)&&(row[1]=="allnum")){
                    cout<<row[2]<<"的总人数为: "<<ct.getAllNum(row[2])<<endl;
                    continue;
                }
                else if ((row.size()>=3)&&(row[1]=="parent")){
                    shared_ptr <TSBNode> p = ct.getParent(row[2]);
                    if (p) cout<<row[2]<<"的上一级部门为: "<<p->name<<endl;
                    continue;
                }
                else if (row[1]=="nodenum"){
                    cout<<"总结点数为: "<<ct.getNodeNum()<<endl;
                    continue;
                }
                else if (row[1]=="employeenum"){
                    cout<<"公司总员工为: "<<ct.getEmployeeNum()<<endl;
                    continue;
                }
                else if (row[1]=="leafnum"){
                    cout<<"叶子结点数为: "<<ct.getLeafNum()<<endl;
                    continue;
                }
                else if ((row.size()>=3)&&(row[1]=="delete")){
                    if (row[2]=="total"){
                        if (ct.destroy()) cout<<"已将树销毁"<<endl;
                            else cout<<"销毁树失败!"<<endl;
                    }
                        else{
                            if (ct.destroyTree(row[2])) cout<<"已将子树销毁"<<endl;
                                else cout<<"销毁子树失败!"<<endl;
                        }
                    continue;
                }
                else{
                    cout<<"\""<<row[1]<<"\" is an unknown command."<<endl;
                }
        }
            else if (row[0]=="exit") break;
            else{
                cout<<"\""<<row[0]<<"\" is not an internal command."<<endl;
                continue;
            }
    }
    return 0;
}

void testTree(companyTree & ct){
    cout<<ct.getSonNum("互动娱乐")<<endl;
    cout<<ct.getGrandSonNum("业务部")<<endl;
    cout<<ct.getAllNum("互动娱乐")<<endl;
    cout<<ct.getEmployeeNum()<<endl;
    cout<<ct.getLeafNum()<<endl;
    cout<<ct.getNodeNum()<<endl;
    ct.generalizedList();
    if (ct.destroy()) cout<<"已将树销毁"<<endl;
    return;
}
void saveCmd(string & cmd){
    ofstream outcmd(absolutePath(".\\cmd.txt"),ios::app);
    if (!outcmd){
        cout<<"指令未保存!"<<endl;
        return;
    }
    outcmd<<cmd<<endl;
    outcmd.close();
    return;
}