#include "seqlist.h"//手动定义一个顺序表模板类，来代替<vector>
#include <iostream>
#include <fstream>
#include <sstream>//用于读取csv文件
#include <string>
#include <iomanip>
using namespace std;

class timeloc{//时间地点类
    protected:
    seqlist <string> time;
    string location;
    public:
    timeloc();
    ~timeloc();
    timeloc(seqlist <string> t,string l);
    timeloc(const timeloc & other);
    timeloc & operator =(const timeloc & other);
    seqlist <string> puttime();//获取时间顺序表
    const seqlist <string> puttime() const;//获取时间顺序表
    void settime(seqlist <string> t);//设置时间顺序表
    string putloc();//获取地点
    const string putloc() const;//获取地点
    void setloc(string l);//设置地点
    bool supermarket();//判断该对象的location是否为supermarket
    void addtime(const string t);//添加新的时间
    friend ostream & operator <<(ostream & o,const timeloc & tl);//插入运算符重载
};

void seqlist_test();//顺序表测试函数
bool comparetime(const timeloc & a,const timeloc & b);//时间比较函数
bool compareloc(const timeloc & a,const timeloc & b);//地点比较函数
void seqgetcsv(ifstream & file,seqlist <timeloc> & a);//读取csv文件并存入顺序表函数
void inserttimeloc(seqlist <timeloc> & a,seqlist <timeloc> & b);//插入新纪录函数
void displaysupermarket(seqlist <timeloc> & a,seqlist <timeloc> & b);//显示所有location为supermarket的对象
void querysupermarket(seqlist <timeloc> & a,seqlist <timeloc> & b,seqlist <timeloc> & c);//将所有location为supermarket的对象移动到新的顺序表中
void mergesupermarket(seqlist <timeloc> & a);//将顺序表中超市相同的记录合并为同一条

const string yinv1file = "./inv1.csv";
const string yinv2file = "./inv2.csv";
const string cinv1file = "./inv1c.csv";
const string cinv2file = "./inv2c.csv";

int main()
{
    char yn;
    cout<<"请选择csv文件内的语言 y:英语 c:汉语 (y/c): ";
    cin>>yn;
    string inv1file = "";
    string inv2file = "";
    if (yn=='y'){
        inv1file = yinv1file;
        inv2file = yinv2file;
    }
        else{
            inv1file = cinv1file;
            inv2file = cinv2file;
        }

    cout<<"是否要将inv1.csv文件的数据加载到主存? y:是 n:退出程序 (y/n): ";
    cin>>yn;
    if (yn!='y') return 0;
    ifstream inv1(inv1file,ios::in);
    seqlist <timeloc> pa;//用顺序表存储病人1的信息
    seqgetcsv(inv1,pa);
    inv1.close();
    cout<<"已成功将inv1.csv文件的数据加载到主存"<<endl;

    cout<<"是否要将inv2.csv文件的数据加载到主存? y:是 n:退出程序 (y/n): ";
    cin>>yn;
    if (yn!='y') return 0;
    ifstream inv2(inv2file,ios::in);
    seqlist <timeloc> pb;//用顺序表存储病人2的信息
    seqgetcsv(inv2,pb);
    inv2.close();
    cout<<"已成功将inv2.csv文件的数据加载到主存"<<endl;

    cout<<"是否要将inv1.csv文件的信息输出到屏幕上? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<" Time Location"<<endl;
        for(int i=0;i<pa.size();i++){
            cout<<pa[i]<<endl;
        }
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    cout<<"是否要将inv2.csv文件的信息输出到屏幕上? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<" Time Location"<<endl;
        for(int i=0;i<pb.size();i++){
            cout<<pb[i]<<endl;
        }
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    cout<<"是否要将新的记录插入到顺序表中? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        inserttimeloc(pa,pb);
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    cout<<"是否要查找两个顺序表中的所有地点包含超市的记录并输出? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        displaysupermarket(pa,pb);
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    seqlist <timeloc> superm;
    cout<<"是否要将两个顺序表中所有地点包含超市的记录移动到一个新的顺序表SuperM? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        querysupermarket(pa,pb,superm);
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    cout<<"是否要将顺序表SuperM中超市相同的记录合并为同一记录? y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        mergesupermarket(superm);
    }
        else if ((yn!='y')&&(yn!='n')) return 0;

    
    return 0;
}

void seqlist_test(){
    int aa[5] = {1,2,3,4,5};
    seqlist <int> a(5,aa);
    a.display();
    a.pushback(6);
    a.display();
    a.popback();
    a.display();
    a.insert(2,7);
    a.display();
    int bb[3] = {6,8,9};
    a.insert(0,3,bb);
    a.display();
    a.sort();
    a.display();
    a.erase(1,5);
    a.display();
    a.erase(4);
    a.display();
    cout<<a.size()<<endl;
    cout<<a[3]<<endl;
    seqlist <int> b(a);
    b.display();
    seqlist <int> c;
    c = b;
    c.display();
    return;
}

