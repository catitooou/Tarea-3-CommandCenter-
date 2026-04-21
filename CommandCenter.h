#ifndef COMMANDCENTER_H
#define COMMANDCENTER_H

#include <functional>
#include <map>
#include <list>
#include <string>
#include "Entity.h"

//función que recibe lista de strings y no retorna ningun valor
using Command = std::function<void(const std::list<std::string>&)>;

class CommandCenter {
private:
    Entity& entity;
    std::map<std::string, Command> commands;
    std::list<std::string> history;
    std::map<std::string, std::list<std::pair<std::string, std::list<std::string>>>> macros;

public:
    explicit CommandCenter(Entity& e);

    // Registra un comando bajo un nombre
    void registerCommand(const std::string& name, Command cmd);

    // Ejecuta un comando registrado por nombre
    void execute(const std::string& name, const std::list<std::string>& args);

    // Elimina un comando del mapa dinámicamente

    void removeCommand(const std::string& name);

    // secuencia de pasos
    void registerMacro(
        const std::string& name,
        const std::list<std::pair<std::string, std::list<std::string>>>& steps
    );

    // Ejecuta un macro registrado

    void executeMacro(const std::string& name);

    
    void printHistory() const;
};

#endif