#ifndef CAR_H
#define CAR_H
#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cmath>

class car{
    private:
    std::string carid;
    std::string arrivetime;
    std::string leavetime;
    double totalhour;
    int price;
    public:
    car(){
        carid = "";
        arrivetime = "00:00";
        leavetime = "00:00";
        totalhour = 0;
        price = 0;
    }
    car(std::string id,std::string at){
        carid = id;
        arrivetime = at;
        leavetime = "00:00";
        totalhour = 0;
        price = 0;
    }
    ~car(){}
    car(const car & other){
        carid = other.carid;
        arrivetime = other.arrivetime;
        leavetime = other.leavetime;
        totalhour = other.totalhour;
        price = other.price;
    }
    car & operator =(const car & other){
        if (this!=&other){
            carid = other.carid;
            arrivetime = other.arrivetime;
            leavetime = other.leavetime;
            totalhour = other.totalhour;
            price = other.price;
        }
        return *this;
    }
    friend std::ostream & operator <<(std::ostream & o,const car & cara);
    void setid(std::string id){
        carid = id;
        return;
    }
    std::string getid(){
        return carid;
    }
    void setarrive(std::string at){
        arrivetime = at;
        return;
    }
    std::string getarrive(){
        return arrivetime;
    }
    void setleave(std::string lt){
        leavetime = lt;
        double arrivehour = stod(arrivetime.substr(0,2)) + stod(arrivetime.substr(3,2)) / 60;
        double leavehour = stod(leavetime.substr(0,2)) + stod(leavetime.substr(3,2)) / 60;
        if (arrivehour>leavehour) leavehour += 24;
        totalhour = leavehour - arrivehour;
        price = (int)ceil(totalhour) * 3;
        return;
    }
    std::string getleave(){
        return leavetime;
    }
    double gettotalhour(){
        return totalhour;
    }
    int getprice(){
        return price;
    }
};
std::ostream & operator <<(std::ostream & o,const car & cara){
    o<<"车牌号: "<<cara.carid<<" 进停车场时间: "<<cara.arrivetime<<" 出停车场时间: "<<cara.leavetime<<" 总时长: "<<std::fixed<<std::setprecision(2)<<cara.totalhour<<"小时 价格: "<<cara.price<<std::endl;
    return o;
}

#endif