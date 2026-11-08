#ifndef DISJOINT_SET_H
#define DISJOINT_SET_H
#pragma once

/*
    轻量级并查集实现，对接图的最小生成树的kruskal算法

    如果想要存储自定义类型, 则需要自己实现std::hash模板函数和==运算符重载
*/

#include <vector>
#include <unordered_map>
#include <cstddef>

namespace pge{
    template <class T>
    class disjoint_set{
        protected:
        std::unordered_map <T,size_t> positions;//数据与双亲数组下标的映射
        std::vector <long long> parents;//双亲数组

        public:
        disjoint_set(){
            positions.clear();
            parents.clear();
        }
        ~disjoint_set(){}
        void push(const T & newElement){
            if (positions.find(newElement) != positions.end()) return;
            size_t position = parents.size();
            parents.push_back(-1);
            positions.emplace(newElement,position);
            return;
        }
        long long findRoot(size_t position){
            long long root = position;
            while (parents[root] >= 0){
                root = parents[root];
            }
            while (position != root) {
                long long temp = parents[position];
                parents[position] = root;
                position = temp;
            }
            return root;
        }
        void merge(size_t posA,size_t posB){
            long long rootA = findRoot(posA);
            long long rootB = findRoot(posB);
            if (rootA != rootB){
                long long temp = parents[rootA] + parents[rootB];
                if (parents[rootA] < parents[rootB]){
                    parents[rootA] = temp;
                    parents[rootB] = rootA;
                }
                else{
                    parents[rootB] = temp;
                    parents[rootA] = rootB;
                }
            }
            return;
        }
        void merge(const T & a,const T & b){
            auto itA = positions.find(a);
            auto itB = positions.find(b);
            if (itA == positions.end() || itB == positions.end()) return;
            merge(itA->second,itB->second);
            return;
        }
        bool isConnected(const T & a,const T & b){
            auto itA = positions.find(a);
            auto itB = positions.find(b);
            if (itA == positions.end() || itB == positions.end()) return false;
            long long rootA = findRoot(itA->second);
            long long rootB = findRoot(itB->second);
            return rootA == rootB;
        }
    };
}

#endif