#include <iostream>
#include <list>
#include <string>
#include <functional>
#include "Entity.h"
void healCommand(Entity&, const std::list<std::string>&);
void damageCommand(Entity&, const std::list<std::string>&);

int main() {
    Entity player("Hero");

    // Funcion lambda
    auto moveCommand = [&player](const std::list<std::string>& args) {
        if (args.size() != 2) {
            std::cout << "Error: move necesita 2 argumentos\n";
            return;
        }

        auto it = args.begin();
        int x = std::stoi(*it++);
        int y = std::stoi(*it);

        player.move(x, y);
    };

    std::list<std::string> args1 = {"10"};
    healCommand(player, args1);

    std::list<std::string> args2 = {"5"};
    damageCommand(player, args2);

    std::list<std::string> args3 = {"3", "4"};
    moveCommand(args3);

    std::cout << player.status() << std::endl;

    return 0;
}