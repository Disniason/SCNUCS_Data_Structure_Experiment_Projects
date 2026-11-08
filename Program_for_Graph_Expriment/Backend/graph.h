#ifndef GRAPH_H
#define GRAPH_H
#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <cstddef>

namespace pge{
    class edge;
    class graphPath;

    class node{//图的顶点类
        public:
        std::string nodeName;//结点名称
        bool isExit;//true : 是出口 false : 不是出口
        std::list <pge::edge> neighbors;

        node(const std::string & name,bool ie);
        node(const pge::node & other);
        ~node();
        pge::node & operator =(const pge::node & other);//深复制
        void addNeighbor(const pge::edge & e);//添加邻接边
        void removeNeighbor(const std::string & node_name,std::unordered_set <std::string> & allEdgeNames);//移除邻接顶点(将相关联的邻接边全部删除)
        void removeEdge(const std::string & name);//移除邻接边
        size_t getDegree();//获取结点度(邻接边的数量)
        long long getDistance(const std::string & endNodeName,const std::string & name);
        long long getCongestionLevel(const std::string & endNodeName,const std::string & name);
        long long getSecurityRiskLevel(const std::string & endNodeName,const std::string & name);
        bool setDistance(const std::string & endNodeName,const std::string & name,long long d);
        bool setCongestionLevel(const std::string & endNodeName,const std::string & name,long long c);
        bool setSecurityRiskLevel(const std::string & endNodeName,const std::string & name,long long s);
        //std::string findNeighborName(const std::string & edgeName);//根据邻接边名查找邻接顶点名(已经遗弃)
        void output_structure(const std::string & table,std::ostream & out = std::cout) const;//输出顶点的结构

        //为了实现dijkstra算法的辅助函数，用于获取以this为起点，到名称为endNodeName的终点的边界点里面距离最短的，参数列表中传入的引用edgeName用于返回对应的边的名字
        long long getMinDistance(const std::string &endNodeName,std::string & edgeName,bool(*filter)(const pge::edge&)=NULL);
    };

    class edge{//图的边类，由于图不是简单图即带重边，所以必须区分不同的边
        public:
        std::string edgeName;//边名
        std::string startNodeName;//边始点名
        std::shared_ptr <pge::node> neighborNode;//边终点顶点的指针
        long long distance;//边的长度
        long long congestionLevel;//拥堵程度
        long long securityRiskLevel;//安全风险等级

        edge(const std::string & name,const std::string & startName,std::shared_ptr <pge::node> n,long long d,long long c,long long s);
        edge(const pge::edge & other);
        ~edge();
        pge::edge & operator =(const pge::edge & other);//浅复制
        friend bool operator ==(const pge::edge & a,const pge::edge & b);
        friend bool operator <(const pge::edge & a,const pge::edge & b);
        friend bool operator >(const pge::edge & a,const pge::edge & b);
        void output_structure(const std::string & table,std::ostream & out = std::cout) const;
    };

    //一系列运算符重载，用于std::unordered_map, std::priority_queue等标准库模板类
    bool operator ==(const pge::edge & a,const pge::edge & b);
    bool operator <(const pge::edge & a,const pge::edge & b);
    bool operator >(const pge::edge & a,const pge::edge & b);
    /*
    struct edgeComparator{
        bool operator ()(const pge::edge & a,const pge::edge & b) const;
    };*/

    class graph{//图类
        protected:
        std::string dataPath;//该图所在数据库的文件夹路径
        std::string graphName;//图名
        std::unordered_map <std::string,std::shared_ptr <pge::node>> nodes;//顶点名到顶点指针的映射，便于查找
        std::unordered_set <std::string> allEdgeNames;//防止出现同名边
        
        bool createNode(const std::string & name,bool ie);//打开图时添加结点的函数，与addNode功能完全相同，但没有错误输出
        bool createEdge(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d,long long c,long long s);//打开图时添加边的函数，与addEdge功能完全相同，但没有错误输出
        void output_kruskal_minimum_spanning_tree(std::ostream & out = std::cout);//辅助实现kruskal算法
        void depth_first_search_helper(const std::string & nodeName,std::unordered_set <std::string> & visited,std::vector <std::string> & nodeNames);//辅助实现深度优先遍历的递归函数
        void output_structure(const std::string & table,std::ostream & out = std::cout) const;//辅助输出树结构的函数
        void get_emergency_all_path_helper(const std::string & startNodeName,const std::string & endNodeName,pge::graphPath path,std::vector <pge::graphPath> & paths,bool(*filter)(const pge::edge &));//辅助获取所有路径的函数
        //工具函数，用于给定起点和终点（事实上这里起终点没有明显区别，就是两个端点），生成{startNodeName}_{endNodeName}格式的边节点的名称，可以用于graph类中的addEdge函数的代表边的名称的name参数的生成
        const std::string generateEdgeName(const std::string & startNodeName,const std::string & endNodeName);

