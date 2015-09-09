#include "ColaEventos.h"
#include <wx/msgdlg.h>
#include <queue>
#include <iostream>
#include "ConfigManager.h"
#include "Recolector.h"

using namespace std;

int numElementos,tamanoMaxStack;
queue <string> cola;
string auxiliar,nombreArchivo,ubicacion;
bool activado=recolector->getEstadoSistemaRegistro();

void ColaEventos::insertar(wxString elemento){
    setActivar(recolector->getEstadoSistemaRegistro());
        if(numElementos<tamanoMaxStack-1){ //Si el tamaño es menor al requerido, se agrega a la cola
            numElementos++;
            if(getActivar()){
                agregarEventoCola(elemento);
            }
        }else{
            if(getActivar()){
                agregarEventoCola(elemento);// Si el tamaño es el indicado, se escribe en el archivo(en disco)
                escribirEnXml();
                numElementos=0;
            }
        }
}
void ColaEventos::agregarEventoCola(wxString elemento){
    //Poner para obtener valor 
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
    tamanoMaxStack=recolector->getNumStack();
    numElementos=tamanoMaxStack+1;
    agregarEventoCola(elemento);// Si el tamaño es el indicado, se escribe en el archivo(en disco)
    escribirEnXml();
    xmlFile.close();
    
//    wxMessageBox("SI");
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
    auxiliar.clear();
}
void ColaEventos::setActivar(bool act){
    activado=act;
}
bool ColaEventos::getActivar(){
    return activado;
}

ColaEventos::ColaEventos() {
}

ColaEventos::ColaEventos(const ColaEventos& orig) {
}

ColaEventos::~ColaEventos() {
}

