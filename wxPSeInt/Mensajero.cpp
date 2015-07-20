#include "Mensajero.h"
#include <wx/msgdlg.h>
#include <iostream>
#include <fstream>
#include "Estructurador.h"
#include <stdio.h>


using namespace std;
fstream archivoXML;
void Mensajero::mostrar(string repositorioPath){
    fstream repositorio;
    string line,repositorioXML,file;
    
    repositorioXML=repositorioPath+"RepositorioXML.txt";
    repositorio.open(repositorioXML.c_str());
    if(!repositorio.is_open()){
        cout<<"No se abrio\n";
    }else{
//        cout<<"Archivo abierto\n";
        while ( getline (repositorio,line)){
            if(line.length()<=15){
                cout<<"Hay una linea Vacía\n";
            }else{
                file=repositorioPath+line;
                archivoXML.open(file.c_str());
                if(archivoXML.is_open()){
                    cout<<"ARCHIVO XML ABIERTO \n";
                    
                }else{
                    cout<<"ARCHIVO XML NOOOOOOOOOOOO\n";
                    cout<<line+"\n";
                }
            }
        }
        repositorio.close();
    }
    cout<<line;
}
Mensajero::Mensajero() {
}

Mensajero::Mensajero(const Mensajero& orig) {
}

Mensajero::~Mensajero() {
}

