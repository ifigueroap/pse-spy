#ifndef EVENTO_H
#define	EVENTO_H
#include <wx/String.h>
#include <wx/msgdlg.h>
class Evento {
public:
    void setEvento(wxString ev);
    void setSiguiente(Evento *sig);
    wxString getEvento();
    Evento *getSiguiente();
    void borrarTodos();
    Evento();
    Evento(const Evento& orig);
    virtual ~Evento();
    wxString eventoXML;
    Evento *siguiente;
private:
    
};

#endif	/* EVENTO_H */