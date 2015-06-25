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
    fecha=fecha+":"+IntToString(time.wMonth);
    fecha=fecha+":"+IntToString(time.wDay);
    fecha=fecha+"\t"+IntToString(time.wHour);
    fecha=fecha+":"+IntToString(time.wMinute);
    fecha=fecha+":"+IntToString(time.wSecond);
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
    return fecha;
}