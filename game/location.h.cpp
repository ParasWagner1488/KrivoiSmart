#ifndef LOCATION_H
#define LOCATION_H

#include <string>
#include "player.h"

using namespace std;

// Базовый класс локации
class Location {
protected:
    string name;
    string description;
    
public:
    Location(const string& name, const string& description);
    virtual ~Location() = default;
    
    virtual void enter(Player* player) = 0;
    virtual void explore(Player* player) = 0;
    virtual void handleInput(Player* player, const string& input) = 0;
    virtual string getMenu() = 0;
    
    string getName() const { return name; }
    string getDescription() const { return description; }
};

// Конкретные локации
class TownSquare : public Location {
private:
    bool revolverFound;
    
public:
    TownSquare();
    void enter(Player* player) override;
    void explore(Player* player) override;
    void handleInput(Player* player, const string& input) override;
    string getMenu() override;
    
private:
    void inspectWell(Player* player);
    void talkToStranger(Player* player);
};

class Saloon : public Location {
private:
    bool whiskeyDrunk;
    bool pokerPlayed;
    bool fightHappened;
    
public:
    Saloon();
    void enter(Player* player) override;
    void explore(Player* player) override;
    void handleInput(Player* player, const string& input) override;
    string getMenu() override;
    
private:
    void drinkWhiskey(Player* player);
    void playPoker(Player* player);
    void startFight(Player* player);
};

class SheriffOffice : public Location {
private:
    bool jobAccepted;
    bool rewardTaken;
    
public:
    SheriffOffice();
    void enter(Player* player) override;
    void explore(Player* player) override;
    void handleInput(Player* player, const string& input) override;
    string getMenu() override;
    
private:
    void offerJob(Player* player);
    void takeReward(Player* player);
    void stealReward(Player* player);
};

#endif