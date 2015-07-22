#ifndef MENSAJERO_H
#define	MENSAJERO_H
#include <iostream>
#include <wx/string.h>
class Mensajero {
public:
    void mostrar(std::string repositorioPath);
    wxString wxHTTP_POST(const wxString &server, const wxString &page, const wxString& parameters);
    Mensajero();
    Mensajero(const Mensajero& orig);
    virtual ~Mensajero();
private:

};

#endif	/* MENSAJERO_H */

