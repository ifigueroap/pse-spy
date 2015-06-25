#include "Evento.h"
#include <wx/msgdlg.h>

void Evento::setEvento(wxString ev){
    eventoXML=ev;
}
void Evento::setSiguiente(Evento *sig){
    siguiente=sig;
}
wxString Evento::getEvento(){
    return eventoXML;
}
Evento *Evento::getSiguiente(){
    return siguiente;
}

    
    
void Evento::borrarTodos(){
    if (siguiente)
        siguiente->borrarTodos();
    delete this;
}
 
Evento::Evento() {
    siguiente=NULL;
}

Evento::Evento(const Evento& orig) {
}

Evento::~Evento() {
}