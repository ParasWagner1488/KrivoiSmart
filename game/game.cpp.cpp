#include "game.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

using namespace std;

Game::Game() : currentLocation(nullptr), isRunning(true) {
    srand(static_cast<unsigned int>(time(0)));
    initialize();
}

void Game::initialize() {
    cout << "========================================\n";
    cout << "       КОВБОЙСКИЙ КВЕСТ\n";
    cout << "========================================\n\n";
    
    cout << "Вы - безымянный ковбой, прибывший в пыльный городок Грэйсбери.\n";
    cout << "Ваша лошадь пала в дороге, денег почти не осталось.\n";
    cout << "Вам предстоит выжить в этом жестоком мире и найти свой путь...\n\n";
    
    currentLocation = &townSquare;
    player.setCurrentLocation(currentLocation);
    currentLocation->enter(&player);
}

// ... (реализация остальных методов) ...