        public:
        graph(const std::string & name = "graph");
        graph(const pge::graph & other);
        ~graph();
        pge::graph & operator =(const pge::graph & other);//深复制
        bool addNode(const std::string & name,bool ie);//添加顶点
        bool removeNode(const std::string & nodeName);//移除顶点
        bool addEdge(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d,long long c,long long s);//添加边
        bool removeEdge(const std::string & startNodeName,const std::string &endNodeName,const std::string & name);//移除边
        long long getDistance(const std::string & startNodeName,const std::string & endNodeName,const std::string & name);
        long long getCongestionLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name);
        long long getSecurityRiskLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name);
        long long getDistance(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name);
        long long getCongestionLevel(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name);
        long long getSecurityRiskLevel(std::shared_ptr <pge::node> startNodePtr,std::shared_ptr <pge::node> endNodePtr,const std::string & name);
        bool setDistance(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long d);
        bool setCongestionLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long c);
        bool setSecurityRiskLevel(const std::string & startNodeName,const std::string & endNodeName,const std::string & name,long long s);
        void getAllExit(std::vector <std::shared_ptr <pge::node>> & exitNodes);//获取所有出口顶点的指针
        void displayAllExit(std::ostream & out = std::cout);//打印所有出口顶点的名称
        size_t getNodeNum();//获取顶点总数
        void setDataPath(const std::string & path);//设置该图所在数据库的文件夹路径
        std::string getDataPath();//获取该图所在数据库的文件夹路径
        void setGraphName(const std::string & name);//设置图名
        std::string getGraphName();//获取图名
        void clear();//清空图
        void display_minimum_spanning_tree(std::ostream & out = std::cout);//打印最小生成树
        bool outputFile_minimum_spanning_tree(const std::string & fileName);//输出最小生成树
        void breadth_first_search(const std::string & startNodeName,std::vector <std::string> & nodeNames);//广度优先遍历
        void depth_first_search(const std::string & startNodeName,std::vector <std::string> & nodeNames);//深度优先遍历
        void display_structure(std::ostream & out = std::cout);//显示图结构
        bool outputFile_structure(const std::string & filePath,const std::string & fileName = "graph.json");//输出图结构到文件
        bool input_structure(const std::string & filePath);//从文件导入图结构

        
        //求在正常情况下的最短路径，返回路径上的边的名称数组
        void get_dijkstra_normal_shortest_path(const std::string & startNodeName,const std::string & endNodeName,std::vector<std::tuple<std::string,std::string,std::string>>&path);
        //求在紧急情况下的最短路径，返回路径上的边的名称数组
        void get_dijkstra_emergency_shortest_path(const std::string & startNodeName,const std::string & endNodeName,std::vector<std::tuple<std::string,std::string,std::string>>&path,bool(*filter)(const pge::edge &) = [](const pge::edge & e)->bool{return e.congestionLevel < 50 && e.securityRiskLevel < 50;});
        //求在紧急情况下的所有路径，返回路径数组，每个路径数组又是一个边的名称数组
        void display_emergency_all_path(const std::string & startNodeName,const std::string & endNodeName,std::ostream & out,bool(*filter)(const pge::edge &) = [](const pge::edge & e)->bool{return e.congestionLevel < 50 && e.securityRiskLevel < 50;});
        //求在紧急情况下的所有通往出口的路径，返回路径数组，每个路径数组又是一个边的名称数组
        void display_emergency_all_exit_path(const std::string & startNodeName,std::ostream & out,bool(*filter)(const pge::edge &) = [](const pge::edge & e)->bool{return e.congestionLevel < 50 && e.securityRiskLevel < 50;});
        //判断正常情况下是否存在路径
        bool normal_accessible(const std::string & startNodeName,const std::string & endNodeName);
        //判断紧急情况下是否存在路径
        bool emergency_accessible(const std::string & startNodeName,const std::string & endNodeName,bool(*filter)(const pge::edge &) = [](const pge::edge & e)->bool{return e.congestionLevel < 50 && e.securityRiskLevel < 50;});
        
    };

    //void testGraph();
    std::string getReference(const std::string & str,size_t start);

    std::string encodeChinese(const std::string & cs);//编码中文
    std::string decodeChinese(const std::string & hex_str);//译码中文

    class graphPath{//图路径类, 辅助实现查找所有路径
        public:
        std::vector <std::tuple <std::string,std::string,std::string>> allEdges;//所有边
        double distance;//和
        double congestionLevel;//平均值
        double securityRiskLevel;//平均值
        std::unordered_set <std::string> visitedNodeNames;//已经遍历过的顶点名，防止出现环
        
        graphPath();
        graphPath(const pge::graphPath & other);
        ~graphPath();
        graphPath & operator =(const pge::graphPath & other);
        void addEdge(const pge::edge & e);
        friend std::ostream & operator <<(std::ostream & o,const pge::graphPath & p);
        std::string getEndNodeName();
    };

    std::ostream & operator <<(std::ostream & o,const pge::graphPath & p);
    bool compareGraphPath(const graphPath & a,const graphPath & b);//图路径比较器, 与edge类的<重载逻辑相同
}

#endif