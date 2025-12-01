#include "inventory.h"

Inventory::Inventory(int capacity) : capacity(capacity), count(0) {
    items = new Item*[capacity];
    for (int i = 0; i < capacity; i++) {
        items[i] = nullptr;
    }
}

Inventory::~Inventory() {
    for (int i = 0; i < count; i++) {
        delete items[i];
    }
    delete[] items;
}

bool Inventory::addItem(Item* item) {
    if (count >= capacity) {
        cout << "Инвентарь полон! Нельзя взять предмет: " << item->getName() << "\n";
        return false;
    }
    items[count++] = item;
    cout << "Добавлен предмет: " << item->getName() << "\n";
    return true;
}

bool Inventory::removeItem(int index) {
    if (index < 0 || index >= count) {
        return false;
    }
    
    cout << "Удален предмет: " << items[index]->getName() << "\n";
    delete items[index];
    
    for (int i = index; i < count - 1; i++) {
        items[i] = items[i + 1];
    }
    
    items[count - 1] = nullptr;
    count--;
    return true;
}

void Inventory::showInventory() const {
    if (count == 0) {
        cout << "Инвентарь пуст.\n";
        return;
    }
    
    cout << "\n=== ИНВЕНТАРЬ (" << count << "/" << capacity << ") ===\n";
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << items[i]->getName() 
             << " - " << items[i]->getDescription() << "\n";
    }
    cout << "========================\n";
}

Item* Inventory::getItem(int index) const {
    if (index < 0 || index >= count) return nullptr;
    return items[index];
}

bool Inventory::hasItem(const string& itemName) const {
    for (int i = 0; i < count; i++) {
        if (items[i]->getName() == itemName) {
            return true;
        }
    }
    return false;
}