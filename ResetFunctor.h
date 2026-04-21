#ifndef RESETFUNCTOR_H
#define RESETFUNCTOR_H

#include <list>
#include <string>
#include <iostream>
#include "Entity.h"

/*
 ResetFunctor: Functor que ejecuta reset sobre la entidad mantiene estado interno: contador de usos y límite máximo.
 Al superar el límite el comando es bloqueado automáticamente
 */
class ResetFunctor {
private:
    Entity& entidad;    // Referencia 
    int contadorDeEjecuciones;     // Contador interno 
    int usosMaximos;       

public:
    ResetFunctor(Entity& e, int usosMaximos)
        : entidad(e), contadorDeEjecuciones(0), usosMaximos(usosMaximos) {}

    // operator() hace que la clase sea invocable como función
    void operator()(const std::list<std::string>& args) {
        if (contadorDeEjecuciones >= usosMaximos) {
            std::cout << "[ResetFunctor] Limite alcanzado ("
                      << usosMaximos << " usos). Reset BLOQUEADO.\n";
            return;
        }
        entidad.reset();
        contadorDeEjecuciones++;
        std::cout << "[ResetFunctor] Reset ejecutado. Uso "
                  << contadorDeEjecuciones << "/" << usosMaximos << ".\n";
    }

    int getCount() const { return contadorDeEjecuciones; }
    int getMaxUses() const { return usosMaximos; }
};

#endif