#include "Estructurador.h"
#include <wx/String.h>
#include "Recolector.h"
#include <wx/msgdlg.h>
#include "dateTime.h"
#include "ColaEventos.h"
#include <wx/stdpaths.h>
#include "wx/filename.h"
#include "Mensajero.h"
using namespace std;

// wxMessageBox(_ZZ("")); MENSAJE DE PRUEBA #include <wx/msgdlg.h>
Estructurador *estructurador=NULL;
ColaEventos *colaEv;
std::string repositorioPath;
//GESTIONAR ARCHIVO
//-------------------------------------------------------------------------------
void Estructurador::setCodigo(wxString codigo){
    this->codigo="<![CDATA[";
    this->codigo.append( (string)codigo.mb_str());
    this->codigo.append("]]>");
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
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
    evento.clear();
}

//PROCESAR ARCHIVO
//-------------------------------------------------------------------------------

void Estructurador::setRunSource(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>Ejecutar</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n";
}
void Estructurador::setErrorSintaxis(wxString lineaError,wxString descripcionError,wxString textoError){
    if(lineaError=="NO"){
    }else{
        evento.Append("\t\t\t\t\t<ErrorEncontrado>\n"
            "\t\t\t\t\t\t<TipoError>ErrorSintaxis</TipoError>\n"
            "\t\t\t\t\t\t<LineaErrada>"+lineaError+"</LineaErrada>\n"
            "\t\t\t\t\t\t<InstruccionErrada><![CDATA["+textoError+"]]> </InstruccionErrada>\n"
            "\t\t\t\t\t\t<DescripcionError><![CDATA["+descripcionError+"]]></DescripcionError>\n"
            "\t\t\t\t\t</ErrorEncontrado>");
    }
}
void Estructurador::setNumErrorSintaxis(int numError){
    wxString numero;
    numero<<numError;
    evento.Append("\n\t\t\t\t<NumErrores>"+numero+"</NumErrores>\n"
            "\t\t\t</Procesar>\n"
            "\t\t</EventoRegistrado>\n");
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setErrorEjecucion(wxString descripcionError){
    evento.Append(descripcionError);
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setErrorEjecucion(){
    evento.clear();
    evento.Append("\t\t\t</Procesar>\n"
            "\t\t</EventoRegistrado>\n");
    colaEv->insertar(evento);
}
void Estructurador::setVeriSyntax(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>VerificarSintaxis</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n";
}
void Estructurador::setRunStepStep(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>EjecutarPasoPaso</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n";
}
void Estructurador::setCreateDraw(wxString nombreArchivo){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<Procesar>\n"
            "\t\t\t\t<NombreAccion>DibujarDiagrama</NombreAccion>\n"
            "\t\t\t\t<NombreArchivo>"+nombreArchivo+"</NombreArchivo>\n"
            "\t\t\t\t<CodigoArchivo>\n"+getCodigo()+"\t\t\t\t</CodigoArchivo>\n";
}

//ACCIONES DEL EDITOR
//-------------------------------------------------------------------------------
void Estructurador::setCutCode(wxString textoCortado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Cortar</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoCortado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setCopyCode(wxString textoCopiado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Copiar</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoCopiado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setPasteCode(wxString textoPegado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Pegar</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoPegado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setPasteCode(wxString textoBorrado,wxString textoPegado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>Pegar</NombreAccion>\n"
            "\t\t\t\t<TextoEliminado><![CDATA["+textoBorrado+"]]></TextoEliminado>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoPegado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
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
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"</LineasAsociadas>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"-"+fn+"</LineasAsociadas>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"</LineasAsociadas>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"-"+fn+"</LineasAsociadas>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"</LineasAsociadas>\n"
            "\t\t\t\t<TextoEliminado><![CDATA["+textoEliminado+"]]></TextoEliminado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"-"+fn+"</LineasAsociadas>\n"
            "\t\t\t\t<TextoEliminado><![CDATA["+textoEliminado+"]]></TextoEliminado>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"</LineasAsociadas>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoDuplicado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
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
            "\t\t\t\t<LineasAsociadas>"+ini+"-"+fn+"</LineasAsociadas>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoDuplicado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setselectAll(wxString nombreAccion, wxString codigoFuente){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+codigoFuente+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
    evento.clear();
}
void Estructurador::setFindText(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoBuscado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
}
void Estructurador::setFindNext(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento="\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoBuscado+"]]></TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
}
void Estructurador::setFindPrev(wxString nombreAccion, wxString textoBuscado){
    evento.clear();
    evento<<"\t\t<EventoRegistrado>\n"
            "\t\t\t<FechaHora>"+getFecha()+"</FechaHora>\n"
            "\t\t\t<EdicionEditor>\n"
            "\t\t\t\t<NombreAccion>"+nombreAccion+"</NombreAccion>\n"
            "\t\t\t\t<TextoRelevante><![CDATA["+textoBuscado+"</TextoRelevante>\n"
            "\t\t\t</EdicionEditor>\n"
            "\t\t</EventoRegistrado>\n";
    colaEv->insertar(evento);
}
string Estructurador::getCodigo(){
        return this->codigo;
}
string Estructurador::getRepositorioPath(){
    std::cout<<"\n"+repositorioPath+"AAA";
    return repositorioPath;
}
void Estructurador::terminarTags(){
    wxString evento;
    evento="</SecuenciaEventos>\n";
    colaEv->insertarfin(evento);
    //Se crea la clase mensajero para revisar si existen archivos por enviar al inicio del programa
    Mensajero msj;
    msj.recogerDatosLocal(repositorioPath);
}


//Constructor
Estructurador::Estructurador() {
    wxFileName path;
    wxString userDataDir = wxStandardPaths::Get().GetUserConfigDir();
    std::cout<<userDataDir<<std::endl;
    path.AssignDir(userDataDir);
    path.AppendDir("PseSpy");
    if (!path.DirExists()){
        path.Mkdir();
        std::cout<<"Se crea"<<std::endl;
    } else {
        std::cout<<"Ya existe"<<std::endl;
    }
    string nombreArchivo;
    nombreArchivo=getFechaName();
    repositorioPath=path.GetFullPath();
    path.SetName("PseSpy-"+nombreArchivo+".xml");
    colaEv = new ColaEventos(path.GetFullPath().mb_str());
    path.SetName("RepositorioXML.txt");
    colaEv->iniciarTagXml();
    colaEv->setRepositorioXML(path.GetFullPath().mb_str(),nombreArchivo);    
    
}
Estructurador::Estructurador(const Estructurador& orig) {
}
//Destructor
Estructurador::~Estructurador() {
}
