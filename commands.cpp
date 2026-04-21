#include "Entity.h"
#include <list>
#include <string>
#include <iostream>

// Funcion Libre
void healCommand(Entity& entity, const std::list<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Error: heal necesita 1 argumento\n";
        return;
    }

    try {
        int value = std::stoi(args.front());
        if (value < 0) {
            std::cout << "Error! el valor debe ser positivo \n";
            return;
        }
        entity.heal(value);
    } catch (...) {
        std::cout << "[heal] Error: argumento invalido (no es un numero )\n";
    }
}


void damageCommand(Entity& entity, const std::list<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Error:damage necesita 1 argumento\n";
        return;
    }

    try {
        int value = std::stoi(args.front());
        if (value < 0) {
            std::cout << "[damage] Error:el valor debe ser  positivo \n";
            return;
        }
        entity.damage(value);
    } catch (...) {
        std::cout << "[damage] Error!  argumento invalido no es un numero  \n";
    }

}
void statusCommand(Entity& entity, const std::list<std::string>& args) {
    // No necesita argumentos imprime el estado actual de la entidad
    std::cout << "[status] " << entity.status() << "\n";
}
 