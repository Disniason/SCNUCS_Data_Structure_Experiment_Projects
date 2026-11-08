#include "command.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <tuple>
#include <unordered_map>
#include <memory>
#include <functional>
#include <stdexcept>

namespace pge{

    void cmdSplit(const std::string & cmd,std::vector <std::string> & parts){
        parts.clear();
        std::istringstream iss(cmd);
        std::string part;
        while(iss>>part){
            parts.push_back(part);
        }
        return;
    }

    pge::graphDB gdb(".\\data");

    std::shared_ptr <pge::graph> globalGraphPtr = std::make_shared <pge::graph>("empty_graph");
    

    cmdDispatcher::cmdDispatcher(){
        cmdMap["user"] = {
            {"inputStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if(globalGraphPtr->input_structure(parts[3])) out<<"导入图成功!"<<std::endl;
                    else out<<"导入图失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"displayStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->display_structure(out);
                return;
            }},
            {"outputStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->outputFile_structure(parts[3],parts[4])) out<<"图结构导出成功!"<<std::endl;
                    else out<<"图结构导出失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"refreshGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                std::string graphName = globalGraphPtr->getGraphName();
                if (gdb.createGraph(graphName,globalGraphPtr)) out<<"刷新图成功!"<<std::endl;
                    else out<<"刷新图失败: "<<graphName<<std::endl;
                return;
            }},
            {"displayAllGraphName", [](const std::vector <std::string> & parts,std::ostream & out){
                gdb.displayAllGraphName(out);
                return;
            }},
            {"createGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::string graphName = parts[3];
                    if (gdb.createGraph(graphName,globalGraphPtr)) out<<"打开图成功!"<<std::endl;
                    else out<<"打开图失败: "<<graphName<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"saveGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (gdb.saveGraph(globalGraphPtr)) out<<"成功保存图: "<<globalGraphPtr->getGraphName()<<std::endl;
                else out<<"保存图失败: "<<globalGraphPtr->getGraphName()<<std::endl;
                return;
            }},
            {"clear", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->clear();
                return;
            }},
            {"displayAllExit", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->displayAllExit(out);
                return;
            }},
            // hjb 增加了没有该边时的输出
            {"displayEdgeInfo", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 6){
                    long long distance = globalGraphPtr->getDistance(parts[3],parts[4],parts[5]);
                    if (distance==std::numeric_limits<long long>::max()){
                        out<<"边 ("<<parts[3]<<") -- "<<parts[5]<<" -- ("<<parts[4]<<") 不存在！";
                    }
                    else{
                        out<<"边 ("<<parts[3]<<") -- "<<parts[5]<<" -- ("<<parts[4]<<") 的基本信息为:"<<std::endl;
                        out<<"长度: "<<distance<<std::endl;
                        out<<"拥堵程度: "<<globalGraphPtr->getCongestionLevel(parts[3],parts[4],parts[5])<<std::endl;
                        out<<"安全风险等级: "<<globalGraphPtr->getSecurityRiskLevel(parts[3],parts[4],parts[5])<<std::endl;
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"displayMST", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->display_minimum_spanning_tree(out);
                return;
            }},
            {"outputFileMST", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if (globalGraphPtr->outputFile_minimum_spanning_tree(parts[3])) out<<"输出最小生成树成功!"<<std::endl;
                    else out<<"输出最小生成树失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
            }},
            {"DFS", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::vector <std::string> dfs;
                    globalGraphPtr->depth_first_search(parts[3],dfs);
                    if (dfs.size() > 0){
                        out<<"深度优先遍历:"<<std::endl;
                        for(const std::string & nodeName : dfs){
                            out<<nodeName<<std::endl;
                        }
                    }
                    else out<<"图是空的!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"BFS", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::vector <std::string> bfs;
                    globalGraphPtr->breadth_first_search(parts[3],bfs);
                    if (bfs.size() > 0){
                        out<<"深度优先遍历:"<<std::endl;
                        for(const std::string & nodeName : bfs){
                            out<<nodeName<<std::endl;
                        }
                    }
                    else out<<"图是空的!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"normalShortestPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    std::vector <std::tuple <std::string,std::string,std::string>> nsp;
                    globalGraphPtr->get_dijkstra_normal_shortest_path(parts[3],parts[4],nsp);
                    if (nsp.size() == 0){
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间无最短路径!"<<std::endl;
                        return;
                    }
                    else{
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间的最短路径为:"<<std::endl;
                        for(const std::tuple <std::string,std::string,std::string> & e : nsp){
                            out<<"("<<std::get <0>(e)<<") -- "<<std::get <2>(e)<<" -- ("<<std::get <1>(e)<<")"<<std::endl;
                        }
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyShortestPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    std::vector <std::tuple <std::string,std::string,std::string>> esp;
                    globalGraphPtr->get_dijkstra_emergency_shortest_path(parts[3],parts[4],esp);
                    if (esp.size() == 0){
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间无最短路径!"<<std::endl;
                        return;
                    }
                    else{
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间的最短路径为:"<<std::endl;
                        for(const std::tuple <std::string,std::string,std::string> & e : esp){
                            out<<"("<<std::get <0>(e)<<") -- "<<std::get <2>(e)<<" -- ("<<std::get <1>(e)<<")"<<std::endl;
                        }
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyAllPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5) globalGraphPtr->display_emergency_all_path(parts[3],parts[4],out);
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyAllExitPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4) globalGraphPtr->display_emergency_all_exit_path(parts[3],out);
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"isNormalAccessible", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->normal_accessible(parts[3],parts[4])) out<<"结点 "<<parts[3]<<" 可达结点 "<<parts[4]<<std::endl;
                    else out<<"结点 "<<parts[3]<<" 不可达结点 "<<parts[4]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"isEmergencyAccessible", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->emergency_accessible(parts[3],parts[4])) out<<"结点 "<<parts[3]<<" 可达结点 "<<parts[4]<<std::endl;
                    else out<<"结点 "<<parts[3]<<" 不可达结点 "<<parts[4]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }}




        };
        cmdMap["administrator"] = {
            {"inputStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if(globalGraphPtr->input_structure(parts[3])) out<<"导入图成功!"<<std::endl;
                    else out<<"导入图失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"displayStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->display_structure(out);
                return;
            }},
            {"outputStructure", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->outputFile_structure(parts[3],parts[4])) out<<"图结构导出成功!"<<std::endl;
                    else out<<"图结构导出失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"refreshGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                std::string graphName = globalGraphPtr->getGraphName();
                if (gdb.createGraph(graphName,globalGraphPtr)) out<<"刷新图成功!"<<std::endl;
                    else out<<"刷新图失败: "<<graphName<<std::endl;
                return;
            }},
            {"displayAllGraphName", [](const std::vector <std::string> & parts,std::ostream & out){
                gdb.displayAllGraphName(out);
                return;
            }},
            {"createGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::string graphName = parts[3];
                    if (gdb.createGraph(graphName,globalGraphPtr)) out<<"打开图成功!"<<std::endl;
                    else out<<"打开图失败: "<<graphName<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"saveGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (gdb.saveGraph(globalGraphPtr)) out<<"成功保存图: "<<globalGraphPtr->getGraphName()<<std::endl;
                else out<<"保存图失败: "<<globalGraphPtr->getGraphName()<<std::endl;
                return;
            }},
            {"establishGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4) globalGraphPtr = std::make_shared <pge::graph>(parts[3]);
                else std::cerr<<"指令格式错误!"<<std::endl;
            }},
            {"removeGraph", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if (gdb.removeGraph(parts[3])) out<<"成功删除图: "<<parts[3]<<std::endl;
                    else out<<"删除图失败: "<<parts[3]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
            }},
            {"addNode", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    std::string nodeName = parts[3];
                    bool isExit = std::stoi(parts[4]) != 0;
                    if (pge::globalGraphPtr->addNode(nodeName,isExit)) out<<"成功添加结点: "<<nodeName<<std::endl;
                    else out<<"添加结点失败: "<<nodeName<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"addEdge", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 9){
                    long long d = 0;
                    long long c = 0;
                    long long s = 0;
                    try{
                        d = std::stoll(parts[6]);
                        c = std::stoll(parts[7]);
                        s = std::stoll(parts[8]);
                    } catch (...){
                        out<<"输入的数字不合法, 请重新输入!"<<std::endl;
                        return;
                    }
                    if (globalGraphPtr->addEdge(parts[3],parts[4],parts[5],d,c,s)) out<<"成功添加边: "<<parts[5]<<std::endl;
                    else out<<"添加边失败: "<<parts[5]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"removeNode", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if (globalGraphPtr->removeNode(parts[3])) out<<"成功删除结点: "<<parts[3]<<std::endl;
                    else out<<"删除结点失败: "<<parts[3]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"removeEdge", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 6){
                    if (globalGraphPtr->removeEdge(parts[3],parts[4],parts[5])) out<<"成功删除边: "<<parts[5]<<std::endl;
                    else out<<"删除边失败: "<<parts[5]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"clear", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->clear();
                return;
            }},
            {"displayAllExit", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->displayAllExit(out);
                return;
            }},
            // hjb 增加了没有该边时的输出
            {"displayEdgeInfo", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 6){
                    long long distance = globalGraphPtr->getDistance(parts[3],parts[4],parts[5]);
                    if (distance==std::numeric_limits<long long>::max()){
                        out<<"边 ("<<parts[3]<<") -- "<<parts[5]<<" -- ("<<parts[4]<<") 不存在！";
                    }
                    else{
                        out<<"边 ("<<parts[3]<<") -- "<<parts[5]<<" -- ("<<parts[4]<<") 的基本信息为:"<<std::endl;
                        out<<"长度: "<<distance<<std::endl;
                        out<<"拥堵程度: "<<globalGraphPtr->getCongestionLevel(parts[3],parts[4],parts[5])<<std::endl;
                        out<<"安全风险等级: "<<globalGraphPtr->getSecurityRiskLevel(parts[3],parts[4],parts[5])<<std::endl;
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"setEdgeInfo", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 9){
                    long long d = 0;
                    long long c = 0;
                    long long s = 0;
                    try{
                        d = std::stoll(parts[6]);
                        c = std::stoll(parts[7]);
                        s = std::stoll(parts[8]);
                    } catch (...){
                        out<<"输入的数字不合法, 请重新输入!"<<std::endl;
                        return;
                    }
                    if (
                        globalGraphPtr->setDistance(parts[3],parts[4],parts[5],d) &&
                        globalGraphPtr->setCongestionLevel(parts[3],parts[4],parts[5],c) &&
                        globalGraphPtr->setSecurityRiskLevel(parts[3],parts[4],parts[5],s)
                    ) out<<"成功设置边信息: "<<parts[5]<<std::endl;
                    else out<<"设置边信息失败: "<<parts[5]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"displayMST", [](const std::vector <std::string> & parts,std::ostream & out){
                globalGraphPtr->display_minimum_spanning_tree(out);
                return;
            }},
            {"outputFileMST", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    if (globalGraphPtr->outputFile_minimum_spanning_tree(parts[3])) out<<"输出最小生成树成功!"<<std::endl;
                    else out<<"输出最小生成树失败!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
            }},
            {"DFS", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::vector <std::string> dfs;
                    globalGraphPtr->depth_first_search(parts[3],dfs);
                    if (dfs.size() > 0){
                        out<<"深度优先遍历:"<<std::endl;
                        for(const std::string & nodeName : dfs){
                            out<<nodeName<<std::endl;
                        }
                    }
                    else out<<"图是空的!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"BFS", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4){
                    std::vector <std::string> bfs;
                    globalGraphPtr->breadth_first_search(parts[3],bfs);
                    if (bfs.size() > 0){
                        out<<"深度优先遍历:"<<std::endl;
                        for(const std::string & nodeName : bfs){
                            out<<nodeName<<std::endl;
                        }
                    }
                    else out<<"图是空的!"<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"normalShortestPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    std::vector <std::tuple <std::string,std::string,std::string>> nsp;
                    globalGraphPtr->get_dijkstra_normal_shortest_path(parts[3],parts[4],nsp);
                    if (nsp.size() == 0){
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间无最短路径!"<<std::endl;
                        return;
                    }
                    else{
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间的最短路径为:"<<std::endl;
                        for(const std::tuple <std::string,std::string,std::string> & e : nsp){
                            out<<"("<<std::get <0>(e)<<") -- "<<std::get <2>(e)<<" -- ("<<std::get <1>(e)<<")"<<std::endl;
                        }
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyShortestPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    std::vector <std::tuple <std::string,std::string,std::string>> esp;
                    globalGraphPtr->get_dijkstra_emergency_shortest_path(parts[3],parts[4],esp);
                    if (esp.size() == 0){
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间无最短路径!"<<std::endl;
                        return;
                    }
                    else{
                        out<<"结点 "<<parts[3]<<" 到结点 "<<parts[4]<<" 之间的最短路径为:"<<std::endl;
                        for(const std::tuple <std::string,std::string,std::string> & e : esp){
                            out<<"("<<std::get <0>(e)<<") -- "<<std::get <2>(e)<<" -- ("<<std::get <1>(e)<<")"<<std::endl;
                        }
                    }
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyAllPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5) globalGraphPtr->display_emergency_all_path(parts[3],parts[4],out);
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"emergencyAllExitPath", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 4) globalGraphPtr->display_emergency_all_exit_path(parts[3],out);
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"isNormalAccessible", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->normal_accessible(parts[3],parts[4])) out<<"结点 "<<parts[3]<<" 可达结点 "<<parts[4]<<std::endl;
                    else out<<"结点 "<<parts[3]<<" 不可达结点 "<<parts[4]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }},
            {"isEmergencyAccessible", [](const std::vector <std::string> & parts,std::ostream & out){
                if (parts.size() >= 5){
                    if (globalGraphPtr->emergency_accessible(parts[3],parts[4])) out<<"结点 "<<parts[3]<<" 可达结点 "<<parts[4]<<std::endl;
                    else out<<"结点 "<<parts[3]<<" 不可达结点 "<<parts[4]<<std::endl;
                }
                else std::cerr<<"指令格式错误!"<<std::endl;
                return;
            }}






        };
        cmdSavePath = ".\\cmd.log";
    }

    void cmdDispatcher::saveCmd(const std::string & cmd,std::ofstream & out) const{
        if (!out.is_open()){
            std::cerr<<"无法打开指令日志文件 "<<cmdSavePath<<std::endl;
            return;
        }
        out<<cmd<<std::endl;
        return;
    }

    void cmdDispatcher::execute() const{
        std::string cmd;
        std::ofstream out(cmdSavePath,std::ios::app);//指令日志文件输出流
        while (std::getline(std::cin,cmd)){
            saveCmd(cmd,out);//将指令保存到日志文件
            try{
                std::vector <std::string> parts;
                cmdSplit(cmd,parts);//将指令分割为字符串数组
                if (parts.size() < 2){
                    if (parts.size() == 1 && parts[0] == "exit") break;
                    std::cerr<<"指令格式错误!"<<std::endl;
                    continue;
                }
                else if (parts[0] != "PGE"){
                    std::cerr<<"未知的指令系统: "<<parts[0]<<std::endl;
                    continue;
                }
                else{
                    auto identificationIt = cmdMap.find(parts[1]);
                    if (identificationIt == cmdMap.end()){
                        std::cerr<<"未知的身份: "<<parts[1]<<std::endl;
                        continue;
                    }
                    else{
                        auto cmdIt = identificationIt->second.find(parts[2]);
                        if (cmdIt == identificationIt->second.end()){
                            std::cerr<<"未知的指令: "<<parts[2]<<std::endl;
                            continue;
                        }
                        else{
                            cmdIt->second(parts,std::cout);
                        }
                    }
                }
            } catch (const std::exception & e){
                std::cerr<<e.what()<<std::endl;
                continue;
            }
        }
        out.close();
        return;
    }

    std::string cmdDispatcher::executeCommand(const std::string & cmd) const{
        std::ostringstream oss;
        try{
            std::vector <std::string> parts;
            cmdSplit(cmd,parts);//将指令分割为字符串数组
            if (parts.size() < 2){
                if (parts.size() == 1 && parts[0] == "exit") return "";
                std::cerr<<"指令格式错误!"<<std::endl;
                return "";
            }
            else if (parts[0] != "PGE"){
                std::cerr<<"未知的指令系统: "<<parts[0]<<std::endl;
                return "";
            }
            else{
                auto identificationIt = cmdMap.find(parts[1]);
                if (identificationIt == cmdMap.end()){
                    std::cerr<<"未知的身份: "<<parts[1]<<std::endl;
                    return "";
                }
                else{
                    auto cmdIt = identificationIt->second.find(parts[2]);
                    if (cmdIt == identificationIt->second.end()){
                        std::cerr<<"未知的指令: "<<parts[2]<<std::endl;
                        return "";
                    }
                    else{
                        cmdIt->second(parts,oss);
                        return oss.str();
                    }
                }
            }
        } catch (const std::exception & e){
            oss<<e.what()<<std::endl;
            return oss.str();
        }
        return "";
    }

    const pge::cmdDispatcher cmd;

}