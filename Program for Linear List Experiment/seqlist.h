#ifndef SEQLIST_H
#define SEQLIST_H
#pragma once

#include <cstddef>
#include <iostream>

template <class T>
class seqlist{//顺序表类，模拟vector类功能
    protected:
    static size_t maxsize;
    size_t length;
    T *p;
    void resize(size_t newsize);
    public:
    seqlist();//默认构造函数
    ~seqlist();//析构函数
    seqlist(size_t n,const T *a);//用数组构造
    seqlist(size_t n,const T a);//用n个a构造
    seqlist(const seqlist <T> &  other);//深复制拷贝构造函数
    seqlist <T> & operator =(const seqlist <T> & other);//赋值运算符重载
    size_t size();//获取元素个数
    const size_t size() const;//获取元素个数
    size_t capacity();//获取已分配的空间大小
    const size_t capacity() const;//获取已分配的空间大小
    T & operator [](size_t pos);//下标运算符重载
    const T & operator [](size_t pos) const;//下标运算符重载
    bool empty();//判断是否为空
    const bool empty() const;//判断是否为空
    bool full();//判断是否为满
    const bool full() const;//判断是否为满
    void clear();//清空顺序表
    void setsize(size_t newl);//设置顺序表长度
    void pushback(const T a);//尾插
    void popback();//尾删
    void insert(size_t pos,const T a);//在位置pos插入a
    void insert(size_t pos,size_t n,const T *a);//在位置pos插入数组a
    void erase(size_t pos);//删除位置pos的元素
    void erase(size_t a,size_t b);//删除区间[a,b)内的元素
    void sort();//默认排序
    void sort(bool (*comp)(const T &,const T &));//用指定方法排序
    void display();//显示顺序表
};

template <class T>
void seqlist <T>::resize(size_t newsize){
    if (newsize<=maxsize) return;
    maxsize = newsize;
    T *newp = new T[maxsize];
    for(size_t i=0;i<length;i++){
        newp[i] = p[i];
    }
    delete []p;
    p = newp;
    return;
}
template <class T>
size_t seqlist <T>:: maxsize = 10;
template <class T>
seqlist <T>::seqlist(){
    p = new T[maxsize];
    length = 0;
}
template <class T>
seqlist <T>::~seqlist(){
    delete []p;
    length = 0;
}
template <class T>
seqlist <T>::seqlist(size_t n,const T *a){
    p = new T[maxsize];
    if (n>=maxsize) resize(n+1);
    length = 0;
    for(size_t i=0;i<n;i++){
        if (i>=maxsize) resize(maxsize*2);
        p[i] = a[i];
        length += 1;
    }
}
template <class T>
seqlist <T>::seqlist(size_t n,const T a){
    p = new T[maxsize];
    if (n>=maxsize) resize(n+1);
    length = 0;
    for(size_t i=0;i<n;i++){
        if (i>=maxsize) resize(maxsize*2);
        p[i] = a;
        length += 1;
    }
}
template <class T>
seqlist <T>::seqlist(const seqlist <T> & other) {
    this->length = other.length;
    this->maxsize = other.maxsize;
    this->p = new T[this->maxsize];
    for(size_t i=0;i<this->length;i++) {
        this->p[i] = other.p[i];
    }
}
template <class T>
seqlist <T> & seqlist<T>::operator =(const seqlist <T> & other){
    if (this!=&other){
        delete[] this->p;
        this->length = other.length;
        this->maxsize = other.maxsize;
        this->p = new T[this->maxsize];
        for(size_t i=0;i<this->length;i++) {
            this->p[i] = other.p[i];
        }
    }
    return *this;
}
template <class T>
size_t seqlist <T>::size(){
    return length;
}
template <class T>
const size_t seqlist <T>::size() const{
    return length;
}
template <class T>
size_t seqlist <T>::capacity(){
    return maxsize;
}
template <class T>
const size_t seqlist <T>::capacity() const{
    return maxsize;
}
template <class T>
T & seqlist <T>::operator [](size_t pos){
    if (pos>=length){
        std::cerr<<pos<<" is not a correct subscript."<<std::endl;
    }
    return p[pos];
}
template <class T>
const T & seqlist <T>::operator [](size_t pos) const{
    if (pos>=length){
        std::cerr<<pos<<" is not a correct subscript."<<std::endl;
    }
    return p[pos];
}
template <class T>
bool seqlist <T>::empty(){
    return (length<=0);
}
template <class T>
const bool seqlist <T>::empty() const{
    return (length<=0);
}
template <class T>
bool seqlist <T>::full(){
    return (length>=maxsize);
}
template <class T>
const bool seqlist <T>::full() const{
    return (length>=maxsize);
}
template <class T>
void seqlist <T>::clear(){
    length = 0;
    return;
}
template <class T>
void seqlist <T>::setsize(size_t newl){
    length = newl;
    return;
}
template <class T>
void seqlist <T>::pushback(const T a){
    if (length>=maxsize) resize(maxsize*2);
    length += 1;
    p[length-1] = a;
    return;
}
template <class T>
void seqlist <T>::popback(){
    length -= 1;
    return;
}
template <class T>
void seqlist <T>::insert(size_t pos,const T a){
    if (pos>length){
        std::cerr<<pos<<" is not a correct subscript."<<std::endl;
        return;
    }
        else if (pos==length){
            pushback(a);
            return;
        }
    length += 1;
    if (length>=maxsize) resize(maxsize*2);
    for(size_t i=(length-1);i>=(pos+1);i--){
        p[i] = p[i-1];
    }
    p[pos] = a;
    return;
}
template <class T>
void seqlist <T>::insert(size_t pos,size_t n,const T *a){
    if (pos>=length){
        std::cerr<<pos<<" is not a correct subscript."<<std::endl;
        return;
    }
    length += n;
    if (length>=maxsize) resize(length+1);
    for(size_t i=(length-1);i>=(pos+n);i--){
        p[i] = p[i-n];
    }
    for(size_t i=0;i<n;i++){
        p[i+pos] = a[i];
    }
    return;
}
template <class T>
void seqlist <T>::erase(size_t pos){
    if (pos>=length){
        std::cerr<<pos<<" is not a correct subscript."<<std::endl;
        return;
    }
    for(size_t i=pos;i<(length-1);i++){
        p[i] = p[i+1];
    }
    length -= 1;
    return;
}
template <class T>
void seqlist <T>::erase(size_t a,size_t b){
    if ((a>=length)||(b>=length)||(a>=b)){
        std::cerr<<a<<" and "<<b<<" are not correct subscript."<<std::endl;
        return;
    }
    length -= (b - a);
    for(size_t i=a;i<length;i++){
        p[i] = p[i + b - a];
    }
    return;
}
template <class T>
void seqlist <T>::sort(){
    for(size_t i=0;i<(length-1);i++){
        for (size_t j=0;j<(length-i-1);j++){
            if (p[j]>=p[j+1]){
                T temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
    return;
}
template <class T>
void seqlist <T>::sort(bool (*comp)(const T &,const T &)){
    for(size_t i=0;i<(length-1);i++){
        for (size_t j=0;j<(length-i-1);j++){
            if (!comp(p[j],p[j+1])){
                T temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
    return;
}
template <class T>
void seqlist <T>::display(){
    std::cout<<"{ ";
    for(size_t i=0;i<length;i++){
        std::cout<<p[i];
        if (i!=(length-1)) std::cout<<", ";
    }
    std::cout<<" }"<<std::endl;
    return;
}


#endif