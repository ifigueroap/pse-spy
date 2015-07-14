#include "ColaEventos.h"
#include <wx/msgdlg.h>
#include <queue>
#include <iostream>
using namespace std;
int numElementos,tamanoMaxStack;
queue <string> cola;
string auxiliar,nombreArchivo,ubicacion;

void ColaEventos::insertar(wxString elemento){
    if(numElementos<tamanoMaxStack-1){
        numElementos++;
        agregarEventoCola(elemento);
    }else{
        agregarEventoCola(elemento);
        escribirEnXml();
        numElementos=0;
    }
}
void ColaEventos::agregarEventoCola(wxString elemento){
    cola.push(elemento.mb_str());
}

void ColaEventos::escribirEnXml(){
    while(!cola.empty()){
        auxiliar=auxiliar+cola.front();
        cola.pop();
    }
    xmlFile<<auxiliar<<endl;
    auxiliar.clear();
}



void ColaEventos::iniciarTagXml(){
    xmlFile<<"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n"
            "<SecuenciaEventos>"<<endl;
    numElementos=0;
}
void ColaEventos::insertarfin(wxString elemento){
    numElementos=tamanoMaxStack+1;
    insertar(elemento);
    xmlFile.close();
}
void ColaEventos::setRepositorioXML(string repositorio, string nombre){
    repositorioXML.open(repositorio.c_str(),ios::app);
    if(repositorioXML.is_open()){
        repositorioXML<<"PseSpy-"+nombre+".xml"<<endl;
        repositorioXML.close();
    }else{
        std::cout<<"No se abrio el archivo";
    }
}

ColaEventos::ColaEventos(const char *ubicacion) {
    xmlFile.open(ubicacion,ios::app);
    if (!xmlFile.is_open()){
        wxMessageBox("Error al abrir archivo de registro");
    }
    numElementos=0;
    tamanoMaxStack=2;
    auxiliar.clear();
}

ColaEventos::ColaEventos() {
}

ColaEventos::ColaEventos(const ColaEventos& orig) {
}

ColaEventos::~ColaEventos() {
}

