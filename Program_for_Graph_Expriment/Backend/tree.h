#ifndef TREE_H
#define TREE_H
#pragma once

/*
    轻量级树结构实现，对接图的最小生成树输出功能
*/

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <cstddef>

namespace pge{

    class treeNode{
        public:
        std::string nodeName;//结点名
        std::vector <std::shared_ptr <pge::treeNode>> children;//孩子指针数组

        treeNode(const std::string & name):nodeName(name){
            children.clear();
        }
        ~treeNode(){}

    };

    class tree{
        protected:
        std::unordered_map <std::string,std::shared_ptr <pge::treeNode>> allNodes;//顶点表
        size_t undirectedEdgeNum;//无向边的数量

        void output(std::shared_ptr <pge::treeNode> nodePtr,const std::string & table,std::unordered_set <std::string> & visited,std::ostream & out = std::cout){
            if (!nodePtr) return;
            out<<table<<nodePtr->nodeName;
            visited.insert(nodePtr->nodeName);
            std::vector <std::shared_ptr <pge::treeNode>> childs;
            for(const auto & child : nodePtr->children){
                if (visited.find(child->nodeName) == 0) childs.push_back(child);
            }
            if (childs.empty()) out<<"()";
            else{
                out<<"("<<std::endl;
                for(const auto & child : childs){
                    output(child,table + "\t",visited,out);
                    if (child == childs.back()) out<<std::endl;
                    else out<<","<<std::endl;
                }
                out<<table<<")";
            }
            return;
        }

        public:
        tree(){
            allNodes.clear();
            undirectedEdgeNum = 0;
        }
        tree(const std::string & rootName){
            allNodes.clear();
            allNodes.emplace(rootName,std::make_shared <pge::treeNode>(rootName));
            undirectedEdgeNum = 0;
        }
        ~tree(){
            for(auto & nodeIt : allNodes){
                nodeIt.second->children.clear();
            }
            allNodes.clear();
            undirectedEdgeNum = 0;
        }

        bool addNode(const std::string & parentName,const std::string & childName){
            auto parentIt = allNodes.find(parentName);
            if (parentIt == allNodes.end()){
                std::cerr<<"未找到父结点 "<<parentName<<std::endl;
                return false;
            }
            if (allNodes.find(childName) != allNodes.end()){
                std::cerr<<"已存在同名结点 "<<childName<<std::endl;
                return false;
            }
            auto childPtr = std::make_shared <pge::treeNode>(childName);
            parentIt->second->children.push_back(childPtr);
            allNodes.emplace(childName,childPtr);
            return true;
        }

        bool addNode(const std::string & nodeName){
            if (allNodes.find(nodeName) != allNodes.end()){
                std::cerr<<"已存在同名结点 "<<nodeName<<std::endl;
                return false;
            }
            auto nodePtr = std::make_shared <pge::treeNode>(nodeName);
            allNodes.emplace(nodeName,nodePtr);
            return true;
        }

        //删除结点，但是不销毁整个子树，因此会形成森林
        bool removeNode(const std::string & childName){
            auto childIt = allNodes.find(childName);
            if (childIt == allNodes.end()){
                std::cerr<<"未找到子结点 "<<childName<<std::endl;
                return false;
            }
            for(auto & nodeIt : allNodes){
                auto & children = nodeIt.second->children;
                children.erase(
                    std::remove_if(
                        children.begin(),
                        children.end(),
                        [&](const std::shared_ptr <pge::treeNode> & childPtr){
                            return childPtr->nodeName == childName;
                        }
                    ),
                    children.end()
                );
            }
            allNodes.erase(childIt);
            return true;
        }

        bool addEdge(const std::string & parentName,const std::string & childName){
            auto parentIt = allNodes.find(parentName);
            if (parentIt == allNodes.end()){
                std::cerr<<"未找到父结点 "<<parentName<<std::endl;
                return false;
            }
            auto childIt = allNodes.find(childName);
            if (childIt == allNodes.end()){
                std::cerr<<"未找到子结点 "<<childName<<std::endl;
                return false;
            }
            auto childPtr = allNodes.find(childName)->second;
            auto parentPtr = allNodes.find(parentName)->second;
            parentIt->second->children.push_back(childPtr);
            childIt->second->children.push_back(parentPtr);
            undirectedEdgeNum += 1;
            return true;
        }

        void output(const std::string & rootName,std::ostream & out = std::cout){
            auto rootIt = allNodes.find(rootName);
            if (rootIt == allNodes.end()){
                std::cerr<<"未找到根结点 "<<rootName<<std::endl;
                return;
            }
            std::unordered_set <std::string> visited;
            while (visited.size() < allNodes.size()){
                output(rootIt->second,"",visited,out);
                out<<std::endl;
                while (true){
                    ++rootIt;
                    if (rootIt == allNodes.end()) break;
                    if (visited.find(rootIt->first) == visited.end()) break;
                }
            }
            return;
        }

        size_t size(){
            return allNodes.size();
        }

        size_t getUndirectedEdgeNum(){
            return undirectedEdgeNum;
        }
    };


}











#endif