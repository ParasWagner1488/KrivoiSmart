#include "location.h"
#include "item.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

// ==================== ГОРОДСКАЯ ПЛОЩАДЬ ====================
Location::Location(const string& name, const string& description) 
    : name(name), description(description) {}

TownSquare::TownSquare() : Location("Главная площадь", "Пыльная площадь городка Грэйсбери"), revolverFound(false) {}

void TownSquare::enter(Player* player) {
    cout << "\n=== ГЛАВНАЯ ПЛОЩАДЬ ===\n";
    cout << description << "\n";
    cout << "Перед вами колодец, почтовое отделение и несколько ковбоев, лениво курящих у стойки.\n";
    cout << "На западе виднеется салун 'Последний шанс', на востоке - офис шерифа.\n";
}

// ... (остальная реализация локаций) ...