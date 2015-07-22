#include "Mensajero.h"
#include <wx/msgdlg.h>
#include <iostream>
#include <fstream>
#include "Estructurador.h"
#include <stdio.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>
#include <wx/string.h>

using namespace std;
void Mensajero::recogerDatosLocal(string repositorioPath){
    wxString respuestaServidor;
    fstream repositorio, archivoXML;
    //repositorio es el archivo .txt que tiene los nombreArchivoXMLs de los archivos XML.
    //archivoXML es el archivo que se utiliza para escribir y guardar los XML.
    string nombreArchivoXML,repositorioXML,file,contenidoArchivoXML,aux;
    //nombreArchivoXML es el nombreArchivoXML del archivo XML
    //repositorioXML es el path completo con el nombreArchivoXML del repositorio.
    //file es el path completo del XML en el que se escribe para guardar
    //contenidoArchivoXML, es el contenidoArchivoXML del archivo XML
    
    repositorioXML=repositorioPath+"RepositorioXML.txt"; //Abrir archivo que contiene nombreArchivoXMLs de los XML
    repositorio.open(repositorioXML.c_str());
    if(!repositorio.is_open()){                         //No se abrio
        cout<<"No se abrio\n";
    }else{
        while ( getline (repositorio,nombreArchivoXML)){            //While que permite ingresar a cada nombreArchivoXML!
            if(nombreArchivoXML.length()<=15){                      //El nombreArchivoXML no es valido
                cout<<"Hay una linea Vacía\n";
            }else{                                      //Nombre valido
                file=repositorioPath+nombreArchivoXML;
                archivoXML.open(file.c_str());          //Se intenta abrir el archivo
                if(archivoXML.is_open()){               //Si se abrió correctamente
                    contenidoArchivoXML.clear();
                    while(getline(archivoXML,aux)){     //Se llena un string con el contenidoArchivoXML del archivo
                        contenidoArchivoXML.append(aux);
                        contenidoArchivoXML.append("\n");
                    }
                    archivoXML.close();
                    repositorio.close();
//------------------------------------------------------------ Se envía el nombreArchivoXML y el contenidoArchivoXML del archivo.
                    respuestaServidor = Enviar("192.168.10.7","tesis/vistas/RecibeDatos.php","parameters="+nombreArchivoXML+","+contenidoArchivoXML);
//------------------------------------------------------------ respuesta son los "echo"
                    remove(file.c_str());
                    remove(repositorioXML.c_str());
                }else{
                    //NO SE PUDO ABRIR EL ARCHIVO, el repositorio.txt apuntaba a un archivo no existente
                }
            }
        }
    }
    cout<<nombreArchivoXML;
}
wxString Mensajero::Enviar(const wxString &ipServidor, const wxString &page, const wxString& parameters){
   wxString respuesta = wxEmptyString;
   wxHTTP http;
   http.SetHeader("Content-type", "application/x-www-form-urlencoded");
   http.SetPostBuffer(parameters);
   http.Connect(ipServidor,80);
   wxInputStream *http_stream = http.GetInputStream("/"+page);
   if (http.GetError() == wxPROTO_NOERR){
      wxStringOutputStream out_stream(&respuesta);
      http_stream->Read(out_stream);
   }else{
       respuesta = wxEmptyString;
   }
   wxDELETE(http_stream);
   http.Close();
   return respuesta;
}

Mensajero::Mensajero() {
}

Mensajero::Mensajero(const Mensajero& orig) {
}

Mensajero::~Mensajero() {
}

