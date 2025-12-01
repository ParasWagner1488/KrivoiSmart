#ifndef PLAYER_H
#define PLAYER_H

#include "inventory.h"

class Location;

class Player {
private:
    int health;
    int maxHealth;
    Inventory inventory;
    Location* currentLocation;
    bool hasJobOffer;
    bool hasFoughtInSaloon;
    int money;

public:
    Player();
    
    // Геттеры
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getMoney() const { return money; }
    Inventory& getInventory() { return inventory; }
    Location* getCurrentLocation() const { return currentLocation; }
    bool hasJob() const { return hasJobOffer; }
    bool hasFought() const { return hasFoughtInSaloon; }
    
    // Сеттеры
    void setHealth(int h);
    void setCurrentLocation(Location* location);
    void setHasJob(bool hasJob);
    void setHasFought(bool fought);
    void setMoney(int m);
    
    // Методы
    void takeDamage(int damage);
    void heal(int amount);
    void addMoney(int amount);
    bool isAlive() const;
    void showStatus() const;
    void useItem(int index);
};

#endif