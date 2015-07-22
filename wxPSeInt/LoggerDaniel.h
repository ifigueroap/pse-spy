#ifndef LOGGER_Daniel
#define LOGGER_Daniel
//#include <string>
#include <fstream>
//#include <wx/string.h>
using namespace std;
class LoggerDaniel {
private:
protected:
public:    
//Registrar codigo del archivo
    void RegCode(wxString codigo);
//Gestionar archivos ----------------------------------
    void RegCreateFile();
    void RegCloseFile(wxString nombreArchivo);
    void RegOpenFile(wxString dirArchivo, wxString nombreArchivo);
    void RegSaveFile(wxString nombreArchivo);
    void RegSaveFileAs(wxString nombreArchivo);
//Procesar archivos -----------------------------------
    void RegRunSource(wxString nombreArchivo);
    void RegRunStepStep(wxString nombreArchivo);
    void RegVeriSyntax(wxString nombreArchivo);
    void RegCreateDraw(wxString nombreArchivo);
    void RegNoErrorSyntax();
    void RegResultSyntax(wxString error, wxString textoConError);
    void RegNumErrorSyntax(int numErrores);
    void RegResultEjec(wxString errorEjecucion,int num);

//Ediciones del editor
    void RegCutCode(wxString textoRelevante,int lineaActual);
    void RegCopyCode(wxString textoRelevante, int lineaActual);
    void RegPasteCode(wxString textoRelevante,int lineaActual,wxString textoPegado);
    void RegUndo();
    void RegRedo();
    void RegCommentCode(int inicio, int fin);
    void RegUncommentCode(int inicio, int fin);
    void RegDeleteLines(int inicio, int fin, wxString textoBorrado);
    void RegDeleteLines(int inicio, wxString textoBorrado);
    void RegDuplicateLines(int inicio, int fin, wxString textoDuplicado);
    void RegDuplicateLines(int inicio, wxString textoDuplicado);
    void RegSelectAll(wxString texto);
    void RegFindText(wxString textoBuscado);
    void RegFindNext(wxString textoBuscado);
    void RegFindPrev(wxString textoBuscado);
    
    void closeTagXml();
    bool getVerificarSintaxis();
    LoggerDaniel();
    ~LoggerDaniel();
};

//inline wxString &operator<<(wxString &s, void *p) { return s<<reinterpret_cast<unsigned long>(p); }

extern LoggerDaniel *loggerDaniel;
#ifdef DEBUG
	#define _LOG(x) cerr<<x<<endl; if (logger) logger->Write(wxString()<<x)
#else
	#define __LOG(x) if (loggerDaniel) loggerDaniel->Write(wxString()<<x)
#endif

#endif

