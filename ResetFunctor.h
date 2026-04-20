#ifndef RESETFUNCTOR_H
#define RESETFUNCTOR_H

#include <list>
#include <string>
#include <iostream>
#include "Entity.h"

/*
 * ResetFunctor: Functor que ejecuta reset sobre la entidad.
 * Mantiene estado interno: contador de usos y límite máximo.
 * Al superar el límite, el comando es bloqueado automáticamente.
 */
class ResetFunctor {
private:
    Entity& entity;    // Referencia a la entidad que controla
    int execCount;     // Contador interno de ejecuciones
    int maxUses;       // Limite máximo de usos permitidos

public:
    ResetFunctor(Entity& e, int maxUses)
        : entity(e), execCount(0), maxUses(maxUses) {}

    // operator() hace que la clase sea invocable como función
    void operator()(const std::list<std::string>& args) {
        if (execCount >= maxUses) {
            std::cout << "[ResetFunctor] Limite alcanzado ("
                      << maxUses << " usos). Reset BLOQUEADO.\n";
            return;
        }
        entity.reset();
        execCount++;
        std::cout << "[ResetFunctor] Reset ejecutado. Uso "
                  << execCount << "/" << maxUses << ".\n";
    }

    int getCount() const { return execCount; }
    int getMaxUses() const { return maxUses; }
};

#endif