#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "location.h"

class Game {
private:
    Player player;
    Location* currentLocation;
    TownSquare townSquare;
    Saloon saloon;
    SheriffOffice sheriffOffice;
    bool isRunning;
    
    void initialize();
    void processCommand(const string& command);
    void checkEndConditions();
    void showEnding(int ending);
    
public:
    Game();
    void run();
};

#endif