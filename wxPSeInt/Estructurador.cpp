#include "Estructurador.h"
#include <wx/String.h>
#include "LoggerDaniel.h"
#include <wx/msgdlg.h>
#include "dateTime.h"
#include "ListaEventos.h"

using namespace std;

// wxMessageBox(_ZZ("")); MENSAJE DE PRUEBA #include <wx/msgdlg.h>
Estructurador *estructurador=NULL;
//Evento *nodo=new Evento();

string u=estructurador->ubicacion();
const char *ubicarXml=u.c_str();
ListaEventos lista(ubicarXml);
//GESTIONAR ARCHIVO
//-------------------------------------------------------------------------------
void Estructurador::setCodigo(wxString codigo){
    this->codigo=(string)codigo.mb_str();
}
void Estructurador::setCreateFile(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Gestionar>\n"
            "\t\t\t\t<NombreAccion>CrearArchivo</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>SinTitulo</NombreArchivo>\n"
            "\t\t\t</Gestionar>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setOpenFile(wxString nombreArchivo){
    nombreArchivo=nombreArchivo.AfterLast('\\');
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Gestionar>\n"
            "\t\t\t\t<NombreAccion>AbrirArchivo</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n"
            "\t\t\t</Gestionar>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setCloseFile(wxString nombreArchivo){
    nombreArchivo=nombreArchivo.AfterLast('\\');
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Gestionar>\n"
            "\t\t\t\t<NombreAccion>CerrarArchivo</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n"
            "\t\t\t</Gestionar>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setSaveFile(wxString nombreArchivo){
    nombreArchivo=nombreArchivo.AfterLast('\\');
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Gestionar>\n"
            "\t\t\t\t<NombreAccion>GuardarArchivo</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n"
            "\t\t\t</Gestionar>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setSaveFileAs(wxString nombreArchivo){
    nombreArchivo=nombreArchivo.AfterLast('\\');
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Gestionar>\n"
            "\t\t\t\t<NombreAccion>GuardarArchivoComo</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n"
            "\t\t\t</Gestionar>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}

//PROCESAR ARCHIVO
//-------------------------------------------------------------------------------
void Estructurador::setLineaError(wxString lineaErrada,wxString descripcionError,wxString textoConError){
    this->lineaError=lineaError;
    this->descripcionError=descripcionError;
    this->textoConError=textoConError;
}
void Estructurador::setRunSource(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>Ejecutar</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n";
//    archivoXML<<evento<<endl;
//    evento.clear();
}
void Estructurador::setErrorSintaxis(wxString lineaError,wxString descripcionError,wxString textoError){
    if(lineaError=="NO"){
    }else{
        evento.Append("\t\t\t\t\t<ErrorEncontrado>\n"
            "\t\t\t\t\t\t<NombreError>ErrorSintaxis</NombreError>\n"
            "\t\t\t\t\t\t<LineaErrada>"+lineaError+"</LineaErrada>\n"
            "\t\t\t\t\t\t<InstruccionErrada>"+textoError+" </InstruccionErrada>\n"
            "\t\t\t\t\t\t<DescripcionError>"+descripcionError+"</DescripcionError>\n"
            "\t\t\t\t\t</ErrorEncontrado>");
//        archivoXML<<evento<<endl;
    }
}
void Estructurador::setNumErrorSintaxis(int numError){
    wxString numero;
    numero<<numError;
    evento.Append("\n\t\t\t\t<NumErrores>"+numero+"</NumErrores>\n"
            "\t\t\t</Procesar>\n"
            "\t\t</EventoRegistrado>\n");
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setErrorEjecucion(wxString descripcionError){
    evento.Append(descripcionError);
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setErrorEjecucion(){
    evento.clear();
    evento.Append("\t\t\t</Procesar>\n"
            "\t\t</EventoRegistrado>\n");
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
//    archivoXML<<evento<<endl;
}
void Estructurador::setVeriSyntax(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>VerificarSintaxis</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n";
//    archivoXML<<evento<<endl;
//    evento.clear();
}
void Estructurador::setRunStepStep(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>EjecutarPasoPaso</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n";
}
void Estructurador::setCreateDraw(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>DibujarDiagrama</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n";
//    archivoXML<<evento<<endl;
}

//ACCIONES DEL EDITOR
//-------------------------------------------------------------------------------
void Estructurador::setCutCode(wxString textoCortado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Cortar</NombreAccion>\n"
            "\t\t\t\t<TextoAgregado>"+textoCortado+"</TextoAgregado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setCopyCode(wxString textoCopiado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Copiar</NombreAccion>\n"
            "\t\t\t\t<TextoAgregado>"+textoCopiado+"</TextoAgregado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setPasteCode(wxString textoPegado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Pegar</NombreAccion>\n"
            "\t\t\t\t<TextoAgregado>"+textoPegado+"</TextoAgregado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setPasteCode(wxString textoBorrado,wxString textoPegado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Pegar</NombreAccion>\n"
            "\t\t\t\t<TextoEliminado>"+textoBorrado+"</TextoEliminado>\n"
            "\t\t\t\t<TextoAgregado>"+textoPegado+"</TextoAgregado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setUndo(wxString nombreAccion){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setRedo(wxString nombreAccion){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setComment(wxString nombreAccion, int inicio){
    wxString ini;
    ini<<inicio;
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaComentada>"+ini+"</LineaComentada>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setComment(wxString nombreAccion, int inicio,int fin){
    evento.clear();
    wxString ini,fn;
    ini<<inicio;
    fn<<fin;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaComentada>"+ini+"-"+fn+"</LineaComentada>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setUnComment(wxString nombreAccion, int inicio){
    evento.clear();
    wxString ini;
    ini<<inicio;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaComentada>"+ini+"</LineaComentada>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setUnComment(wxString nombreAccion, int inicio,int fin){
    evento.clear();
    wxString ini,fn;
    ini<<inicio;
    fn<<fin;
        evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaComentada>"+ini+"-"+fn+"</LineaComentada>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//        archivoXML<<evento<<endl;
        Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
        evento.clear();
}
void Estructurador::setDeleteLines(wxString nombreAccion,int inicio, wxString textoEliminado){
    evento.clear();
    wxString ini;
    ini<<inicio;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaElminada>"+ini+"</LineaElminada>\n"
            "\t\t\t\t<textoEliminado>"+textoEliminado+"</textoEliminado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setDeleteLines(wxString nombreAccion,int inicio,int final, wxString textoEliminado){
    evento.clear();
    wxString ini,fn;
    ini<<inicio;
    fn<<final;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaEliminada>"+ini+"-"+fn+"</LineaElminada>\n"
            "\t\t\t\t<textoEliminado>"+textoEliminado+"</textoEliminado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setDuplicateLines(wxString nombreAccion,int inicio, wxString textoDuplicado){
    evento.clear();
    wxString ini;
    ini<<inicio;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaDuplicada>"+ini+"</LineaDuplicada>\n"
            "\t\t\t\t<TextoDuplicado>"+textoDuplicado+"</TextoDuplicado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
}
void Estructurador::setDuplicateLines(wxString nombreAccion,int inicio,int final, wxString textoDuplicado){
    evento.clear();
    wxString ini,fn;
    ini<<inicio;
    fn<<final;
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<LineaDuplicada>"+ini+"-"+fn+"</LineaDuplicada>\n"
            "\t\t\t\t<TextoDuplicado>"+textoDuplicado+"</TextoDuplicado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::setselectAll(wxString nombreAccion, wxString codigoFuente){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<CodigoFuente>"+codigoFuente+"</CodigoFuente>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    archivoXML<<evento<<endl;
    evento.clear();
}
void Estructurador::setFindText(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<textoBuscado>"+textoBuscado+"</textoBuscado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
}
void Estructurador::setFindNext(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<textoBuscado>"+textoBuscado+"</textoBuscado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
}
void Estructurador::setFindPrev(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento<<"\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<textoBuscado>"+textoBuscado+"</textoBuscado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
}
    
void Estructurador::startReg(){
    archivoXML<<"<Registro>"<<"\n\t"<<"<IdArchivo>"<<128379127319782<<"</IdArchivo>"<<endl;
    archivoXML<<"\t<SecuenciaEventos>"<<endl;
}
string Estructurador::getCodigo(){
//        wxMessageBox(this->codigo);
        return this->codigo;
    }
string Estructurador::getLineaError(){
        return this->lineaError;
    }
string Estructurador::getTextoConError(){
        return this->textoConError;
    }
string Estructurador::getDescripcionError(){
        return this->descripcionError;
    }
void Estructurador::cerrarTag(){
    evento.Append("\t\t\t</Procesar>\n"
        "\t\t</EventoRegistrado>\n");
//    archivoXML<<evento<<endl;
    Evento *nodo=new Evento();
    nodo->setEvento(evento);
    lista.insertarEvento(nodo);
    evento.clear();
}
void Estructurador::terminarTags(){
        archivoXML<<"\t</SecuenciaEventos>\n"
                "</Registro>"<<endl;
    Evento *nodo=new Evento();
    wxString evento;
    evento="\t</SecuenciaEventos>\n"
                "</Registro>";
    nodo->setEvento(evento);
    lista.limpiarListaAlFinalizar(nodo);
    }

string Estructurador::ubicacion(){
    string ubic;
    ubic="C:\\archivosXML\\";
    ubic.append(getFechaName());
    ubic.append(".xml");
    return ubic;
}

//Constructor
Estructurador::Estructurador(const char *ubicacion) {
    archivoXML.open(ubicacion,ios::app);
    if (archivoXML.is_open()){
        estructurador=this;
    }
    else{
        wxMessageBox(wxString("No se pudo abrir el archivo de log: ")<<ubicacion);
    }
    startReg();
    lista.iniciarRegistro();
}

Estructurador::Estructurador(const Estructurador& orig) {
}
//Destructor
Estructurador::~Estructurador() {
}
