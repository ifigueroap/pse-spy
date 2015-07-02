//#ifndef DATETIME_H
//#define DATETIME_H

#include<iostream>
#include<windows.h>
#include<tchar.h>
#include<sstream>
#include <stdlib.h>
#include <time.h>
#include<string>

using namespace std;

string IntToString(int number){
  ostringstream oss;
  oss<< number;
  return oss.str();
}
string getFecha(){
    SYSTEMTIME time;
    GetLocalTime(&time);
    string fecha;
    fecha=IntToString(time.wYear);
    if(time.wMonth<10){
        fecha=fecha+"-0"+IntToString(time.wMonth);
    }else{
        fecha=fecha+"-"+IntToString(time.wMonth);
    }
    if(time.wDay<10){
        fecha=fecha+"-0"+IntToString(time.wDay);
    }else{
        fecha=fecha+"-"+IntToString(time.wDay);
    }
    if(time.wHour<10){
        fecha=fecha+"T0"+IntToString(time.wHour);
    }else{
        fecha=fecha+"T"+IntToString(time.wHour);
    }
    if(time.wMinute<10){
        fecha=fecha+":0"+IntToString(time.wMinute);
    }else{
        fecha=fecha+":"+IntToString(time.wMinute);
    }
    if(time.wSecond<10){
        fecha=fecha+":0"+IntToString(time.wSecond);
    }else{
        fecha=fecha+":"+IntToString(time.wSecond);
    }
    return fecha;
}
string getFechaName(){
    SYSTEMTIME time;
    GetLocalTime(&time);
    string fecha;
    fecha=IntToString(time.wYear);
    fecha=fecha+IntToString(time.wMonth);
    fecha=fecha+IntToString(time.wDay);
    fecha=fecha+IntToString(time.wHour);
    fecha=fecha+IntToString(time.wMinute);
    fecha=fecha+IntToString(time.wSecond);;
    int num, c;
    num=1+rand()%10;
    fecha=fecha.append(IntToString(num));
    fecha=fecha.append(".xml");
    return fecha;
}