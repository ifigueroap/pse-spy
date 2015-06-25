#include "ListaEventos.h"
#include "Evento.h"
#include "Estructurador.h"
#include <wx/msgdlg.h>
#include <wx/String.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
using namespace std;
ListaEventos *listaeventos;
void ListaEventos::setInicio(Evento *N){
    inicio=N;
    final=N;
    inicio->siguiente=NULL;
}
Evento* ListaEventos::getInicio(){
    return inicio;
}
Evento* ListaEventos::getUltimo(){
    Evento *ultimo=inicio;
    while(ultimo->siguiente!=NULL){
        ultimo=ultimo->siguiente;
    }
    return ultimo;
}
void ListaEventos::agregarEvento(Evento *nuevo){
    if(inicio==NULL){
        setInicio(nuevo);
        tamano=1;
    }else{
        final->siguiente=nuevo;
        nuevo->siguiente=NULL;
        final=nuevo;
        tamano++;
    }
}
void ListaEventos::insertarEvento(Evento *ev){
    if(tamano<iteracionesParaEscribir-1){
        agregarEvento(ev);
    }else{
        agregarEvento(ev);
        limpiarLista(inicio);
        inicio=final=NULL;
    }
}
void ListaEventos::limpiarLista(Evento *ev){
    Evento *auxiliar=ev;
    Evento *anterior=ev=NULL;
    while(auxiliar){
        if(auxiliar->siguiente){
        colaEventos.Append(auxiliar->eventoXML);
        }else{
            colaEventos.Append(auxiliar->eventoXML);
            xmlFile<<colaEventos;
            delete ev;
            colaEventos.clear();
        }
        anterior=auxiliar;
        auxiliar=auxiliar->siguiente;
//        wxMessageBox(anterior->eventoXML);
        delete anterior;
    }
    tamano=0;
}
void ListaEventos::iniciarRegistro(){
    xmlFile<<"<Registro>\n"
            "\t<SecuenciaEventos>"<<endl;
}
void ListaEventos::limpiarListaAlFinalizar(Evento *nodo){
    tamano=iteracionesParaEscribir+1;
    insertarEvento(nodo);    
}

Evento* ListaEventos::buscar(int num){
    
}
void ListaEventos::eliminarEvento(int num){
    
}

ListaEventos::ListaEventos(const char *ubicacion) {
    xmlFile.open(ubicacion,ios::app);
    if (xmlFile.is_open()){listaeventos=this;}
    inicio=NULL;
    tamano=0;
    iteracionesParaEscribir=5;
}

ListaEventos::ListaEventos(const ListaEventos& orig) {
}
ListaEventos::~ListaEventos() {
}
ListaEventos::ListaEventos(){
}