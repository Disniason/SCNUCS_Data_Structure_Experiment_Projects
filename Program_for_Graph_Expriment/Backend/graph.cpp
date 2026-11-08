#include "graph.h"
#include "tree.h"
#include "disjoint_set.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <memory>
#include <cstddef>
#include <limits>

namespace pge{

    node::node(const std::string & name,bool ie):nodeName(name),isExit(ie){
        neighbors.clear();
    }

    node::node(const pge::node & other){
        nodeName = other.nodeName;
        isExit = other.isExit;
        neighbors = other.neighbors;
    }

    node::~node(){}

    pge::node & node::operator =(const pge::node & other){
        if (this != &other){
            nodeName = other.nodeName;
            isExit = other.isExit;
            neighbors = other.neighbors;
        }
        return *this;
    }

    void node::addNeighbor(const pge::edge & e){
        neighbors.push_back(e);
        return;
    }

    void node::removeNeighbor(const std::string & node_name,std::unordered_set <std::string> & allEdgeNames){
        for (auto it = neighbors.begin();it != neighbors.end();){
            if (it->neighborNode->nodeName == node_name){
                allEdgeNames.erase(it->edgeName);
                it = neighbors.erase(it);
            }
            else  ++it;
        }
        return;
    }

    void node::removeEdge(const std::string & name){
        neighbors.remove_if([&](const pge::edge & e){
            return e.edgeName == name;
        });
        return;
    }

    size_t node::getDegree(){
        return neighbors.size();
    }

