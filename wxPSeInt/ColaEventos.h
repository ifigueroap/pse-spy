#ifndef COLAEVENTOS_H
#define	COLAEVENTOS_H
#include <iostream>
#include <wx/String.h>
#include <fstream>
using namespace std;
class ColaEventos {
public:
    void insertar(wxString elemento);
    void agregarEventoCola(wxString elemento);
    void escribirEnXml();
    void insertarfin(wxString elemento);
    void iniciarTagXml();
    ColaEventos(const char *ubicacion);
    ColaEventos();
    ColaEventos(const ColaEventos& orig);
    virtual ~ColaEventos();
private:
    ofstream xmlFile;
};

#endif	/* COLAEVENTOS_H */

