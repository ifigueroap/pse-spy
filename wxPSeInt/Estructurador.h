#ifndef ESTRUCTURADOR_H
#define	ESTRUCTURADOR_H
#include <iostream>
#include <wx/String.h>
#include <fstream>
using namespace std;
class Estructurador {//SETEAR DATOS A XML
private:
    string codigo;
    ofstream archivoXML;
    wxString evento;
public:
//GESTIONAR ARCHIVO
    void setCodigo(wxString codigo);
    string getCodigo();
    void setCreateFile(wxString nombreArchivo);
    void setCloseFile(wxString nombreArchivo);
    void setOpenFile(wxString nombreArchivo);
    void setSaveFile(wxString nombreArchivo);
    void setSaveFileAs(wxString nombreArchivo);
    
//PROCESAR ARCHIVO

    void setRunSource(wxString nombreArchivo);
    void setErrorSintaxis(wxString lineaError,wxString descripcionError,wxString textoError);
    void setNumErrorSintaxis(int numError);
    void setSinErrorSintaxis();
    void setErrorEjecucion(wxString descripcionError);
    void setErrorEjecucion();
    void setVeriSyntax(wxString nombreArchivo);
    void setRunStepStep(wxString nombreArchivo);
    void setCreateDraw(wxString nombreArchivo);
    
    
//EDICIONES DEL EDITOR
    void setCutCode(wxString textoCortado);
    void setCopyCode(wxString textoCopiado);
    void setPasteCode(wxString textoPegado);
    void setPasteCode(wxString textoBorrado, wxString textoPegado);
    void setUndo(wxString nombreAccion);
    void setRedo(wxString nombreAccion);
    void setComment(wxString nombreAccion, int inicio);
    void setComment(wxString nombreAccion, int inicio,int fin);
    void setUnComment(wxString nombreAccion, int inicio);
    void setUnComment(wxString nombreAccion, int inicio,int fin);
    void setDeleteLines(wxString nombreAccion,int inicio, wxString textoEliminado);
    void setDeleteLines(wxString nombreAccion,int inicio,int final, wxString textoEliminado);
    void setDuplicateLines(wxString nombreAccion,int inicio, wxString textoEliminado);
    void setDuplicateLines(wxString nombreAccion,int inicio,int final, wxString textoEliminado);
    void setselectAll(wxString nombreAccion, wxString codigoFuente);
    void setFindText(wxString nombreAccion, wxString textoBuscado);
    void setFindNext(wxString nombreAccion, wxString textoBuscado);
    void setFindPrev(wxString nombreAccion, wxString textoBuscado);
    
    string getRepositorioPath();
    void terminarTags();
    Estructurador();
    Estructurador(const Estructurador& orig);
    virtual ~Estructurador();
    

};

#endif	/* ESTRUCTURADOR_H */

