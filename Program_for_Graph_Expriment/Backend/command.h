#ifndef COMMAND_H
#define COMMAND_H
#pragma once

#include "graph.h"
#include "graphDB.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>

namespace pge{

    extern void cmdSplit(const std::string & cmd,std::vector <std::string> & parts);//将指令分割为字符串数组的函数

    extern pge::graphDB gdb;//全局图数据库对象
    extern std::shared_ptr <pge::graph> globalGraphPtr;//全局临时图指针

    class cmdDispatcher{
        protected:
        std::unordered_map <
            std::string,
            std::unordered_map <
                std::string,
                std::function <void(const std::vector <std::string> & parts,std::ostream & out)>
            >
        > cmdMap;
        std::string cmdSavePath;//指令日志存储路径

        public:
        cmdDispatcher();

        void saveCmd(const std::string & cmd,std::ofstream & out) const;//保存指令到日志

        void execute() const;//供纯控制台程序使用

        std::string executeCommand(const std::string & cmd) const;//供GUI调用
    };

    extern const pge::cmdDispatcher cmd;//全局对象




}


#endif