#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>

using namespace std;

// ==================== БАЗОВЫЙ КЛАСС ПРЕДМЕТА ====================
class Item {
protected:
    string name;
    string description;
    bool usable;

public:
    Item(const string& name, const string& description, bool usable = true);
    virtual ~Item() = default;
    
    virtual void use() = 0;
    
    string getName() const { return name; }
    string getDescription() const { return description; }
    bool isUsable() const { return usable; }
};

// ==================== КЛАССЫ ПРЕДМЕТОВ ====================
class Revolver : public Item {
public:
    Revolver();
    void use() override;
};

class Whiskey : public Item {
public:
    Whiskey();
    void use() override;
};

class LuckyCoin : public Item {
public:
    LuckyCoin();
    void use() override;
};

class WantedPoster : public Item {
public:
    WantedPoster();
    void use() override;
};

#endif