    long long node::getDistance(const std::string & endNodeName,const std::string & name){
        if (this->nodeName == endNodeName) return 0;
        for(const auto & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name) return neighbor.distance;
        }
        return std::numeric_limits <long long>::max();
    }

    long long node::getCongestionLevel(const std::string & endNodeName,const std::string & name){
        for(const auto & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name) return neighbor.congestionLevel;
        }
        return std::numeric_limits <long long>::max();
    }

    long long node::getSecurityRiskLevel(const std::string & endNodeName,const std::string & name){
        for(const auto & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name) return neighbor.securityRiskLevel;
        }
        return std::numeric_limits <long long>::max();
    }

    bool node::setDistance(const std::string & endNodeName,const std::string & name,long long d){
        for(pge::edge & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name){
                neighbor.distance = d;
                return true;
            }
        }
        std::cerr<<"该点不存在用边 "<<name<<" 连接的邻接结点 "<<endNodeName<<std::endl;
        return false;
    }

    bool node::setCongestionLevel(const std::string & endNodeName,const std::string & name,long long c){
        for(pge::edge & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name){
                neighbor.congestionLevel = c;
                return true;
            }
        }
        std::cerr<<"该点不存在用边 "<<name<<" 连接的邻接结点 "<<endNodeName<<std::endl;
        return false;
    }

    bool node::setSecurityRiskLevel(const std::string & endNodeName,const std::string & name,long long s){
        for(pge::edge & neighbor : neighbors){
            if (neighbor.neighborNode->nodeName == endNodeName && neighbor.edgeName == name){
                neighbor.securityRiskLevel = s;
                return true;
            }
        }
        std::cerr<<"该点不存在用边 "<<name<<" 连接的邻接结点 "<<endNodeName<<std::endl;
        return false;
    }

    /*std::string node::findNeighborName(const std::string & edgeName){
        for(const auto & neighbor : neighbors){
            if (neighbor.edgeName == edgeName && neighbor.neighborNode) return neighbor.neighborNode->nodeName;
        }
        return "";
    }*/

    void node::output_structure(const std::string & table,std::ostream & out) const{
        out<<table<<"{"<<std::endl;
        out<<table<<"\t\"node name\": \""<<nodeName<<"\","<<std::endl;
        out<<table<<"\t\"is exit\": "<<(isExit ? "true" : "false")<<","<<std::endl;
        if (neighbors.empty()) out<<table<<"\t\"neighbors\": []"<<std::endl;
        else{
            out<<table<<"\t\"neighbors\": ["<<std::endl;
            std::string edgeTable = table + "\t\t";
            for(const auto & neightbor : neighbors){
                neightbor.output_structure(edgeTable,out);
                if (neightbor == neighbors.back()) out<<std::endl;
                else out<<","<<std::endl;
            }
            out<<table<<"\t]"<<std::endl;
        }
        out<<table<<"}";
        return;
    }

    long long node::getMinDistance(const std::string &endNodeName,std::string & edgeName,bool(*filter)(const pge::edge&))
    {   
        long long min=std::numeric_limits<long long>::max();
        if(filter==NULL){
            for(const auto & neighbor : neighbors){
                if(neighbor.neighborNode->nodeName==endNodeName&&neighbor.distance<min){
                    min=neighbor.distance;
                    edgeName=neighbor.edgeName;
                }
            }
        }
        else{
            for(const auto & neighbor : neighbors){
                if(filter(neighbor)&&neighbor.neighborNode->nodeName==endNodeName&&neighbor.distance<min){
                    min=neighbor.distance;
                    edgeName=neighbor.edgeName;
                }
            }
        }
        return min;
    }




    edge::edge(const std::string & name,const std::string & startName,std::shared_ptr <pge::node> n,long long d,long long c,long long s):
        edgeName(name),
        startNodeName(startName),
        neighborNode(n),
        distance(d),
        congestionLevel(c),
        securityRiskLevel(s){}
    
    edge::edge(const pge::edge & other){
        edgeName = other.edgeName;
        startNodeName = other.startNodeName;
        neighborNode = other.neighborNode;
        distance = other.distance;
        congestionLevel = other.congestionLevel;
        securityRiskLevel = other.securityRiskLevel;
    }
    
    edge::~edge(){}

    pge::edge & edge::operator =(const pge::edge & other){
        if (this != &other){
            edgeName = other.edgeName;
            startNodeName = other.startNodeName;
            neighborNode = other.neighborNode;
            distance = other.distance;
            congestionLevel = other.congestionLevel;
            securityRiskLevel = other.securityRiskLevel;
        }
        return *this;
    }

    bool operator ==(const pge::edge & a,const pge::edge & b){
        return (
            (a.edgeName == b.edgeName) &&
            (a.distance == b.distance) &&
            (a.congestionLevel == b.congestionLevel) &&
            (a.securityRiskLevel == b.securityRiskLevel)
        );
    }

    bool operator <(const pge::edge & a,const pge::edge & b){
        if (a.securityRiskLevel < b.securityRiskLevel) return true;
        else if (a.securityRiskLevel > b.securityRiskLevel) return false;
        else{
            if (a.distance < b.distance) return true;
            else if (a.distance > b.distance) return false;
            else{
                if (a.congestionLevel < b.congestionLevel) return true;
                else return false;
            }
        }
    }

    bool operator >(const pge::edge & a,const pge::edge & b){
        return b < a;
    }

    void edge::output_structure(const std::string & table,std::ostream & out) const{
        out<<table<<"{"<<std::endl;
        out<<table<<"\t\"edge name\": \""<<edgeName<<"\","<<std::endl;
        out<<table<<"\t\"start node name\": \""<<startNodeName<<"\","<<std::endl;
        out<<table<<"\t\"neighbor node name\": \""<<(neighborNode ? neighborNode->nodeName : "null")<<"\","<<std::endl;
        out<<table<<"\t\"distance\": "<<distance<<","<<std::endl;
        out<<table<<"\t\"congestion level\": "<<congestionLevel<<","<<std::endl;
        out<<table<<"\t\"security risk level\": "<<securityRiskLevel<<std::endl;
        out<<table<<"}";
        return;
    }
    /*
    bool edgeComparator::operator ()(const pge::edge & a,const pge::edge & b) const{
        if (a.securityRiskLevel < b.securityRiskLevel) return true;
        else if (a.securityRiskLevel > b.securityRiskLevel) return false;
        else{
            if (a.distance < b.distance) return true;
            else if (a.distance > b.distance) return false;
            else{
                if (a.congestionLevel < b.congestionLevel) return true;
                else return false;
            }
        }
    }*/



    bool graph::createNode(const std::string & name,bool ie){
        if (nodes.find(name) == nodes.end()){
            nodes.emplace(name,std::make_shared <pge::node>(name,ie));
            return true;
        }
        else return false;
    }

    bool graph::createEdge(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d,long long c,long long s){
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()) return false;
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()) return false;
        if (allEdgeNames.find(name) != allEdgeNames.end()) return false;
        startIt->second->addNeighbor(pge::edge(name,startNodeName,endIt->second,d,c,s));
        endIt->second->addNeighbor(pge::edge(name,endNodeName,startIt->second,d,c,s));
        allEdgeNames.insert(name);
        return true;
    }

    void graph::output_kruskal_minimum_spanning_tree(std::ostream & out){
        std::priority_queue <pge::edge,std::vector <pge::edge>,std::greater <pge::edge>> pq;//将大顶堆变为小顶堆
        pge::disjoint_set <std::string> ds;//自定义并查集
        pge::tree mst;//自定义树类
        for(const auto & nodeIt : nodes){//初始化小顶堆、并查集和树类
            for(const auto & neighbor : nodeIt.second->neighbors){
                pq.push(neighbor);
            }
            ds.push(nodeIt.first);
            mst.addNode(nodeIt.first);
        }
        while (mst.getUndirectedEdgeNum() < nodes.size()-1 && pq.size() > 0){
            pge::edge minEdge = pq.top();
            pq.pop();
            if (!minEdge.neighborNode) continue;
            std::string startNodeName = minEdge.startNodeName;
            std::string endNodeName = minEdge.neighborNode->nodeName;
            if (!ds.isConnected(startNodeName,endNodeName)){
                ds.merge(startNodeName,endNodeName);
                mst.addEdge(startNodeName,endNodeName);
            }
        }
        mst.output(nodes.begin()->first,out);//树结构输出
        return;
    }

    void graph::depth_first_search_helper(const std::string & nodeName,std::unordered_set <std::string> & visited,std::vector <std::string> & nodeNames){
        visited.insert(nodeName);
        nodeNames.push_back(nodeName);
        auto nodeIt = nodes.find(nodeName);
        if (nodeIt == nodes.end() || !nodeIt->second) return;
        const auto & nodeNeighbors = nodeIt->second->neighbors;
        for(const auto & neighbor : nodeNeighbors){
            if (!neighbor.neighborNode) continue;
            std::string neighborName = neighbor.neighborNode->nodeName;
            if (visited.find(neighborName) == visited.end()) depth_first_search_helper(neighborName,visited,nodeNames);
        }
        return;
    }

    void graph::output_structure(const std::string & table,std::ostream & out) const{
        out<<table<<"{"<<std::endl;
        out<<table<<"\t\"graph name\": \""<<graphName<<"\","<<std::endl;
        if (nodes.empty()) out<<table<<"\t\"nodes\": []"<<std::endl;
        else{
            out<<table<<"\t\"nodes\": ["<<std::endl;
            std::string nodeTable = table + "\t\t";
            size_t count = 0;
            for(const auto & nodeIt : nodes){
                if (nodeIt.second){
                    nodeIt.second->output_structure(nodeTable,out);
                    if (count >= nodes.size() - 1) out<<std::endl;
                    else out<<","<<std::endl;
                }
                count += 1;
            }
            out<<table<<"\t]"<<std::endl;
        }
        out<<table<<"}"<<std::endl;
        return;
    }

    void graph::get_emergency_all_path_helper(
        const std::string & startNodeName,
        const std::string & endNodeName,
        pge::graphPath path,
        std::vector <pge::graphPath> & paths,
        bool(*filter)(const pge::edge &)
    ){
        if (startNodeName == endNodeName){
            paths.push_back(path);
            return;
        }
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()){
            std::cerr<<"该结点不在图中: "<<startNodeName<<std::endl;
            return;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()){
            std::cerr<<"该结点不在图中: "<<endNodeName<<std::endl;
            return;
        }
        for(const pge::edge & neighbor : startIt->second->neighbors){
            if (!neighbor.neighborNode) continue;
            const std::string & nextNodeName = neighbor.neighborNode->nodeName;
            if (filter(neighbor) && path.visitedNodeNames.find(nextNodeName) == path.visitedNodeNames.end()) {
                pge::graphPath newPath = path; 
                newPath.addEdge(neighbor);
                get_emergency_all_path_helper(nextNodeName,endNodeName,newPath,paths,filter);
            }
        }
        return;
    }

    const std::string graph::generateEdgeName(const std::string &startNodeName, const std::string &endNodeName)
    {
        return startNodeName+"_"+endNodeName;
    }

    

    graph::graph(const std::string & name):graphName(name){
        dataPath = ".\\data";
        nodes.clear();
        allEdgeNames.clear();
    }

    graph::graph(const pge::graph & other){
        dataPath = other.dataPath;
        graphName = other.graphName;
        nodes = other.nodes;
        allEdgeNames = other.allEdgeNames;
    }

    graph::~graph(){
        outputFile_structure(dataPath,pge::encodeChinese(graphName));
    }

    pge::graph & graph::operator =(const pge::graph & other){
        if (this != &other){
            dataPath = other.dataPath;
            graphName = other.graphName;
            nodes = other.nodes;
            allEdgeNames = other.allEdgeNames;
        }
        return *this;
    }

    bool graph::addNode(const std::string & name,bool ie){
        if (nodes.find(name) != nodes.end()){
            std::cerr<<"已存在同名结点!"<<std::endl;
            return false;
        }
        nodes.emplace(name,std::make_shared <pge::node>(name,ie));
        return true;
    }

    bool graph::removeNode(const std::string & nodeName){
        auto removeIt = nodes.find(nodeName);
        if (removeIt == nodes.end()){
            std::cerr<<"未找到结点 "<<nodeName<<std::endl;
            return false;
        }
        for(auto & nodeIt : nodes){
            nodeIt.second->removeNeighbor(nodeName,allEdgeNames);
        }
        nodes.erase(removeIt);
        return true;
    }

    bool graph::addEdge(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d,long long c,long long s){
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()){
            std::cerr<<"未找到结点: "<<startNodeName<<std::endl;
            return false;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()){
            std::cerr<<"未找到结点: "<<endNodeName<<std::endl;
            return false;
        }
        if (allEdgeNames.find(name) != allEdgeNames.end()){
            std::cerr<<"已存在同名边: "<<name<<std::endl;
            return false;
        }
        if (startNodeName == endNodeName) d = 0;
        startIt->second->addNeighbor(pge::edge(name,startNodeName,endIt->second,d,c,s));
        endIt->second->addNeighbor(pge::edge(name,endNodeName,startIt->second,d,c,s));
        // hjb 添加边之后将边的名字存入allEdgeNames，避免插入重名边，名字作为边的唯一id
        allEdgeNames.insert(name);
        return true;
    }

    bool graph::removeEdge(const std::string & startNodeName,const std::string & endNodeName,const std::string & name){
        if (allEdgeNames.find(name) == allEdgeNames.end()){
            std::cerr<<"未找到边: "<<name<<std::endl;
            return false;
        }
        auto startIt = nodes.find(startNodeName);
        if (startIt != nodes.end()) startIt->second->removeEdge(name);
        else{
            std::cerr<<"未找到结点 "<<startNodeName<<std::endl;
            return false;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt != nodes.end()) endIt->second->removeEdge(name);
        else{
            std::cerr<<"未找到结点 "<<endNodeName<<std::endl;
            return false;
        }
        allEdgeNames.erase(name);
        return true;
    }

    long long graph::getDistance(const std::string & startNodeName,const std::string & endNodeName,const std::string & name){
        auto startIt = nodes.find(startNodeName);
        if (startIt != nodes.end()) return startIt->second->getDistance(endNodeName,name);
        else{
            std::cerr<<"未找到结点 "<<startNodeName<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    long long graph::getCongestionLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name){
        auto startIt = nodes.find(startNodeName);
        if (startIt != nodes.end()) return startIt->second->getCongestionLevel(endNodeName,name);
        else{
            std::cerr<<"未找到结点 "<<startNodeName<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    long long graph::getSecurityRiskLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name){
        auto startIt = nodes.find(startNodeName);
        if (startIt != nodes.end()) return startIt->second->getSecurityRiskLevel(endNodeName,name);
        else{
            std::cerr<<"未找到结点 "<<startNodeName<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    long long graph::getDistance(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name){
        if (startNodePtr && endNodePtr) return startNodePtr->getDistance(endNodePtr->nodeName,name);
        else{
            std::cerr<<"结点指针无效!"<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    long long graph::getCongestionLevel(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name){
        if (startNodePtr && endNodePtr) return startNodePtr->getCongestionLevel(endNodePtr->nodeName,name);
        else{
            std::cerr<<"结点指针无效!"<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    long long graph::getSecurityRiskLevel(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name){
        if (startNodePtr && endNodePtr) return startNodePtr->getSecurityRiskLevel(endNodePtr->nodeName,name);
        else{
            std::cerr<<"结点指针无效!"<<std::endl;
            return std::numeric_limits<long long>::max();
        }
    }

    bool graph::setDistance(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d){
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()){
            std::cerr<<"该边的始点不存在!"<<std::endl;
            return false;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()){
            std::cerr<<"改变的终点不存在!"<<std::endl;
            return false;
        }
        return startIt->second->setDistance(endNodeName,name,d) && endIt->second->setDistance(startNodeName,name,d);
    }

    bool graph::setCongestionLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long c){
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()){
            std::cerr<<"该边的始点不存在!"<<std::endl;
            return false;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()){
            std::cerr<<"改变的终点不存在!"<<std::endl;
            return false;
        }
        return startIt->second->setCongestionLevel(endNodeName,name,c) && endIt->second->setCongestionLevel(startNodeName,name,c);
    }

    bool graph::setSecurityRiskLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long s){
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end()){
            std::cerr<<"该边的始点不存在!"<<std::endl;
            return false;
        }
        auto endIt = nodes.find(endNodeName);
        if (endIt == nodes.end()){
            std::cerr<<"改变的终点不存在!"<<std::endl;
            return false;
        }
        return startIt->second->setSecurityRiskLevel(endNodeName,name,s) && endIt->second->setSecurityRiskLevel(startNodeName,name,s);
    }
    
    void graph::getAllExit(std::vector <std::shared_ptr <pge::node>> & exitNodes){
        exitNodes.clear();
        for(const auto & nodeIt : nodes){
            if (nodeIt.second && nodeIt.second->isExit) exitNodes.push_back(nodeIt.second);
        }
        return;
    }

    void graph::displayAllExit(std::ostream & out){
        size_t num = 0;
        for(const auto & nodeIt : nodes){
            if (nodeIt.second && nodeIt.second->isExit){
                out<<nodeIt.second->nodeName<<std::endl;
                num += 1;
            }
        }
        if (num == 0) out<<"没有出口!"<<std::endl;
        return;
    }

    size_t graph::getNodeNum(){
        return nodes.size();
    }

    void graph::setDataPath(const std::string & path){
        dataPath = path;
        return;
    }
    
    std::string graph::getDataPath(){
        return dataPath;
    }

    void graph::setGraphName(const std::string & name){
        graphName = name;
        return;
    }

    std::string graph::getGraphName(){
        return graphName;
    }

    void graph::clear(){
        nodes.clear();
        allEdgeNames.clear();
        return;
    }

    void graph::display_minimum_spanning_tree(std::ostream & out){
        output_kruskal_minimum_spanning_tree(out);
        return;
    }

    bool graph::outputFile_minimum_spanning_tree(const std::string & fileName){
        std::ofstream outFile(fileName,std::ios::out);
        if (!outFile.is_open()){
            std::cerr<<"无法打开文件 "<<fileName<<std::endl;
            return false;
        }
        output_kruskal_minimum_spanning_tree(outFile);
        outFile.close();
        return true;
    }

    void graph::breadth_first_search(const std::string & startNodeName,std::vector <std::string> & nodeNames){
        nodeNames.clear();
        if (nodes.find(startNodeName) == nodes.end()){
            std::cerr<<"没有找到结点 "<<startNodeName<<std::endl;
            return;
        }
        std::unordered_map <std::string,bool> visited;
        std::queue <std::string> q;
        q.push(startNodeName);
        visited[startNodeName] = true;
        while (!q.empty()){
            std::string nodeName = q.front();
            q.pop();
            nodeNames.push_back(nodeName);
            auto nodeIt = nodes.find(nodeName);
            if (nodeIt == nodes.end() || !nodeIt->second) continue;
            const auto & nodeNeighbors = nodeIt->second->neighbors;
            for(const auto & neighbor : nodeNeighbors){
                if (!neighbor.neighborNode) continue;
                std::string neighborName = neighbor.neighborNode->nodeName;
                if (!visited[neighborName]){
                    q.push(neighborName);
                    visited[neighborName] = true;
                }
            }
        }
    }

    void graph::depth_first_search(const std::string & startNodeName,std::vector <std::string> & nodeNames){
        nodeNames.clear();
        auto startIt = nodes.find(startNodeName);
        if (startIt == nodes.end() || !startIt->second){
            std::cerr<<"没有找到结点 "<<startNodeName<<std::endl;
            return;
        }
        std::unordered_set <std::string> visited;
        depth_first_search_helper(startNodeName,visited,nodeNames);
        return;
    }

    void graph::display_structure(std::ostream & out){
        output_structure("",out);
        return;
    }

    bool graph::outputFile_structure(const std::string & filePath,const std::string & fileName){
        std::filesystem::path pathFilePath(filePath);
        std::filesystem::path pathFileName(fileName + ".json");
        std::filesystem::path fullPath = pathFilePath / pathFileName;
        std::ofstream outFile(fullPath,std::ios::out);
        if (!outFile.is_open()){
            std::cerr<<"无法打开文件 "<<fullPath<<std::endl;
            return false;
        }
        output_structure("",outFile);
        outFile.close();
        return true;
    }

    bool graph::input_structure(const std::string & filePath){
        if (filePath.substr(filePath.size()-5,5) != ".json"){
            std::cerr<<filePath<<" 不是一个合法的json文件!"<<std::endl;
            return false;
        }
        std::ifstream in(std::filesystem::path(filePath),std::ios::in);
        if (!in.is_open()){
            std::cerr<<"无法打开文件 "<<filePath<<std::endl;
            return false;
        }
        std::vector <std::string> lines;
        for(std::string line; std::getline(in,line);){
            lines.push_back(line);
        }
        in.close();
        if ((lines.size() >= 1 && lines[1].substr(0,14) != "\t\"graph name\":") || lines.size() < 3){
            std::cerr<<"该json文件不合法!"<<std::endl;
            return false;
        }
        this->clear();
        for(size_t i=0;i<lines.size();i++){
            if (lines[i].substr(0,14) == "\t\"graph name\":"){
                graphName = getReference(lines[i],16);
                continue;
            }
            else if (lines[i] == "\t\"nodes\": ["){
                size_t j = i + 1;
                for(;lines[j]!="\t]";j++){
                    if (lines[j] == "\t\t{"){
                        size_t k = j + 1;
                        for(;lines[k].substr(0,3) != "\t\t}";k++){
                            if (lines[k].substr(0,15) == "\t\t\t\"node name\":"){
                                std::string nodeName = getReference(lines[k],17);
                                bool isExit = lines[k+1][14] == 't';
                                createNode(nodeName,isExit);
                                continue;
                            }
                            else if (lines[k] == "\t\t\t\"neighbors\": ["){
                                size_t l = k + 1;
                                for(;lines[l] != "\t\t\t]";l++){
                                    if (lines[l] == "\t\t\t\t{"){
                                        std::string edgeName = getReference(lines[l+1],19);
                                        std::string startNodeName = getReference(lines[l+2],25);
                                        std::string neighborNodeName = getReference(lines[l+3],28);
                                        long long distance = std::stoll(lines[l+4].substr(17,lines[l+4].size()-18));
                                        long long congestionLevel = std::stoll(lines[l+5].substr(25,lines[l+4].size()-26));
                                        long long securityRiskLevel = std::stoll(lines[l+6].substr(28,lines[l+4].size()-28));
                                        createEdge(startNodeName,neighborNodeName,edgeName,distance,congestionLevel,securityRiskLevel);
                                        //std::cout<<edgeName<<' '<<startNodeName<<' '<<neighborNodeName<<' '<<distance<<' '<<congestionLevel<<' '<<securityRiskLevel<<std::endl;
                                        l += 6;
                                        continue;
                                    }
                                }
                                k = l - 1;
                                continue;
                            }
                            else if (lines[k] == "\t\t\t\"neighbors\": []"){
                                continue;
                            }
                        }
                        j = k - 1;
                    }
                    else if (lines[j] == "\t\t]") break;
                }
                i = j - 1;
                break;
            }
            else if (lines[i] == "\t\"nodes\": []"){
                continue;
            }
        }
        return true;
    }

    void graph::get_dijkstra_normal_shortest_path(const std::string & startNodeName, const std::string & endNodeName, std::vector <std::tuple <std::string,std::string,std::string>> & path)
    //最后一个参数的顺序，起点名，终点名，边名
    {
        struct info{//结构体，包含了完成此函数需要的信息，dist：当前最短距离，finished：最短路径是否已求出，path：路径信息
            long long dist;
            bool finished;
            std::pair <std::string,std::string> path;//第一个string最短路径中前一个节点的名称，第二个string储存边的名称来明确是哪条边
        };
        //辅助实现优先级队列选权最小的
        struct distNamePair{
            long long dist;
            std::string nodeName;
            distNamePair(const long long & dist,const std::string & nodeName):dist(dist),nodeName(nodeName){};
        };
        struct cmp {
            bool operator() (const distNamePair &x,distNamePair &y) {
                return x.dist>y.dist;
            }
        };//end 辅助实现优先级队列选权最小的
        if (nodes.find(startNodeName) == nodes.end()){
            std::cerr<<"源节点不存在"<<std::endl;
            return;
        }
        if(nodes.find(endNodeName) == nodes.end()){
            std::cerr<<"终点节点不存在"<<std::endl;
        }
        std::unordered_map<std::string,info> nodeInfo;
        long long w,min;
        std::priority_queue <distNamePair,std::vector <distNamePair>,cmp> pq;
        long long maxWeight = std::numeric_limits<long long>::max();
        for(const auto & nodeIt : nodes){
            std::string edgeName;
            nodeInfo[nodeIt.first] = info();
            nodeInfo[nodeIt.first].dist = nodes[startNodeName]->getMinDistance(nodeIt.first,edgeName,[](const pge::edge& e)->bool{return true;});
            nodeInfo[nodeIt.first].finished = false;
            if(nodeIt.first != startNodeName && nodeInfo[nodeIt.first].dist<maxWeight){
                nodeInfo[nodeIt.first].path = std::make_pair(startNodeName,edgeName);
                //辅助实现优先级队列选权最小的
                pq.push(distNamePair(nodeInfo[nodeIt.first].dist,nodeIt.first));
                //end 辅助实现优先级队列选权最小的
            }
            else{
                nodeInfo[nodeIt.first].path = std::make_pair(std::string(),std::string());
            }
        }
        nodeInfo[startNodeName].finished = true;
        nodeInfo[startNodeName].dist = 0;
        size_t n = this->getNodeNum();
        for(size_t i=0;i<n;i++){
            min=std::numeric_limits <long long>::max();
            std::string curMinNodeName = startNodeName;//这样初始化，如果没有新的边，下面做的事情相当于没有更新
            //这里可以用优先队列进行优化，每次更新的时候直接把新的距离和顶点的名字加进去，然后判断finished=true的就跳过，直到找到finished=false的第一个
            // for(const auto & nodeIt : nodes){
            //     if(nodeInfo[nodeIt.first].finished==false&&nodeInfo[nodeIt.first].dist<min){curMinNodeName=nodeIt.first;min=nodeInfo[nodeIt.first].dist;}
            // }
            //优先级队列做法
            while(!pq.empty()){
                distNamePair cur=pq.top();
                pq.pop();
                if(nodeInfo[cur.nodeName].finished == false){
                    curMinNodeName = cur.nodeName;
                    break;
                }
            }
            //end 优先级队列做法
            nodeInfo[curMinNodeName].finished = true;
            for(const auto & nodeIt : nodes){
                std::string edgeName;
                w = nodes[curMinNodeName]->getMinDistance(nodeIt.first,edgeName);
                if(nodeInfo[nodeIt.first].finished == false && w < maxWeight && nodeInfo[curMinNodeName].dist + w < nodeInfo[nodeIt.first].dist){
                    nodeInfo[nodeIt.first].dist = nodeInfo[curMinNodeName].dist + w;
                    nodeInfo[nodeIt.first].path.first = curMinNodeName,nodeInfo[nodeIt.first].path.second = edgeName;
                    //辅助实现优先级队列选权最小的
                    pq.push(distNamePair(nodeInfo[nodeIt.first].dist,nodeIt.first));
                    //end 辅助实现优先级队列选权最小的
                }
            }
        }
        std::string nodeName = endNodeName;
        while(!nodeInfo[nodeName].path.first.empty()){
            path.push_back(std::make_tuple(nodeInfo[nodeName].path.first,nodeName,nodeInfo[nodeName].path.second));
            nodeName = nodeInfo[nodeName].path.first;
        }
        std::reverse(path.begin(),path.end());//上面得到的path是逆序的路径，需要逆序来得到顺序的从源节点到终点的路径
    }

    void graph::get_dijkstra_emergency_shortest_path(const std::string & startNodeName, const std::string & endNodeName,std::vector<std::tuple<std::string,std::string,std::string>> & path,bool(*filter)(const pge::edge &))
    //path：里面三元组的顺序，起点名，终点名，边名，
    //filter：给定保留下来的元素需要满足的为真的条件，接收一个边。在这个函数里面filter是传给getMinDistance调用的
    {
        struct info{//结构体，包含了完成此函数需要的信息，dist：当前最短距离，finished：最短路径是否已求出，path：路径信息
            long long dist;
            bool finished;
            std::pair <std::string,std::string> path;//第一个string最短路径中前一个节点的名称，第二个string储存边的名称来明确是哪条边
        };
        //辅助实现优先级队列选权最小的
        struct distNamePair{
            long long dist;
            std::string nodeName;
            distNamePair(const long long & dist,const std::string & nodeName):dist(dist),nodeName(nodeName){};
        };
        struct cmp {
            bool operator() (const distNamePair & x,distNamePair & y) {
                return x.dist>y.dist;
            }
        };//end 辅助实现优先级队列选权最小的
        if (nodes.find(startNodeName) == nodes.end()){
            std::cerr<<"源节点不存在"<<std::endl;
            return;
        }
        if(nodes.find(endNodeName) == nodes.end()){
            std::cerr<<"终点节点不存在"<<std::endl;
            return;
        }
        std::unordered_map <std::string,info> nodeInfo;
        long long w,min;
        std::priority_queue <distNamePair,std::vector <distNamePair>,cmp> pq;
        long long maxWeight = std::numeric_limits <long long>::max();
        for(const auto & nodeIt : nodes){
            std::string edgeName;
            nodeInfo[nodeIt.first] = info();
            nodeInfo[nodeIt.first].dist = nodes[startNodeName]->getMinDistance(nodeIt.first,edgeName,filter);
            nodeInfo[nodeIt.first].finished = false;
            if(nodeIt.first != startNodeName && nodeInfo[nodeIt.first].dist < maxWeight){
                nodeInfo[nodeIt.first].path = std::make_pair(startNodeName,edgeName);
                //辅助实现优先级队列选权最小的
                pq.push(distNamePair(nodeInfo[nodeIt.first].dist,nodeIt.first));
                //end 辅助实现优先级队列选权最小的
            }
            else{
                nodeInfo[nodeIt.first].path = std::make_pair(std::string(),std::string());
            }
        }
        nodeInfo[startNodeName].finished = true;
        nodeInfo[startNodeName].dist = 0;
        size_t n = this->getNodeNum();
        for(size_t i=0;i<n;i++){
            min = std::numeric_limits <long long>::max();
            std::string curMinNodeName = startNodeName;//这样初始化，如果没有新的边，下面做的事情相当于没有更新
            //这里可以用优先队列进行优化，每次更新的时候直接把新的距离和顶点的名字加进去，然后判断finished=true的就跳过，直到找到finished=false的第一个
            // for(const auto & nodeIt : nodes){
            //     if(nodeInfo[nodeIt.first].finished==false&&nodeInfo[nodeIt.first].dist<min){curMinNodeName=nodeIt.first;min=nodeInfo[nodeIt.first].dist;}
            // }
            //优先级队列做法
            while(!pq.empty()){
                distNamePair cur = pq.top();
                pq.pop();
                if(nodeInfo[cur.nodeName].finished == false){
                    curMinNodeName = cur.nodeName;
                    break;
                }
            }
            //end 优先级队列做法
            nodeInfo[curMinNodeName].finished = true;
            for(const auto & nodeIt : nodes){
                std::string edgeName;
                w=nodes[curMinNodeName]->getMinDistance(nodeIt.first,edgeName,[](const pge::edge& e)->bool{return e.congestionLevel < 50 && e.securityRiskLevel < 50;});
                if(nodeInfo[nodeIt.first].finished == false && w < maxWeight && nodeInfo[curMinNodeName].dist + w < nodeInfo[nodeIt.first].dist){
                    nodeInfo[nodeIt.first].dist = nodeInfo[curMinNodeName].dist + w;
                    nodeInfo[nodeIt.first].path.first = curMinNodeName,nodeInfo[nodeIt.first].path.second = edgeName;
                    //辅助实现优先级队列选权最小的
                    pq.push(distNamePair(nodeInfo[nodeIt.first].dist,nodeIt.first));
                    //end 辅助实现优先级队列选权最小的
                }
            }
        }
        std::string nodeName = endNodeName;
        while(!nodeInfo[nodeName].path.first.empty()){
            path.push_back(std::make_tuple(nodeInfo[nodeName].path.first,nodeName,nodeInfo[nodeName].path.second));
            nodeName = nodeInfo[nodeName].path.first;
        }
        std::reverse(path.begin(),path.end());//上面得到的path是逆序的路径, 需要逆序来得到顺序的从源节点到终点的路径
    }

    void graph::display_emergency_all_path(
        const std::string & startNodeName,
        const std::string & endNodeName,
        std::ostream & out,
        bool(*filter)(const pge::edge &)
    ){
        if (nodes.find(startNodeName) == nodes.end()){
            out<<"该结点不在图中: "<<startNodeName<<std::endl;
            return;
        }
        if (nodes.find(endNodeName) == nodes.end()){
            out<<"该结点不在图中: "<<endNodeName<<std::endl;
            return;
        }
        if (startNodeName == endNodeName) {
            out<<"起点和终点相同，无路径需要遍历!"<<std::endl;
            return;
        }
        pge::graphPath tempPath;//临时对象
        tempPath.visitedNodeNames.insert(startNodeName);//初始化
        std::vector <pge::graphPath> allPaths;//保存所有路径
        get_emergency_all_path_helper(startNodeName,endNodeName,tempPath,allPaths,filter);//辅助函数
        if (allPaths.size() == 0){
            out<<"结点 "<<startNodeName<<" 到结点 "<<endNodeName<<" 还没有路径!"<<std::endl;
            return;
        }
        std::sort(allPaths.begin(),allPaths.end(),compareGraphPath);//排序
        for(size_t i=0;i<allPaths.size();i++){
            out<<"路径 "<<(i+1)<<" :"<<std::endl<<allPaths[i];//输出
        }
        return;
    }

    void graph::display_emergency_all_exit_path(const std::string & startNodeName,std::ostream & out,bool(*filter)(const pge::edge &)){
        if (nodes.find(startNodeName) == nodes.end()){
            out<<"该结点不在图中: "<<startNodeName<<std::endl;
            return;
        }
        std::vector <std::shared_ptr <pge::node>> allExitNodes;
        getAllExit(allExitNodes);
        if (allExitNodes.size() == 0){
            out<<"没有任何出口!"<<std::endl;
            return;
        }
        std::vector <pge::graphPath> allPaths;
        for(const auto & n : allExitNodes){
            pge::graphPath tempPath;
            tempPath.visitedNodeNames.insert(startNodeName);
            get_emergency_all_path_helper(startNodeName,n->nodeName,tempPath,allPaths,filter);
        }
        if (allPaths.size() == 0){
            out<<"没有任何通往出口的路径!"<<std::endl;
            return;
        }
        std::sort(allPaths.begin(),allPaths.end(),compareGraphPath);
        for(size_t i=0;i<allPaths.size();i++){
            out<<"路径 "<<(i+1)<<" (通往"<<allPaths[i].getEndNodeName()<<"):"<<std::endl<<allPaths[i];
        }
        return;
    }

    bool graph::normal_accessible(const std::string & startNodeName,const std::string & endNodeName){
        if (startNodeName == endNodeName) return true;
        if (nodes.find(startNodeName) == nodes.end()){
            std::cerr<<"该结点不在图中: "<<startNodeName<<std::endl;
            return false;
        }
        if (nodes.find(endNodeName) == nodes.end()){
            std::cerr<<"该结点不在图中: "<<endNodeName<<std::endl;
            return false;
        }
        std::stack <std::string> allNodeNames;
        std::unordered_set <std::string> visitedNodes;
        allNodeNames.push(startNodeName);
        visitedNodes.insert(startNodeName);
        while (!allNodeNames.empty()){
            std::string nodeName = allNodeNames.top();
            allNodeNames.pop();
            if (nodeName == endNodeName) return true;
            auto nodeIt = nodes.find(nodeName);
            if (nodeIt == nodes.end() || !nodeIt->second) continue;
            std::shared_ptr <pge::node> np = nodeIt->second;
            for(const auto & neighbor : np->neighbors){
                if (!neighbor.neighborNode) continue;
                std::string neighborName = neighbor.neighborNode->nodeName;
                if (visitedNodes.find(neighborName) == visitedNodes.end()){
                    visitedNodes.insert(neighborName);
                    allNodeNames.push(neighborName);
                }
            }
        }
        return false;
    }

    bool graph::emergency_accessible(const std::string & startNodeName,const std::string & endNodeName,bool(*filter)(const pge::edge &)){
        if (startNodeName == endNodeName) return true;
        if (nodes.find(startNodeName) == nodes.end()){
            std::cerr<<"该结点不在图中: "<<startNodeName<<std::endl;
            return false;
        }
        if (nodes.find(endNodeName) == nodes.end()){
            std::cerr<<"该结点不在图中: "<<endNodeName<<std::endl;
            return false;
        }
        std::stack <std::string> allNodeNames;
        std::unordered_set<std::string> visitedNodes;
        allNodeNames.push(startNodeName);
        visitedNodes.insert(startNodeName);
        while (!allNodeNames.empty()){
            std::string nodeName = allNodeNames.top();
            allNodeNames.pop();
            if (nodeName == endNodeName) return true;
            auto nodeIt = nodes.find(nodeName);
            if (nodeIt == nodes.end() || !nodeIt->second) continue;
            std::shared_ptr <pge::node> np = nodeIt->second;
            for(const auto & neighbor : np->neighbors){
                if (filter(neighbor)){
                    if (!neighbor.neighborNode) continue;
                    std::string neighborName = neighbor.neighborNode->nodeName;
                    if (visitedNodes.find(neighborName) == visitedNodes.end()){
                        visitedNodes.insert(neighborName);
                        allNodeNames.push(neighborName);
                    }
                }
            }
        }
        return false;
    }





    /*
    void testGraph(){
        pge::graph g("图");
        g.display_structure();
        g.addNode("A",false);
        g.addNode("B",true);
        g.addEdge("A","B","Edge1",10,5,2);
        g.addEdge("A","B","Edge2",15,3,1);
        //g.display_structure();
        g.outputFile_structure(".\\");

        g.input_structure(".\\graph.json");
        g.display_structure();

        return;
    }
    */

    std::string getReference(const std::string & str,size_t start){
        size_t end = str.size();
        for(size_t i=start;i<str.size();i++){
            if (str[i] == '\"'){
                end = i;
                break;
            }
        }
        return str.substr(start,end-start);
    }

    std::string encodeChinese(const std::string & cs){
        if (cs.empty()) return "";
        std::ostringstream oss;
        for(char c : cs){
            oss<<std::uppercase<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast <unsigned int>(static_cast <unsigned char>(c));
        }
        return oss.str();
    }

    std::string decodeChinese(const std::string & hex_str){
        if (hex_str.empty()) return "";
        std::string result = "";
        if (hex_str.size() % 2 != 0){
            std::cerr<<"非法的编码!"<<std::endl;
            return "";
        }
        for(size_t i=0;i<hex_str.size();i+=2){
            std::string hex_byte = hex_str.substr(i, 2);
            unsigned int byte_val;
            std::istringstream iss(hex_byte);
            if (!(iss>>std::hex>>byte_val) || byte_val>0xFF){
                std::cerr<<"非法的编码!"<<std::endl;
                return "";
            }
            result += static_cast <char>(byte_val);
        }
        return result;
    }




    graphPath::graphPath(){
        allEdges.clear();
        distance = 0;
        congestionLevel = 0;
        securityRiskLevel = 0;
        visitedNodeNames.clear();
    }

    graphPath::graphPath(const pge::graphPath & other){
        allEdges = other.allEdges;
        distance = other.distance;
        congestionLevel = other.congestionLevel;
        securityRiskLevel = other.securityRiskLevel;
        visitedNodeNames = other.visitedNodeNames;
    }

    graphPath::~graphPath(){}

    graphPath & graphPath::operator =(const pge::graphPath & other){
        if (this != &other){
            allEdges = other.allEdges;
            distance = other.distance;
            congestionLevel = other.congestionLevel;
            securityRiskLevel = other.securityRiskLevel;
            visitedNodeNames = other.visitedNodeNames;
        }
        return *this;
    }

    void graphPath::addEdge(const pge::edge & e){
        if (!e.neighborNode){
            std::cerr << "错误：边" << e.edgeName << "的邻接节点为空！" << std::endl;
            return;
        }
        const std::string & nextNodeName = e.neighborNode->nodeName;
        if (visitedNodeNames.find(nextNodeName) != visitedNodeNames.end()) return;
        visitedNodeNames.insert(nextNodeName);
        allEdges.push_back(std::make_tuple(e.startNodeName,nextNodeName,e.edgeName));
        this->distance += static_cast <double>(e.distance);
        if (allEdges.size() == 1){
            this->congestionLevel = static_cast <double>(e.congestionLevel);
            this->securityRiskLevel = static_cast <double>(e.securityRiskLevel);
        }
        else{
            this->congestionLevel = (this->congestionLevel * (allEdges.size() - 1) + static_cast <double>(e.congestionLevel)) / allEdges.size();
            this->securityRiskLevel = (this->securityRiskLevel * (allEdges.size() - 1) + static_cast <double>(e.securityRiskLevel)) / allEdges.size();
        }
        return;
    }

    std::string graphPath::getEndNodeName(){
        return std::get <1>(allEdges[allEdges.size() - 1]);
    }

    std::ostream & operator <<(std::ostream & o,const pge::graphPath & p){
        for(const auto & e : p.allEdges){
            o<<"("<<std::get <0>(e)<<") -- "<<std::get <2>(e)<<" -- ("<<std::get <1>(e)<<")"<<std::endl;
        }
        o<<"路径长度: "<<p.distance<<std::endl;
        o<<"拥堵程度: "<<p.congestionLevel<<std::endl;
        o<<"安全风险等级: "<<p.securityRiskLevel<<std::endl;
        return o;
    }

    bool compareGraphPath(const graphPath & a,const graphPath & b){
        if (a.securityRiskLevel < b.securityRiskLevel) return true;
        else if (a.securityRiskLevel > b.securityRiskLevel) return false;
        else{
            if (a.distance < b.distance) return true;
            else if (a.distance > b.distance) return false;
            else{
                if (a.congestionLevel < b.congestionLevel) return true;
                else return false;
            }
        }
    }

}


