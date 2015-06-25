#ifndef LISTAEVENTOS_H
#define LISTAEVENTOS_H
#include "Evento.h"
#include <iostream>
#include <wx/String.h>
#include <fstream>
using namespace std;
class ListaEventos {
private:
wxString colaEventos;
Evento *inicio;
Evento *final;
int tamano, iteracionesParaEscribir;
ofstream xmlFile;
public:
    int i;
    void setInicio(Evento *N);
    Evento* getInicio();
    Evento* getUltimo();
    void agregarEvento(Evento *evento);
    Evento* buscar(int num);
    void eliminarEvento(int num);
    void insertarEvento(Evento *ev);
    void limpiarLista(Evento *ev);
    
    
    
//Iniciar y finalizar texto en archivo XML
    void iniciarRegistro();
    void limpiarListaAlFinalizar(Evento *nodo);

    
    ListaEventos();
    ListaEventos(const char *ubicacion);
    ListaEventos(const ListaEventos& orig);
    virtual ~ListaEventos();

};
#endif  /* LISTAEVENTOS_H */