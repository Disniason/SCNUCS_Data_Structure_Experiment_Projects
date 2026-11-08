#ifndef STACKPARK_H
#define STACKPARK_H
#pragma once

#include "car.h"

#include <iostream>
#include <memory>
#include <iomanip>

class stackpark{
    private:
    int maxcarsize;
    int length;
    std::unique_ptr <car []> stackcar;
    public:
    stackpark(){
        maxcarsize = 0;
        length = 0;
    }
    ~stackpark(){}
    void setmaxsize(int mcs){
        maxcarsize = mcs;
        length = 0;
        stackcar = std::make_unique <car []>(maxcarsize);
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
    bool push(car & cara){
        if (full()) return false;
        stackcar[length] = cara;
        length += 1;
        return true;
    }
    bool pop(car & cara){
        if (empty()) return false;
        length -= 1;
        cara = stackcar[length];
        return true;
    }
    void display(){
        for(int i=0;i<length;i++){
            std::cout<<stackcar[i];
        }
        return;
    }
    int size(){
        return length;
    }
    bool leave(std::string id,std::string time,stackpark & temp){
        bool leavesuc = false;
        while (!empty()){
            car tempcar;
            pop(tempcar);
            if (tempcar.getid()==id){
                tempcar.setleave(time);
                std::cout<<"停留时间: "<<std::fixed<<std::setprecision(2)<<tempcar.gettotalhour()<<"小时"<<std::endl;
                std::cout<<"缴纳费用: "<<tempcar.getprice()<<"元"<<std::endl;
                leavesuc = true;
                break;
            }
                else{
                    temp.push(tempcar);
                }
        }
        while (!temp.empty()){
            car tempcar;
            temp.pop(tempcar);
            push(tempcar);
        }
        return leavesuc;
    }
    void clear(){
        maxcarsize = 0;
        length = 0;
        return;
    }
};


#endif