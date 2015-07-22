#ifndef MENSAJERO_H
#define	MENSAJERO_H
#include <iostream>
#include <wx/string.h>
class Mensajero {
public:
    void recogerDatosLocal(std::string repositorioPath);
    wxString Enviar(const wxString &server, const wxString &page, const wxString& parameters);
    void conectar(const wxString &ipServidor);
    void desconectar();
    Mensajero();
    Mensajero(const Mensajero& orig);
    virtual ~Mensajero();
private:

};

#endif	/* MENSAJERO_H */

