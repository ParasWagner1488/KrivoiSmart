#include "player.h"
#include <iostream>

using namespace std;

Player::Player() : health(3), maxHealth(3), currentLocation(nullptr), 
                   hasJobOffer(false), hasFoughtInSaloon(false), money(10) {}

void Player::setHealth(int h) { 
    health = h > maxHealth ? maxHealth : h; 
}

void Player::setCurrentLocation(Location* location) { 
    currentLocation = location; 
}

void Player::setHasJob(bool hasJob) { 
    hasJobOffer = hasJob; 
}

void Player::setHasFought(bool fought) { 
    hasFoughtInSaloon = fought; 
}

void Player::setMoney(int m) { 
    money = m; 
}

void Player::takeDamage(int damage) {
    health -= damage;
    if (health < 0) health = 0;
    cout << "Вы получили " << damage << " урона! Здоровье: " << health << "/" << maxHealth << "\n";
}

void Player::heal(int amount) {
    health += amount;
    if (health > maxHealth) health = maxHealth;
    cout << "Вы восстановили " << amount << " здоровья! Здоровье: " << health << "/" << maxHealth << "\n";
}

void Player::addMoney(int amount) {
    money += amount;
    cout << "Вы получили $" << amount << ". Теперь у вас: $" << money << "\n";
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::showStatus() const {
    cout << "\n=== СОСТОЯНИЕ ИГРОКА ===\n";
    cout << "Здоровье: " << health << "/" << maxHealth << "\n";
    cout << "Деньги: $" << money << "\n";
    cout << "Работа у шерифа: " << (hasJobOffer ? "Да" : "Нет") << "\n";
    cout << "=======================\n";
}

void Player::useItem(int index) {
    Item* item = inventory.getItem(index - 1);
    if (item) {
        if (item->isUsable()) {
            cout << "\nВы используете: " << item->getName() << "\n";
            item->use();
        } else {
            cout << "Этот предмет нельзя использовать напрямую.\n";
        }
    } else {
        cout << "Нет предмета с таким номером.\n";
    }
}