#include <wx/msgdlg.h>
#include <wx/datetime.h>
#include "string_conversions.h"
#include <wx/string.h>
#include "Recolector.h"
#include "version.h"
#include "mxUtils.h"
#include "ConfigManager.h"
#include <iostream>
#include "Estructurador.h"
#include <windows.h>
Recolector *recolector=NULL;
Estructurador estructuraDatos;

//Datos del archivo de configuración
wxString ip_server;
wxString url_registro1;
bool farmer1;
int tamano_pila1;
//----------------------------------


wxString finEjecucion;
int sintaxisCorrecta=0;
int valor=0;
int valor2=0;
bool ejecucionCorrecta;//Variables para manejo de funcionalidades de registro de resultados
//----------------------------------REGISTRAR CODIGO DEL ARCHIVO----------------------------------
void Recolector::RegCode(wxString codigo){//Registrar código
    estructuraDatos.setCodigo(codigo);
}
//----------------------------------REGISTRAR MODIFICAR ARCHIVO----------------------------------

void Recolector::RegCreateFile(){
    estructuraDatos.setCreateFile("SinTitulo");
}
void Recolector::RegCloseFile(wxString nombreArchivo){
    estructuraDatos.setCloseFile(nombreArchivo);
}
void Recolector::RegOpenFile(wxString dirArchivo,wxString nombreArchivo){
    estructuraDatos.setOpenFile(nombreArchivo+".psc");
}
void Recolector::RegSaveFile(wxString nombreArchivo){
    estructuraDatos.setSaveFile(nombreArchivo);
}
void Recolector::RegSaveFileAs(wxString nombreArchivo){
    estructuraDatos.setSaveFileAs(nombreArchivo);
}


//----------------------------------REGISTRAR PROCESAR ARCHIVO----------------------------------

//Procesar codigo fuente(ejecutar,ejecutar paso a paso, dibujar, verificar sintaxis)
void Recolector::RegRunSource(wxString nombreArchivo){
    estructuraDatos.setRunSource(nombreArchivo);
}
void Recolector::RegRunStepStep(wxString nombreArchivo){
    estructuraDatos.setRunStepStep(nombreArchivo);
}
void Recolector::RegCreateDraw(wxString nombreArchivo){
    estructuraDatos.setCreateDraw(nombreArchivo);
}
void Recolector::RegVeriSyntax(wxString nombreArchivo){
    estructuraDatos.setVeriSyntax(nombreArchivo);
}

//Resultado del procesamiento del codigo fuente
void Recolector::RegResultSyntax(wxString error,wxString textoConError){
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
void Recolector::RegNumErrorSyntax(int numErrores){
    if(numErrores==0){
        RegResultEjec("",0);
    }
    if(numErrores!=0){
        estructuraDatos.setNumErrorSintaxis(numErrores);
    }
}
void Recolector::RegResultEjec(wxString errorEjecucion, int num){
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
//            valor=0;
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
void Recolector::RegCutCode(wxString textoRelevante, int lineaActual){
    estructuraDatos.setCutCode(textoRelevante);
}
void Recolector::RegCopyCode(wxString textoRelevante,int lineaActual){
    estructuraDatos.setCopyCode(textoRelevante);
}
void Recolector::RegPasteCode(wxString textoBorrado,int lineaActual, wxString textoPegado){
    if(textoBorrado.IsEmpty() ){
        estructuraDatos.setPasteCode(textoPegado);
    }else{
        estructuraDatos.setPasteCode(textoBorrado,textoPegado);
    }
}
void Recolector::RegUndo(){
    estructuraDatos.setUndo("Deshacer");
}
void Recolector::RegRedo(){
    estructuraDatos.setRedo("Rehacer");
}
void Recolector::RegCommentCode(int inicio, int fin){
    if (inicio==fin){
        estructuraDatos.setComment("Comentar",inicio);
    }
    else{
        estructuraDatos.setComment("Comentar",inicio,fin);
    }
}
void Recolector::RegUncommentCode(int inicio, int fin){
    if (inicio==fin){
        estructuraDatos.setUnComment("Descomentar",inicio);
    }
    else{
        estructuraDatos.setUnComment("Descomentar",inicio,fin);
    }
}
void Recolector::RegDeleteLines(int inicio, int fin, wxString textoBorrado){
    estructuraDatos.setDeleteLines("EliminarLineas",inicio,fin,textoBorrado);
}
void Recolector::RegDeleteLines(int inicio, wxString textoBorrado){
    estructuraDatos.setDeleteLines("EliminarLineas",inicio,textoBorrado);
}
void Recolector::RegDuplicateLines(int inicio, int fin, wxString textoDuplicado){
    estructuraDatos.setDuplicateLines("DuplicarLineas",inicio,fin,textoDuplicado);
}
void Recolector::RegDuplicateLines(int inicio, wxString textoDuplicado){
    estructuraDatos.setDuplicateLines("DuplicarLineas",inicio,textoDuplicado);
}
void Recolector::RegSelectAll(wxString texto){
    estructuraDatos.setselectAll("SeleccionarTodo",texto);
}
void Recolector::RegFindText(wxString textoBuscado){
    estructuraDatos.setFindText("Buscar",textoBuscado);
}
void Recolector::RegFindNext(wxString textoBuscado){
    estructuraDatos.setFindNext("BuscarSiguiente",textoBuscado);
}
void Recolector::RegFindPrev(wxString textoBuscado){
    estructuraDatos.setFindPrev("BuscarAnterior",textoBuscado);
}


//Datos asociados al archivo de configuración
void Recolector::setIpServer(wxString ip){
    ip_server=ip;
}
wxString Recolector::getIpServer(){
    return ip_server;
}
void Recolector::setUrlServer(wxString url){
    url_registro1=url;
}
wxString Recolector::getUrl(){
    return url_registro1;
}
void Recolector::setNumStack(int num){
    tamano_pila1=num;
}
int Recolector::getNumStack(){
    return tamano_pila1;
}
void Recolector::setEstadoSistemaRegistro(bool activado){
    farmer1=activado;
}
bool Recolector::getEstadoSistemaRegistro(){
    return farmer1;
}

//Funcion para cerrar tag XML "sin errores fuera de ejecucion"
void Recolector::closeTagXml(){
    estructuraDatos.terminarTags();
}


//CONSTRUCTOR
Recolector::Recolector(){
    sintaxisCorrecta=0;//Se utiliza para manejar cuándo aparecen los errores de ejecucion(solo cuando hay sintaxis correcta)
    ejecucionCorrecta=true;
    valor=valor2=0;
}

//DESTRUCTOR
Recolector::~Recolector() {
}



// wxMessageBox(_ZZ("")); MENSAJE DE PRUEBA #include <wx/msgdlg.h>