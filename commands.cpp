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

    int value = std::stoi(args.front());
    entity.heal(value);
}


void damageCommand(Entity& entity, const std::list<std::string>& args) {
    if (args.size() != 1) {
        std::cout << "Error: damage necesita 1 argumento\n";
        return;
    }

    int value = std::stoi(args.front());
    entity.damage(value);
}