#include "CommandCenter.h"
#include <iostream>

CommandCenter::CommandCenter(Entity& e) : entity(e) {}

void CommandCenter::registerCommand(const std::string& name, Command cmd) {
    commands[name] = cmd;
    std::cout << "[CommandCenter] Comando registrado: '" << name << "'\n";
}

void CommandCenter::execute(const std::string& name, const std::list<std::string>& args) {
    // Uso explícito de std::map iterator
    std::map<std::string, Command>::iterator it = commands.find(name);

    if (it == commands.end()) {
        std::cout << "[Error] Comando '" << name << "' no encontrado.\n";
        history.push_back("[ERROR] Comando no encontrado: " + name);
        return;
    }

    // Captura estado antes de ejecutar
    std::string before = entity.status();

    // Ejecuta el comando
    it->second(args);

    // Captura estado después de ejecutar
    std::string after = entity.status();

    // Registra en historial
    std::string entry = "[" + name + "] | Antes: { " + before + " } | Despues: { " + after + " }";
    history.push_back(entry);
}

void CommandCenter::removeCommand(const std::string& name) {
    // Uso explícito de std::map iterator
    std::map<std::string, Command>::iterator it = commands.find(name);

    if (it == commands.end()) {
        std::cout << "[CommandCenter] No se puede eliminar: '" << name << "' no existe.\n";
        return;
    }

    commands.erase(it);
    std::cout << "[CommandCenter] Comando '" << name << "' eliminado exitosamente.\n";
}

void CommandCenter::registerMacro(
    const std::string& name,
    const std::list<std::pair<std::string, std::list<std::string>>>& steps
) {
    macros[name] = steps;
    std::cout << "[CommandCenter] Macro '" << name << "' registrado con "
              << steps.size() << " paso(s).\n";
}

void CommandCenter::executeMacro(const std::string& name) {
    auto macroIt = macros.find(name);
    if (macroIt == macros.end()) {
        std::cout << "[Error] Macro '" << name << "' no encontrado.\n";
        return;
    }

    std::cout << "[Macro] Ejecutando '" << name << "'...\n";

    const std::list<std::pair<std::string, std::list<std::string>>>& steps = macroIt->second;

    // Uso explícito de std::list iterator para recorrer pasos
    std::list<std::pair<std::string, std::list<std::string>>>::const_iterator stepIt = steps.begin();

    while (stepIt != steps.end()) {
        // Verifica que el comando del paso exista antes de ejecutar
        std::map<std::string, Command>::iterator cmdIt = commands.find(stepIt->first);
        if (cmdIt == commands.end()) {
            std::cout << "[Macro ERROR] Detenido en paso '" << stepIt->first
                      << "': comando no existe.\n";
            return;
        }
        execute(stepIt->first, stepIt->second);
        ++stepIt;
    }

    std::cout << "[Macro] '" << name << "' completado.\n";
}

void CommandCenter::printHistory() const {
    std::cout << "\n========== HISTORIAL DE EJECUCION ==========\n";

    if (history.empty()) {
        std::cout << "(sin registros)\n";
    } else {
        // Uso explícito de std::list iterator
        std::list<std::string>::const_iterator it = history.begin();
        int i = 1;
        while (it != history.end()) {
            std::cout << i++ << ". " << *it << "\n";
            ++it;
        }
    }

    std::cout << "============================================\n";
}