bool comparetime(const timeloc & a,const timeloc & b){
    return a.puttime()[0] < b.puttime()[0];
}
bool compareloc(const timeloc & a,const timeloc & b){
    if (a.putloc()!=b.putloc()) return a.putloc() < b.putloc();
        else return a.puttime()[0] < b.puttime()[0];
}
ostream & operator <<(ostream & o,const timeloc & tl){
    for(int i=0;i<tl.puttime().size();i++){
        o<<tl.puttime()[i];
        if (i!=(tl.puttime().size()-1)) o<<",";
            else o<<" ";
    }
    o<<tl.putloc();
    return o;
}

timeloc::timeloc(){
    time.clear();
    location = "";
}
timeloc::~timeloc(){}
timeloc::timeloc(seqlist <string> t,string l){
    time = t;
    location = l;
}
timeloc::timeloc(const timeloc & other){
    time = other.time;
    location = other.location;
}
timeloc & timeloc::operator =(const timeloc & other){
    if (this!=&other){
        time = other.time;
        location = other.location;
    }
    return *this;
}
seqlist <string> timeloc::puttime(){
    return time;
}
const seqlist <string> timeloc::puttime() const{
    return time;
}
void timeloc::settime(seqlist <string> t){
    time = t;
    return;
}
string timeloc::putloc(){
    return location;
}
const string timeloc::putloc() const{
    return location;
}
void timeloc::setloc(string l){
    location = l;
}
bool timeloc::supermarket(){
    for(int i=0;i<location.size();i++){
        if ((location.size()>=(i+11))&&(location[i]=='S')&&(location.substr(i,11)=="Supermarket")) return true;
            else if ((location.size()>=(i+6))&&(location.substr(i,3)=="超")&&(location.substr(i,6)=="超市")) return true;
    }
    return false;
}
void timeloc::addtime(const string t){
    time.pushback(t);
    return;
}

