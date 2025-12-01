#ifndef INVENTORY_H
#define INVENTORY_H

#include "item.h"

class Inventory {
private:
    Item** items;
    int capacity;
    int count;

public:
    Inventory(int capacity = 5);
    ~Inventory();
    
    bool addItem(Item* item);
    bool removeItem(int index);
    void showInventory() const;
    
    int getCount() const { return count; }
    int getCapacity() const { return capacity; }
    Item* getItem(int index) const;
    bool hasItem(const string& itemName) const;
};

#endif