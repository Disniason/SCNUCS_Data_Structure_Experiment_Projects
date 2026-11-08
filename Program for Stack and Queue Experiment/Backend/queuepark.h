#ifndef QUEUEPARK_H
#define QUEUEPARK_H
#pragma once

#include "car.h"

#include <memory>

class listnode{
    public:
    car data;
    std::shared_ptr <listnode> next;
    listnode(car cara){
        data = cara;
        next = nullptr;
    }
};

class queuepark{
    private:
    int maxcarsize;
    int length;
    std::shared_ptr <listnode> tail;
    public:
    queuepark(){
        maxcarsize = 0;
        length = 0;
        tail = std::make_shared <listnode>(car());
        tail->next = tail;
    }
    ~queuepark(){}
    void setmaxsize(int mcs){
        maxcarsize = mcs;
    }
    int getmaxsize(){
        return maxcarsize;
    }
    bool empty(){
        return length == 0;
    }
    bool full(){
        return length >= maxcarsize;
    }
    bool push(car cara){
        if (full()) return false;
        std::shared_ptr <listnode> newnode = std::make_shared <listnode>(cara);
        newnode->next = tail->next;
        tail->next = newnode;
        tail = newnode;
        length += 1;
        return true;
    }
    bool pop(car & cara){
        if (empty()) return false;
        cara = tail->next->next->data;
        tail->next->next = tail->next->next->next;
        length -= 1;
        if (length==0) tail = tail->next;
        return true;
    }
    void display(){
        std::shared_ptr <listnode> p = tail->next->next;
        if  (empty()) return;
        while (true){
            std::cout<<p->data;
            p = p->next;
            if (p==tail->next) break;
        }
        return;
    }
    int size(){
        return length;
    }
};



#endif