void seqgetcsv(ifstream & file,seqlist <timeloc> & a){
    for(string line;getline(file,line);){
        //cout<<line<<endl;
        if (line=="Time,Location") continue;//跳过表头
        istringstream iss(line);
        seqlist <string> row;//读取csv文件每一行的信息并将其存在顺序表中，以便后续处理
        for(string temp;getline(iss,temp,',');){
            row.pushback(temp);
        }
        istringstream tss(row[0]);
        seqlist <string> thourmin;//读取时钟和分钟，便于处理数据
        for(string temp;getline(tss,temp,':');){
            thourmin.pushback(temp);
        }
        if (thourmin[0].size()==1) thourmin[0] = "0" + thourmin[0];//若时钟位数为1，则在前面补'0'，以便后续时间的比较
        seqlist <string> time;
        string t = thourmin[0] + ":" + thourmin[1];
        time.pushback(t);
        string loc = row[1];
        a.pushback(timeloc(time,loc));
    }
    return;
}
void inserttimeloc(seqlist <timeloc> & a,seqlist <timeloc> & b){
    string tl;
    cout<<"请按顺序依次输入时间(如00:00) 地点(中间用空格隔开): "<<endl;
    cin.ignore();
    getline(cin,tl);
    istringstream iss(tl);
    seqlist <string> row;
    for(string temp;getline(iss,temp,' ');){//将输入的信息拆分，因为输入的地点可能有多个空格
        row.pushback(temp);
    }
    istringstream tss(row[0]);
    seqlist <string> thourmin;
    for(string temp;getline(tss,temp,':');){
        thourmin.pushback(temp);
    }
    if (thourmin[0].size()==1) thourmin[0] = "0" + thourmin[0];
    seqlist <string> time;
    string t = thourmin[0] + ":" + thourmin[1];
    time.pushback(t);
    string loc = "";
    for(int i=1;i<row.size();i++){
        loc += row[i];
        if (i!=(row.size()-1)) loc += " ";
    }
    timeloc newtl(time,loc);
    char yn;
    cout<<"请输入要插入的顺序表 1:病人1 2:病人2 (1/2): ";
    cin>>yn;
    if (yn=='1'){
        a.pushback(newtl);
        a.sort(comparetime);//使用seqlist的sort函数用指定的比较方法排序
        cout<<"请选择是否输出插入后的顺序表 y:是 n:否 (y/n): ";
        cin>>yn;
        if (yn=='y'){
            cout<<" Time Location"<<endl;
            for(int i=0;i<a.size();i++){
                cout<<a[i]<<endl;
            }
        }
    }
        else{
            b.pushback(newtl);
            b.sort(comparetime);//使用seqlist的sort函数用指定的比较方法排序
            cout<<"请选择是否输出插入后的顺序表 y:是 n:否 (y/n): ";
            cin>>yn;
            if (yn=='y'){
                cout<<" Time Location"<<endl;
                for(int i=0;i<b.size();i++){
                    cout<<b[i]<<endl;
                }
            }
        }
    return;
}
void displaysupermarket(seqlist <timeloc> & a,seqlist <timeloc> & b){
    cout<<" Time Location"<<endl;
    for(int i=0;i<a.size();i++){
        if (a[i].supermarket()) cout<<a[i]<<endl;
    }
    for(int i=0;i<b.size();i++){
        if (b[i].supermarket()) cout<<b[i]<<endl;
    }
    return;
}
void querysupermarket(seqlist <timeloc> & a,seqlist <timeloc> & b,seqlist <timeloc> & c){
    for(int i=0;i<a.size();i++){
        if (a[i].supermarket()){
            c.pushback(a[i]);
            a.erase(i);
        }
    }
    for(int i=0;i<b.size();i++){
        if (b[i].supermarket()){
            c.pushback(b[i]);
            b.erase(i);
        }
    }
    /*
    //改进代码
    int smnum = 0;
    for(int i=0;i<a.size();i++){
        a[i-smnum] = a[i];
        if (a[i].supermarket()){
            c.pushback(a[i]);
            smnum += 1;
        }
    }
    a.setsize(a.size() - smnum);
    smnum = 0;
    for(int i=0;i<b.size();i++){
        b[i-smnum] = b[i];
        if (b[i].supermarket()){
            c.pushback(b[i]);
            smnum += 1;
        }
    }
    b.setsize(b.size() - smnum);
    */
    c.sort(comparetime);
    char yn;
    cout<<"请选择是否要将顺序表SuperM的内容输出到屏幕上 y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<" Time Location"<<endl;
        for(int i=0;i<c.size();i++){
            cout<<c[i]<<endl;
        }
    }
    cout<<"请选择是否要将顺序表1的内容输出到屏幕上 y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<" Time Location"<<endl;
        for(int i=0;i<a.size();i++){
            cout<<a[i]<<endl;
        }
    }
    cout<<"请选择是否要将顺序表2的内容输出到屏幕上 y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<" Time Location"<<endl;
        for(int i=0;i<b.size();i++){
            cout<<b[i]<<endl;
        }
    }
    return;
}
void mergesupermarket(seqlist <timeloc> & a){
    a.sort(compareloc);//先排序(先按地点排序，再按时间排序，既便于顺序表去重，又不用重新排序相同超市下的时间)
    /*cout<<" Time Location"<<endl;
    for(int i=0;i<a.size();i++){
        cout<<a[i]<<endl;
    }*/
    seqlist <timeloc> b;//辅助顺序表(实际上为一个栈)
    b.pushback(a[0]);
    for(int i=1;i<a.size();i++){
        if (a[i].putloc()==b[b.size()-1].putloc()){
            b[b.size()-1].addtime(a[i].puttime()[0]);//如果和栈顶超市相同，则栈顶超市添加时间
        }
            else b.pushback(a[i]);//如果不同则直接入栈，变成栈顶超市
    }
    b.sort(comparetime);//去重后重新按时间排序
    a = b;
    char yn;
    cout<<"请选择是否要将顺序表SuperM的内容输出到屏幕上 y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        cout<<"No Time              Location"<<endl;
        for(int i=0;i<a.size();i++){
            cout<<setw(2)<<setfill(' ')<<(i+1)<<" ";//直接用重载后的插入运算符可能得不到预期的输出格式
            string alltime = "";
            for(int j=0;j<a[i].puttime().size();j++){
                alltime = alltime + a[i].puttime()[j];
                if (j==(a[i].puttime().size()-1)) alltime += "              ";
                    else alltime += ",";
            }
            cout<<alltime.substr(0,18);
            cout<<a[i].putloc()<<endl;
        }
    }
    cout<<"请选择是否要将顺序表SuperM的内容写入inv_superm.csv文件 y:是 n:否 (y/n): ";
    cin>>yn;
    if (yn=='y'){
        ofstream outinv("./inv_superm.csv",ios::out);
        outinv<<"No,Time,Location"<<endl;
        for(int i=0;i<a.size();i++){
            outinv<<(i+1)<<",";//直接用重载后的插入运算符可能得不到预期的输出格式
            for(int j=0;j<a[i].puttime().size();j++){
                outinv<<a[i].puttime()[j];
                if (j==(a[i].puttime().size()-1)) outinv<<",";
                    else outinv<<" ";
            }
            outinv<<a[i].putloc()<<endl;
        }
        outinv.close();
    }
    return;
}