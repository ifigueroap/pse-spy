#include <wx/msgdlg.h>
#include <wx/datetime.h>
#include "string_conversions.h"
#include <wx/string.h>
#include "LoggerDaniel.h"
#include "version.h"
#include "mxUtils.h"
#include "ConfigManager.h"
#include <iostream>
#include "Estructurador.h"
#include <windows.h>
LoggerDaniel *loggerDaniel=NULL;
Estructurador estructuraDatos;
wxString finEjecucion;
int sintaxisCorrecta=0;
int valor=0;
int valor2=0;
bool ejecucionCorrecta;//Variables para manejo de funcionalidades de registro de resultados
//----------------------------------REGISTRAR CODIGO DEL ARCHIVO----------------------------------
void LoggerDaniel::RegCode(wxString codigo){//Registrar código
    estructuraDatos.setCodigo(codigo);
}
//----------------------------------REGISTRAR MODIFICAR ARCHIVO----------------------------------

void LoggerDaniel::RegCreateFile(){
    estructuraDatos.setCreateFile("SinTitulo");
}
void LoggerDaniel::RegCloseFile(wxString nombreArchivo){
    estructuraDatos.setCloseFile(nombreArchivo);
}
void LoggerDaniel::RegOpenFile(wxString dirArchivo,wxString nombreArchivo){
    estructuraDatos.setOpenFile(nombreArchivo+".psc");
}
void LoggerDaniel::RegSaveFile(wxString nombreArchivo){
    estructuraDatos.setSaveFile(nombreArchivo);
}
void LoggerDaniel::RegSaveFileAs(wxString nombreArchivo){
    estructuraDatos.setSaveFileAs(nombreArchivo);
}


//----------------------------------REGISTRAR PROCESAR ARCHIVO----------------------------------

//Procesar codigo fuente(ejecutar,ejecutar paso a paso, dibujar, verificar sintaxis)
void LoggerDaniel::RegRunSource(wxString nombreArchivo){
    estructuraDatos.setRunSource(nombreArchivo);
}
void LoggerDaniel::RegRunStepStep(wxString nombreArchivo){
    estructuraDatos.setRunStepStep(nombreArchivo);
}
void LoggerDaniel::RegCreateDraw(wxString nombreArchivo){
    estructuraDatos.setCreateDraw(nombreArchivo);
}
void LoggerDaniel::RegVeriSyntax(wxString nombreArchivo){
    estructuraDatos.setVeriSyntax(nombreArchivo);
}

//Resultado del procesamiento del codigo fuente
void LoggerDaniel::RegResultSyntax(wxString error,wxString textoConError){
    wxString lineaError;
    if(error=="NO"){//NO encuentra errores de sintaxis
        sintaxisCorrecta=1;//Permite que se registren errores de ejecución
        estructuraDatos.setErrorSintaxis("NO","0","0");
        RegNumErrorSyntax(0);
    }
    else{
        sintaxisCorrecta=0;//Hay errores de sintaxis, no se puede ejecutar
        error.Remove(6,20);//Quita texto basura del string
        lineaError=error.substr(3,3);//Entrega la linea errada
        error.Remove(0,6);//ahora error entrega la descripcion del error encontrado
        if(textoConError.Last()=='\n'){
            textoConError.Replace("\n","_",true);
            textoConError.Remove(textoConError.Length()-2,textoConError.length());
        }
        estructuraDatos.setErrorSintaxis(lineaError,error,textoConError);
    }
}
void LoggerDaniel::RegNumErrorSyntax(int numErrores){
    if(numErrores==0){
        RegResultEjec("",0);
    }
    if(numErrores!=0){
        estructuraDatos.setNumErrorSintaxis(numErrores);
    }
}
void LoggerDaniel::RegResultEjec(wxString errorEjecucion, int num){
    if(sintaxisCorrecta==1){//Sólo recibe errores de EJECUCION
        if(num==1){
            valor=1;
            errorEjecucion.Remove(0,11);//Quita texto basura del string
            finEjecucion.clear();
            finEjecucion<<"\t\t\t\t<ErrorEncontrado>\n"
            "\t\t\t\t\t<TipoError>ErrorEjecucion</TipoError>\n"
            "\t\t\t\t\t<DescripcionErrorEj><![CDATA["+errorEjecucion+"]]></DescripcionErrorEj>\n"
            "\t\t\t\t</ErrorEncontrado>\n"
            "\t\t\t\t<NumErrores>1</NumErrores>\n"
            "\t\t\t</Procesar>\n"
            "\t\t</EventoRegistrado>\n";
        }
        if(num==0){
            if(ejecucionCorrecta==true){
                finEjecucion.clear();
                finEjecucion<<"\t\t\t</Procesar>\n"
                    "\t\t</EventoRegistrado>";
            }
            ejecucionCorrecta=true;
        }
        if(num==2){
            if(valor==1){
                estructuraDatos.setErrorEjecucion(finEjecucion);
                valor=0;
            }else{
                valor=0;
                estructuraDatos.setErrorEjecucion(finEjecucion);
            }
        }
    }
}


