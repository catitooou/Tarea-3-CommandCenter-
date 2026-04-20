#include <iostream>
#include <list>
#include <string>
#include <functional>
#include "Entity.h"
#include "CommandCenter.h"
#include "ResetFunctor.h"

// Declaraciones de funciones libres (definidas en commands.cpp)
void healCommand(Entity&, const std::list<std::string>&);
void damageCommand(Entity&, const std::list<std::string>&);
void statusCommand(Entity&, const std::list<std::string>&);

// -------------------------------------------------------
// Helpers para imprimir separadores
// -------------------------------------------------------
void printSection(const std::string& title) {
    std::cout << "\n══════════════════════════════════════\n";
    std::cout << "  " << title << "\n";
    std::cout << "══════════════════════════════════════\n";
}

// -------------------------------------------------------
int main() {

    Entity player("Hero");
    CommandCenter center(player);

    // ===================================================
    // 1. REGISTRO DE COMANDOS
    // ===================================================
    printSection("REGISTRO DE COMANDOS");

    // -- Funciones libres (wrapped en lambdas para adaptar firma) --
    center.registerCommand("heal", [&player](const std::list<std::string>& args) {
        healCommand(player, args);
    });

    center.registerCommand("damage", [&player](const std::list<std::string>& args) {
        damageCommand(player, args);
    });

    center.registerCommand("status", [&player](const std::list<std::string>& args) {
        statusCommand(player, args);
    });

    // -- Lambda: comando move (captura player por referencia) --
    center.registerCommand("move", [&player](const std::list<std::string>& args) {
        if (args.size() != 2) {
            std::cout << "[move] Error: necesita exactamente 2 argumentos (x, y).\n";
            return;
        }
        try {
            auto it = args.begin();
            int x = std::stoi(*it++);
            int y = std::stoi(*it);
            player.move(x, y);
        } catch (...) {
            std::cout << "[move] Error: argumentos invalidos (deben ser numeros).\n";
        }
    });

    // -- Functor: ResetFunctor con limite de 3 usos --
    ResetFunctor resetFn(player, 3);
    center.registerCommand("reset", std::ref(resetFn));
    // std::ref permite que el estado interno del functor (execCount)
    // se mantenga actualizado en el objeto original resetFn


    // ===================================================
    // 2. DEMO - FUNCIONES LIBRES (mínimo 3 ejemplos)
    // ===================================================
    printSection("DEMO: Funciones Libres");

    std::cout << ">> heal 20\n";
    center.execute("heal", std::list<std::string>{"20"});

    std::cout << ">> damage 35\n";
    center.execute("damage", std::list<std::string>{"35"});

    std::cout << ">> heal 50\n";
    center.execute("heal", std::list<std::string>{"50"});

    std::cout << ">> status\n";
    center.execute("status", std::list<std::string>{});

    // Casos inválidos
    std::cout << "\n-- Casos invalidos --\n";
    std::cout << ">> heal (sin args)\n";
    center.execute("heal", std::list<std::string>{});

    std::cout << ">> damage abc\n";
    center.execute("damage", std::list<std::string>{"abc"});


    // ===================================================
    // 3. DEMO - LAMBDAS: move (mínimo 3 ejemplos)
    // ===================================================
    printSection("DEMO: Lambda - move");

    std::cout << ">> move 5 3\n";
    center.execute("move", std::list<std::string>{"5", "3"});

    std::cout << ">> move 10 -2\n";
    center.execute("move", std::list<std::string>{"10", "-2"});

    std::cout << ">> move -4 8\n";
    center.execute("move", std::list<std::string>{"-4", "8"});

    std::cout << ">> status\n";
    center.execute("status", std::list<std::string>{});

    // Caso inválido
    std::cout << "\n-- Casos invalidos --\n";
    std::cout << ">> move 5 (solo 1 argumento)\n";
    center.execute("move", std::list<std::string>{"5"});

    std::cout << ">> move x y (no numericos)\n";
    center.execute("move", std::list<std::string>{"x", "y"});


    // ===================================================
    // 4. DEMO - FUNCTOR: ResetFunctor (mínimo 3 ejemplos)
    // ===================================================
    printSection("DEMO: Functor - ResetFunctor (limite: 3 usos)");

    // Primero dañamos al jugador y movemos para ver el efecto del reset
    center.execute("damage", std::list<std::string>{"40"});
    center.execute("move", std::list<std::string>{"99", "99"});
    std::cout << ">> status (antes de resets)\n";
    center.execute("status", std::list<std::string>{});

    std::cout << "\n>> reset (uso 1/3)\n";
    center.execute("reset", std::list<std::string>{});
    center.execute("status", std::list<std::string>{});

    center.execute("damage", std::list<std::string>{"20"});
    center.execute("move", std::list<std::string>{"50", "50"});
    std::cout << "\n>> reset (uso 2/3)\n";
    center.execute("reset", std::list<std::string>{});
    center.execute("status", std::list<std::string>{});

    std::cout << "\n>> reset (uso 3/3 - ultimo permitido)\n";
    center.execute("reset", std::list<std::string>{});
    center.execute("status", std::list<std::string>{});

    std::cout << "\n>> reset (uso 4 - BLOQUEADO)\n";
    center.execute("reset", std::list<std::string>{});

    std::cout << "\n>> reset (uso 5 - BLOQUEADO)\n";
    center.execute("reset", std::list<std::string>{});

    std::cout << "\n[Functor] Total de resets ejecutados: "
              << resetFn.getCount() << "/" << resetFn.getMaxUses() << "\n";


    // ===================================================
    // 5. DEMO - ELIMINACIÓN DINÁMICA DE COMANDOS
    // ===================================================
    printSection("DEMO: Eliminacion dinamica de comandos");

    std::cout << ">> Eliminando 'damage'...\n";
    center.removeCommand("damage");

    std::cout << ">> damage 10 (debe fallar - fue eliminado)\n";
    center.execute("damage", std::list<std::string>{"10"});

    std::cout << ">> Intentar eliminar 'noexiste' (comando inexistente)\n";
    center.removeCommand("noexiste");


    // ===================================================
    // 6. DEMO - MACRO COMANDOS (mínimo 3 macros)
    // ===================================================
    printSection("DEMO: Macro Comandos");

    // Macro 1: recuperar_y_ver — cura y muestra estado
    center.registerMacro("recuperar_y_ver", std::list<std::pair<std::string, std::list<std::string>>>{
        {"heal",   std::list<std::string>{"30"}},
        {"status", std::list<std::string>{}}
    });

    // Macro 2: patrulla — secuencia de movimientos en cuadrado
    center.registerMacro("patrulla", std::list<std::pair<std::string, std::list<std::string>>>{
        {"move",   std::list<std::string>{"3",  "0"}},
        {"move",   std::list<std::string>{"0",  "3"}},
        {"move",   std::list<std::string>{"-3", "0"}},
        {"move",   std::list<std::string>{"0", "-3"}},
        {"status", std::list<std::string>{}}
    });

    // Macro 3: preparar_combate — cura máxima y reposiciona
    center.registerMacro("preparar_combate", std::list<std::pair<std::string, std::list<std::string>>>{
        {"heal",   std::list<std::string>{"50"}},
        {"move",   std::list<std::string>{"10", "5"}},
        {"heal",   std::list<std::string>{"25"}},
        {"status", std::list<std::string>{}}
    });

    // Macro 4 (inválido): contiene 'damage' que fue eliminado
    center.registerMacro("macro_roto", std::list<std::pair<std::string, std::list<std::string>>>{
        {"heal",   std::list<std::string>{"10"}},
        {"damage", std::list<std::string>{"5"}},    // <-- comando eliminado
        {"status", std::list<std::string>{}}
    });

    std::cout << "\n-- Ejecutando macro: recuperar_y_ver --\n";
    center.executeMacro("recuperar_y_ver");

    std::cout << "\n-- Ejecutando macro: patrulla --\n";
    center.executeMacro("patrulla");

    std::cout << "\n-- Ejecutando macro: preparar_combate --\n";
    center.executeMacro("preparar_combate");

    std::cout << "\n-- Ejecutando macro: macro_roto (debe detenerse) --\n";
    center.executeMacro("macro_roto");

    std::cout << "\n-- Ejecutando macro inexistente --\n";
    center.executeMacro("fantasma");


    // ===================================================
    // 7. ESTADO FINAL + HISTORIAL
    // ===================================================
    printSection("ESTADO FINAL");
    std::cout << player.status() << "\n";

    center.printHistory();

    return 0;
}