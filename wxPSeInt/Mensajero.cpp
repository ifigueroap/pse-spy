#include "Mensajero.h"
#include <wx/msgdlg.h>
#include <iostream>
#include <fstream>
#include "Estructurador.h"
#include <stdio.h>
#include <wx/protocol/http.h>
#include <wx/sstream.h>
#include <wx/string.h>
//#include "CkHttp.h"
//#include "CkHttpRequest.h"
//#include "CkHttpResponse.h"

using namespace std;
fstream archivoXML;
void Mensajero::mostrar(string repositorioPath){
    wxString respuestaServidor;
    
    fstream repositorio;
    string nombre,repositorioXML,file,contenido,aux;
    
    repositorioXML=repositorioPath+"RepositorioXML.txt"; //Abrir archivo que contiene nombres de los XML
    repositorio.open(repositorioXML.c_str());
    if(!repositorio.is_open()){                         //No se abrio
        cout<<"No se abrio\n";
    }else{
        while ( getline (repositorio,nombre)){            //While que permite ingresar a cada nombre!
            if(nombre.length()<=15){                      //El nombre no es valido
                cout<<"Hay una linea Vacía\n";
            }else{                                      //Nombre valido
                file=repositorioPath+nombre;
                archivoXML.open(file.c_str());          //Se intenta abrir el archivo
                if(archivoXML.is_open()){               //Si se abrió correctamente
                    contenido.clear();
                    while(getline(archivoXML,aux)){     //Se llena un string con el contenido del archivo
                        contenido.append(aux);
                        contenido.append("\n");
                    }
                    archivoXML.close();
//------------------------------------------------------------ Se envía el nombre y el contenido del archivo.
                respuestaServidor = wxHTTP_POST("192.168.10.72","tesis/vistas/RecibeDatos.php","parameters="+nombre+","+contenido);
                //-------------------------------------------- respuesta son los "echo"
                }else{
                    cout<<"ARCHIVO XML NOOOOOOOOOOOO\n";
                    cout<<nombre+"\n";
                }
            }
        }
        repositorio.close();
    }
    cout<<nombre;
    
}
wxString Mensajero::wxHTTP_POST(const wxString &server, const wxString &page, const wxString& parameters){
   wxString reply = wxEmptyString;
   wxHTTP http;
   http.SetHeader("Content-type", "application/x-www-form-urlencoded");
   http.SetPostBuffer(parameters);
   http.Connect(server);
   if (!http.GetError()) wxMessageBox("SII");
   else wxMessageBox("NOO");
   wxInputStream *http_stream = http.GetInputStream("/"+page);
   if (http.GetError() == wxPROTO_NOERR){
      wxStringOutputStream out_stream(&reply);
      http_stream->Read(out_stream);
   }else{
       reply = wxEmptyString;
   }
   wxDELETE(http_stream);
   http.Close();
   return reply;
   
}

Mensajero::Mensajero() {
}

Mensajero::Mensajero(const Mensajero& orig) {
}

Mensajero::~Mensajero() {
}