//----------------------------------REGISTRAR EDICION DEL EDITOR ----------------------------------

//(Cortar, copiar, pegar, deshacer, rehacer,etc.)
void LoggerDaniel::RegCutCode(wxString textoRelevante, int lineaActual){
    estructuraDatos.setCutCode(textoRelevante);
}
void LoggerDaniel::RegCopyCode(wxString textoRelevante,int lineaActual){
    estructuraDatos.setCopyCode(textoRelevante);
}
void LoggerDaniel::RegPasteCode(wxString textoBorrado,int lineaActual, wxString textoPegado){
    if(textoBorrado.IsEmpty() ){
        estructuraDatos.setPasteCode(textoPegado);
    }else{
        estructuraDatos.setPasteCode(textoBorrado,textoPegado);
    }
}
void LoggerDaniel::RegUndo(){
    estructuraDatos.setUndo("Deshacer");
}
void LoggerDaniel::RegRedo(){
    estructuraDatos.setRedo("Rehacer");
}
void LoggerDaniel::RegCommentCode(int inicio, int fin){
    if (inicio==fin){
        estructuraDatos.setComment("Comentar",inicio);
    }
    else{
        estructuraDatos.setComment("Comentar",inicio,fin);
    }
}
void LoggerDaniel::RegUncommentCode(int inicio, int fin){
    if (inicio==fin){
        estructuraDatos.setUnComment("Descomentar",inicio);
    }
    else{
        estructuraDatos.setUnComment("Descomentar",inicio,fin);
    }
}
void LoggerDaniel::RegDeleteLines(int inicio, int fin, wxString textoBorrado){
    estructuraDatos.setDeleteLines("EliminarLineas",inicio,fin,textoBorrado);
}
void LoggerDaniel::RegDeleteLines(int inicio, wxString textoBorrado){
    estructuraDatos.setDeleteLines("EliminarLineas",inicio,textoBorrado);
}
void LoggerDaniel::RegDuplicateLines(int inicio, int fin, wxString textoDuplicado){
    estructuraDatos.setDuplicateLines("DuplicarLineas",inicio,fin,textoDuplicado);
}
void LoggerDaniel::RegDuplicateLines(int inicio, wxString textoDuplicado){
    estructuraDatos.setDuplicateLines("DuplicarLineas",inicio,textoDuplicado);
}
void LoggerDaniel::RegSelectAll(wxString texto){
    estructuraDatos.setselectAll("SeleccionarTodo",texto);
}
void LoggerDaniel::RegFindText(wxString textoBuscado){
    estructuraDatos.setFindText("Buscar",textoBuscado);
}
void LoggerDaniel::RegFindNext(wxString textoBuscado){
    estructuraDatos.setFindNext("BuscarSiguiente",textoBuscado);
}
void LoggerDaniel::RegFindPrev(wxString textoBuscado){
    estructuraDatos.setFindPrev("BuscarAnterior",textoBuscado);
}


//Funcion para cerrar tag XML "sin errores fuera de ejecucion"
void LoggerDaniel::closeTagXml(){
    estructuraDatos.terminarTags();
}


//CONSTRUCTOR
LoggerDaniel::LoggerDaniel(){
    sintaxisCorrecta=0;//Se utiliza para manejar cuándo aparecen los errores de ejecucion(solo cuando hay sintaxis correcta)
    ejecucionCorrecta=true;
    valor=valor2=0;
}

//DESTRUCTOR
LoggerDaniel::~LoggerDaniel() {
}



// wxMessageBox(_ZZ("")); MENSAJE DE PRUEBA #include <wx/msgdlg.h>