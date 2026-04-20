#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>

class Entity {
private:
    int x,y;
    int health;
    std::string name;

public:
    Entity(std::string name) : x(0), y(0), health(100), name(name) {}

    void move(int dx, int dy) {
        x += dx;
        y += dy;
    }
    void heal(int amount) {
        health += amount;
    }
    void damage(int amount) {
        health -= amount;
    }
    void reset() {
        x = 0;
        y = 0;
        health = 100;
    }

    std::string status() const {
        return "Entity: " + name +
               " Pos(" + std::to_string(x) + "," + std::to_string(y) + ")" +
               " Health: " + std::to_string(health);
    }
};

#endif