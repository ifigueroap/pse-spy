#ifndef MENSAJERO_H
#define	MENSAJERO_H
#include <iostream>
class Mensajero {
public:
    void mostrar(std::string repositorioPath);
    Mensajero();
    Mensajero(const Mensajero& orig);
    virtual ~Mensajero();
private:

};

#endif	/* MENSAJERO_H */

