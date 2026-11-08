#ifndef COMPANYTREE_H
#define COMPANYTREE_H
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class TSonNode: public std::enable_shared_from_this <TSonNode>{
    public:
    bool root;
    bool leaf;
    std::string name;
    int num;
    std::weak_ptr <TSonNode> parent;
    std::vector <std::shared_ptr <TSonNode>> sons;
    TSonNode();
    TSonNode(std::string n);
    TSonNode(bool r,bool l,std::string n,int nu);
    ~TSonNode();
    TSonNode(const TSonNode & other);//浅拷贝
    TSonNode & operator =(const TSonNode & other);//浅拷贝
    int getSonNum();
    void toLeaf(int nu);
    bool destroy();//销毁
    bool selfDestroy();
    bool removeSon(std::shared_ptr <TSonNode> son);
    std::shared_ptr <TSonNode> deepCopy();//深拷贝
};

class companyTree{
    protected:
    std::shared_ptr <TSonNode> head;
    int nodeNum;
    int height;
    int employeeNum;
    std::shared_ptr <TSonNode> searchNode(std::shared_ptr <TSonNode> p,std::string n);
    std::shared_ptr <TSonNode> searchNode(std::string n);
    int getHeight(std::shared_ptr <TSonNode> p);
    int getGrandsonNum(std::shared_ptr <TSonNode> p);
    int getAllSonNum(std::shared_ptr <TSonNode> p);
    int getAllNum(std::shared_ptr <TSonNode> p);
    void generalizedList(std::shared_ptr <TSonNode> p,int h = 0,std::ostream & out = std::cout);
    public:
    companyTree();
    ~companyTree();
    companyTree(const companyTree & other);//深拷贝
    companyTree & operator =(const companyTree & other);//深拷贝
    int getHeight();
    int getSonNum(std::string n);//求某个部门下属一级的子部门数
    int getGrandSonNum(std::string n);//某个部门的最低一级子部门数
    int getAllSonNum(std::string n);//求某个部门所有下属部门的个数
    int getAllNum(std::string n);//求某个部门的总人数
    std::shared_ptr <TSonNode> getParent(std::string n);//求某节点的父结点指针
    int getNodeNum();//求总结点数
    int getEmployeeNum();//求公司总员工数
    int getLeafNum();//求叶子结点总数
    bool destroy();//销毁整个树
    bool destroyTree(std::string n);//销毁子树(即删除整个部门)
    void generalizedList(std::ostream & out = std::cout);//打印树
    void generalizedList(std::string n,std::ostream & out = std::cout);//打印子树
    void outputGeneralizedList(std::string filename);//将树输出到文件中
    void outputGeneralizedList(std::string n,std::string filename);//将子树输出到文件中
};






#endif