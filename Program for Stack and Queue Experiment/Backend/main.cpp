#include "car.h"
#include "stackpark.h"
#include "queuepark.h"

#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int main()
{
    SetConsoleOutputCP(CP_UTF8);
    string cmd;
    stackpark spark;
    queuepark qpark;
    stackpark temppark;
    while(getline(cin,cmd)){
        istringstream iss(cmd);
        vector <string> row;
        for(string temp;getline(iss,temp,' ');){
            row.push_back(temp);
        }
        if ((row.size()>=2)&&(row[0]=="PSQE")){
            if ((row.size()>=3)&&(row[1]=="setstackmaxsize")){
                spark.setmaxsize(stoi(row[2]));
                cout<<"成功设置停车场容量: "<<spark.getmaxsize()<<endl;
            }
                else if ((row.size()>=3)&&(row[1]=="setqueuemaxsize")){
                    qpark.setmaxsize(stoi(row[2]));
                    cout<<"成功设置候车场容量: "<<qpark.getmaxsize()<<endl;
                }
                else if ((row.size()>=4)&&(row[1]=="cararrive")){
                    car newcar(row[2],row[3]);
                    if (spark.full()){
                        if (qpark.push(newcar)) cout<<"候车场的位置: "<<qpark.size()<<endl;
                            else cout<<"候车场已满!"<<endl;
                    }
                        else{
                            spark.push(newcar);
                            cout<<"停车场的位置: "<<spark.size()<<endl;
                        }
                }
                else if ((row.size()>=4)&&(row[1]=="carleave")){
                    temppark.setmaxsize(spark.size());
                    if (!spark.leave(row[2],row[3],temppark)){
                        cout<<"请检查车牌号是否存在!"<<endl;
                        continue;
                    }
                    temppark.clear();
                    if (!spark.full()&&(!qpark.empty())){
                        car tempcar;
                        qpark.pop(tempcar);
                        tempcar.setarrive(row[3]);
                        spark.push(tempcar);
                    }
                }
                else if (row[1]=="displaystackcar"){
                    spark.display();
                }
                else if (row[1]=="displayqueuecar"){
                    qpark.display();
                }
                else if (row[1]=="exit") break;
                else cout<<"\""<<row[1]<<"\" is an unknown command."<<endl;
        }
            else cout<<"\""<<row[0]<<"\" is not an internal command."<<endl;
    }
    return 0;
}