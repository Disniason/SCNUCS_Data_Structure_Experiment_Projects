#ifndef GRAPHDB_H
#define GRAPHDB_H
#pragma once

#include "graph.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <unordered_set>
#include <memory>

namespace pge{

    class graphDB{
        protected:
        std::string dataPath;//数据库文件夹的相对路径
        std::unordered_set <std::string> graphNames;//所有图名集合

        public:
        graphDB(const std::string & path){
            if (!std::filesystem::exists(path) || !std::filesystem::is_directory(path)){
                std::cerr<<"这是一个非法的图数据库路径: "<<path<<std::endl;
                return;
            }
            dataPath = path;
            try{
                for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(dataPath)){
                    if (std::filesystem::is_regular_file(entry.status())) {
                        std::filesystem::path filePath = entry.path();
                        std::string fileName = filePath.filename().string();
                        std::string suffix = ".json";
                        if (fileName.size() >= suffix.size() && fileName.substr(fileName.size() - suffix.size()) == suffix){
                            std::string graphName = fileName.substr(0, fileName.size() - suffix.size());
                            graphName = pge::decodeChinese(graphName);
                            if (!graphName.empty()) graphNames.insert(graphName);
                            else std::cerr<<"发现无效的.json文件(无文件名): "<<fileName<<std::endl;
                        }
                    }
                }
            } catch (const std::filesystem::filesystem_error & e){
                std::cerr<<"遍历文件夹时发生错误："<<e.what()<<std::endl;
            }
        }
        ~graphDB(){}
        bool hasGraph(const std::string & name){
            return graphNames.find(name) != graphNames.end();
        }
        bool createGraph(const std::string & name,std::shared_ptr <pge::graph> g){//打开图
            if (!hasGraph(name)){
                std::cerr<<"数据库中没有该图: "<<name<<std::endl;
                return false;
            }
            g->setDataPath(dataPath);
            g->setGraphName(name);
            g->clear();
            std::string filePath = pge::encodeChinese(name);//计算编码
            filePath = dataPath + "\\" + filePath + ".json";
            if (!g->input_structure(filePath)){
                std::cerr<<"从文件 "<<filePath<<" 读取图结构失败!"<<std::endl;
                return false;
            }
            return true;
        }
        bool saveGraph(std::shared_ptr <pge::graph> g){//保存图
            std::string name = g->getGraphName();
            if (name.empty()){
                std::cerr<<"图的名称不能为空!"<<std::endl;
                return false;
            }
            if (hasGraph(name)) std::cerr<<"数据库中已存在同名图: "<<name<<", 现已将原图覆盖"<<std::endl;
            g->setDataPath(dataPath);
            graphNames.insert(name);
            name = pge::encodeChinese(name);//计算编码
            g->outputFile_structure(dataPath,name);
            return true;
        }

        void displayAllGraphName(std::ostream & out = std::cout){//打印所有图名
            if (graphNames.empty()){
                out<<"暂时还没有图"<<std::endl;
                return;
            }
            for(const std::string & name : graphNames){
                out<<name<<std::endl;
            }
            return;
        }

        bool removeGraph(const std::string & graphName){//删除图
            if (!hasGraph(graphName)){
                std::cerr<<"数据库中没有该图: "<<graphName<<std::endl;
                return false;
            }
            bool noName = graphNames.erase(graphName) > 0;
            std::filesystem::path filePath = std::filesystem::path(dataPath) / std::filesystem::path(pge::encodeChinese(graphName) + ".json");
            bool noFile = !std::filesystem::exists(filePath);
            if (!noFile){
                try {
                    std::filesystem::remove(filePath);
                } catch (const std::filesystem::filesystem_error & e){
                    return false;
                }
            }
            noFile = !std::filesystem::exists(filePath);
            return noName && noFile;
        }

    };

}

#endif