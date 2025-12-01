#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

// ==================== КЛАСС ПРЕДМЕТА ====================
class Item {
protected:
    string name;
    string description;
    bool usable;

public:
    Item(const string& name, const string& description, bool usable = true)
        : name(name), description(description), usable(usable) {}
    
    virtual ~Item() = default;
    
    virtual void use() = 0;
    
    string getName() const { return name; }
    string getDescription() const { return description; }
    bool isUsable() const { return usable; }
};

class Revolver : public Item {
public:
    Revolver() : Item("Старый револьвер", "Ковбойский револьвер с 6 патронами. Изношен, но стреляет.", true) {}
    
    void use() override {
        cout << "Вы достаете револьвер и проверяете барабан. 6 патронов на месте.\n";
        cout << "Звук щелчка курка эхом разносится по округе.\n";
    }
};

class Whiskey : public Item {
public:
    Whiskey() : Item("Виски 'Дикая роза'", "Крепкий алкоголь из местного салуна. Пахнет дымом и дубом.", true) {}
    
    void use() override {
        cout << "Вы отпиваете большой глоток виски. Огонь разливается по жилам!\n";
        cout << "Голова немного кружится, но вы чувствуете прилив смелости.\n";
    }
};

class LuckyCoin : public Item {
public:
    LuckyCoin() : Item("Счастливая монета", "Старая серебряная монета с изображением орла. Приносит удачу в азартных играх.", true) {}
    
    void use() override {
        cout << "Вы подбрасываете монетку. Она падает орлом вверх!\n";
        cout << "Сегодня явно ваш день удачи.\n";
    }
};

class WantedPoster : public Item {
public:
    WantedPoster() : Item("Объявление о розыске", "Банда 'Призраки прерий'. Награда: 500$.", false) {}
    
    void use() override {
        cout << "Вы изучаете объявление:\n";
        cout << "'РАЗЫСКИВАЕТСЯ: банда 'Призраки прерий'.\n";
        cout << "ПРЕСТУПЛЕНИЯ: грабежи, контрабанда, убийства.\n";
        cout << "НАГРАДА: 500$ за поимку лидера.'\n";
    }
};

// ==================== КЛАСС ИНВЕНТАРЯ ====================
class Inventory {
private:
    Item** items;
    int capacity;
    int count;

public:
    Inventory(int capacity = 5) : capacity(capacity), count(0) {
        items = new Item*[capacity];
        for (int i = 0; i < capacity; i++) {
            items[i] = nullptr;
        }
    }
    
    ~Inventory() {
        for (int i = 0; i < count; i++) {
            delete items[i];
        }
        delete[] items;
    }
    
    bool addItem(Item* item) {
        if (count >= capacity) {
            cout << "Инвентарь полон! Нельзя взять предмет: " << item->getName() << "\n";
            return false;
        }
        items[count++] = item;
        cout << "Добавлен предмет: " << item->getName() << "\n";
        return true;
    }
    
    bool removeItem(int index) {
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
    
    void showInventory() const {
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
    
    int getCount() const { return count; }
    int getCapacity() const { return capacity; }
    
    Item* getItem(int index) const {
        if (index < 0 || index >= count) return nullptr;
        return items[index];
    }
    
    bool hasItem(const string& itemName) const {
        for (int i = 0; i < count; i++) {
            if (items[i]->getName() == itemName) {
                return true;
            }
        }
        return false;
    }
};

// ==================== КЛАСС ИГРОКА ====================
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
    Player() : health(3), maxHealth(3), currentLocation(nullptr), 
               hasJobOffer(false), hasFoughtInSaloon(false), money(10) {}
    
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getMoney() const { return money; }
    Inventory& getInventory() { return inventory; }
    Location* getCurrentLocation() const { return currentLocation; }
    bool hasJob() const { return hasJobOffer; }
    bool hasFought() const { return hasFoughtInSaloon; }
    
    void setHealth(int h) { 
        health = h > maxHealth ? maxHealth : h; 
    }
    
    void setCurrentLocation(Location* location) { 
        currentLocation = location; 
    }
    
    void setHasJob(bool hasJob) { 
        hasJobOffer = hasJob; 
    }
    
    void setHasFought(bool fought) { 
        hasFoughtInSaloon = fought; 
    }
    
    void setMoney(int m) { 
        money = m; 
    }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
        cout << "Вы получили " << damage << " урона! Здоровье: " << health << "/" << maxHealth << "\n";
    }
    
    void heal(int amount) {
        health += amount;
        if (health > maxHealth) health = maxHealth;
        cout << "Вы восстановили " << amount << " здоровья! Здоровье: " << health << "/" << maxHealth << "\n";
    }
    
    void addMoney(int amount) {
        money += amount;
        cout << "Вы получили $" << amount << ". Теперь у вас: $" << money << "\n";
    }
    
    bool isAlive() const {
        return health > 0;
    }
    
    void showStatus() const {
        cout << "\n=== СОСТОЯНИЕ ИГРОКА ===\n";
        cout << "Здоровье: " << health << "/" << maxHealth << "\n";
        cout << "Деньги: $" << money << "\n";
        cout << "Работа у шерифа: " << (hasJobOffer ? "Да" : "Нет") << "\n";
        cout << "=======================\n";
    }
    
    void useItem(int index) {
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
};

// ==================== КЛАСС ЛОКАЦИИ ====================
class Location {
protected:
    string name;
    string description;
    
public:
    Location(const string& name, const string& description) 
        : name(name), description(description) {}
    
    virtual ~Location() = default;
    
    virtual void enter(Player* player) = 0;
    virtual void explore(Player* player) = 0;
    virtual void handleInput(Player* player, const string& input) = 0;
    virtual string getMenu() = 0;
    
    string getName() const { return name; }
    string getDescription() const { return description; }
};

// ==================== ГОРОДСКАЯ ПЛОЩАДЬ ====================
class TownSquare : public Location {
private:
    bool revolverFound;
    
public:
    TownSquare() : Location("Главная площадь", "Пыльная площадь городка Грэйсбери"), revolverFound(false) {}
    
    void enter(Player* player) override {
        cout << "\n=== ГЛАВНАЯ ПЛОЩАДЬ ===\n";
        cout << description << "\n";
        cout << "Перед вами колодец, почтовое отделение и несколько ковбоев, лениво курящих у стойки.\n";
        cout << "На западе виднеется салун 'Последний шанс', на востоке - офис шерифа.\n";
    }
    
    void explore(Player* player) override {
        cout << "\nВы осматриваете площадь:\n";
        cout << "1. Колодец выглядит старым и заброшенным.\n";
        cout << "2. На столбе висит потрепанное объявление.\n";
        cout << "3. Один из ковбоев смотрит на вас с интересом.\n";
    }
    
    void handleInput(Player* player, const string& input) override {
        if (input == "1") {
            inspectWell(player);
        } else if (input == "2") {
            cout << "\nВы подходите к объявлению:\n";
            cout << "'РАЗЫСКИВАЕТСЯ: банда Призраки прерий. НАГРАДА: 500$'\n";
            cout << "Внизу подпись: Шериф Джонсон.\n";
        } else if (input == "3") {
            talkToStranger(player);
        } else if (input == "4") {
            cout << "\nВы направляетесь в салун...\n";
        } else if (input == "5") {
            cout << "\nВы направляетесь к шерифу...\n";
        } else if (input == "6") {
            player->showStatus();
        } else if (input == "7") {
            player->getInventory().showInventory();
        } else if (input == "инвентарь") {
            player->getInventory().showInventory();
        } else if (input == "статус") {
            player->showStatus();
        } else {
            cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }
    
    string getMenu() override {
        return "\nДоступные действия:\n"
               "1. Осмотреть колодец\n"
               "2. Прочитать объявление\n"
               "3. Поговорить с незнакомцем\n"
               "4. Пойти в салун\n"
               "5. Пойти к шерифу\n"
               "6. Показать статус\n"
               "7. Показать инвентарь\n"
               "> ";
    }
    
private:
    void inspectWell(Player* player) {
        if (!revolverFound) {
            cout << "\nВы заглядываете в колодец. На глубине что-то блеснуло...\n";
            cout << "Достаете старый револьвер! Он выглядит изношенным, но рабочим.\n";
            player->getInventory().addItem(new Revolver());
            revolverFound = true;
        } else {
            cout << "\nКолодец пуст. Только паутина и сырость.\n";
        }
    }
    
    void talkToStranger(Player* player) {
        cout << "\nНезнакомец: 'Приветствую, странник. Ищешь работу?\n";
        cout << "Шериф набирает людей. Говорят, банда 'Призраков' опять орудует.\n";
        cout << "А в салуне осторожней - там сегодня драка назревает.'\n";
    }
};

// ==================== САЛУН ====================
class Saloon : public Location {
private:
    bool whiskeyDrunk;
    bool pokerPlayed;
    bool fightHappened;
    
public:
    Saloon() : Location("Салун 'Последний шанс'", "Шумное место, полное искателей приключений"), 
               whiskeyDrunk(false), pokerPlayed(false), fightHappened(false) {}
    
    void enter(Player* player) override {
        cout << "\n=== САЛУН 'ПОСЛЕДНИЙ ШАНС' ===\n";
        cout << description << "\n";
        cout << "В углу на расстроенном пианино кто-то играет грустную мелодию.\n";
        cout << "Бармен вытирает стаканы, а за столиками идет оживленная игра в покер.\n";
        cout << "У стойки крупный ковбой явно ищет повод для драки.\n";
    }
    
    void explore(Player* player) override {
        cout << "\nВы осматриваете салун:\n";
        cout << "1. Барная стойка с разнообразным алкоголем\n";
        cout << "2. Покерный стол, за которым идет игра\n";
        cout << "3. Агрессивно настроенный ковбой у стойки\n";
        cout << "4. Выход на главную площадь\n";
    }
    
    void handleInput(Player* player, const string& input) override {
        if (input == "1") {
            drinkWhiskey(player);
        } else if (input == "2") {
            playPoker(player);
        } else if (input == "3") {
            startFight(player);
        } else if (input == "4") {
            cout << "\nВы выходите из салуна на главную площадь...\n";
        } else if (input == "5") {
            player->showStatus();
        } else if (input == "6") {
            player->getInventory().showInventory();
        } else if (input == "инвентарь") {
            player->getInventory().showInventory();
        } else if (input == "статус") {
            player->showStatus();
        } else {
            cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }
    
    string getMenu() override {
        return "\nДоступные действия:\n"
               "1. Выпить виски (5$)\n"
               "2. Играть в покер (ставка 10$)\n"
               "3. Разобраться с грубияном\n"
               "4. Выйти на площадь\n"
               "5. Показать статус\n"
               "6. Показать инвентарь\n"
               "> ";
    }
    
private:
    void drinkWhiskey(Player* player) {
        if (whiskeyDrunk) {
            cout << "\nБармен: 'Думаю, на сегодня хватит, приятель. Не хочу чтобы вас выносили.'\n";
            return;
        }
        
        if (player->getMoney() < 5) {
            cout << "\nУ вас недостаточно денег для виски. Нужно 5$.\n";
            return;
        }
        
        cout << "\nВы заказываете виски 'Дикая роза'.\n";
        player->addMoney(-5);
        player->takeDamage(1);
        whiskeyDrunk = true;
        
        cout << "\nБармен наклоняется и шепчет: 'Между нами, шериф ищет предателя в своем отряде.\n";
        cout << "Будь осторожен, доверяя ему. А еще... банда прячется в старом амбаре к северу от города.'\n";
        
        player->getInventory().addItem(new Whiskey());
    }
    
    void playPoker(Player* player) {
        if (pokerPlayed) {
            cout << "\nИгроки: 'Ты уже проиграл достаточно на сегодня, дружок.'\n";
            return;
        }
        
        if (player->getMoney() < 10) {
            cout << "\nУ вас недостаточно денег для игры. Минимальная ставка 10$.\n";
            return;
        }
        
        cout << "\nВы садитесь за покерный стол. Ставка 10$.\n";
        player->addMoney(-10);
        
        int luck = rand() % 100;
        
        if (player->getInventory().hasItem("Счастливая монета")) {
            luck += 30;
            cout << "Ваша счастливая монета приносит удачу!\n";
        }
        
        if (luck > 50) {
            cout << "Вы выигрываете 50$!\n";
            player->addMoney(50);
            
            if (!player->getInventory().hasItem("Счастливая монета")) {
                cout << "Один из игроков дает вам счастливую монету 'на удачу'.\n";
                player->getInventory().addItem(new LuckyCoin());
            }
        } else if (luck > 20) {
            cout << "Вы теряете ставку. Удача не на вашей стороне.\n";
        } else {
            cout << "Вы проигрываете все! Вас обвиняют в шулерстве и избивают.\n";
            player->takeDamage(2);
        }
        
        pokerPlayed = true;
    }
    
    void startFight(Player* player) {
        if (fightHappened) {
            cout << "\nКовбой лежит без сознания в углу. Лучше его не трогать.\n";
            return;
        }
        
        cout << "\nКовбой: 'Эй, ты! Ты что, на меня косо посмотрел?'\n";
        cout << "1. Извиниться и отойти (потерять 1$)\n";
        cout << "2. Пригрозить револьвером (если есть)\n";
        cout << "3. Начать драку\n";
        
        string choice;
        getline(cin, choice);
        
        if (choice == "1") {
            cout << "\nВы извиняетесь и отходите. Ковбой забирает 1$ 'за моральный ущерб'.\n";
            player->addMoney(-1);
        } else if (choice == "2") {
            if (player->getInventory().hasItem("Старый револьвер")) {
                cout << "\nВы медленно достаете револьвер. Ковбой бледнеет:\n";
                cout << "'Эй, дружище, я пошутил! Возьми мои деньги!' (получаете 20$)\n";
                player->addMoney(20);
                player->setHasFought(true);
            } else {
                cout << "\nУ вас нет оружия! Ковбой смеется и бьет вас.\n";
                player->takeDamage(2);
            }
        } else if (choice == "3") {
            cout << "\nВы ввязываетесь в драку. Это жестокая битва!\n";
            player->takeDamage(2);
            cout << "Вам удается победить, но вы сильно ранены.\n";
            player->setHasFought(true);
        }
        
        fightHappened = true;
    }
};

// ==================== ОФИС ШЕРИФА ====================
class SheriffOffice : public Location {
private:
    bool jobAccepted;
    bool rewardTaken;
    
public:
    SheriffOffice() : Location("Офис шерифа", "Скромный кабинет с клеткой для преступников"), 
                      jobAccepted(false), rewardTaken(false) {}
    
    void enter(Player* player) override {
        cout << "\n=== ОФИС ШЕРИФА ===\n";
        cout << description << "\n";
        cout << "Шериф Джонсон сидит за столом, изучая бумаги.\n";
        cout << "На стене висит несколько объявлений о розыске.\n";
        
        if (player->hasFought() && !rewardTaken) {
            cout << "Шериф смотрит на вас с уважением: 'Слышал, ты разобрался с тем головорезом в салуне.'\n";
        }
    }
    
    void explore(Player* player) override {
        cout << "\nВы осматриваете офис:\n";
        cout << "1. Шериф Джонсон за своим столом\n";
        cout << "2. Объявления о розыске на стене\n";
        cout << "3. Ящик с наградными деньгами (неприкрытый)\n";
        cout << "4. Выход на главную площадь\n";
    }
    
    void handleInput(Player* player, const string& input) override {
        if (input == "1") {
            offerJob(player);
        } else if (input == "2") {
            cout << "\nВы изучаете объявления:\n";
            cout << "Большинство - о банде 'Призраки прерий'.\n";
            cout << "Особо опасен их лидер - 'Слепой Джек'.\n";
            player->getInventory().addItem(new WantedPoster());
        } else if (input == "3") {
            if (!jobAccepted) {
                cout << "\nЯщик заперт. Шериф: 'Это награда за поимку бандитов.'\n";
            } else if (rewardTaken) {
                cout << "\nЯщик пуст. Вы уже забрали награду.\n";
            } else {
                cout << "\n1. Взять заслуженную награду\n";
                cout << "2. Украсть все деньги\n";
                
                string choice;
                getline(cin, choice);
                
                if (choice == "1") {
                    takeReward(player);
                } else if (choice == "2") {
                    stealReward(player);
                }
            }
        } else if (input == "4") {
            cout << "\nВы выходите из офиса шерифа на главную площадь...\n";
        } else if (input == "5") {
            player->showStatus();
        } else if (input == "6") {
            player->getInventory().showInventory();
        } else if (input == "инвентарь") {
            player->getInventory().showInventory();
        } else if (input == "статус") {
            player->showStatus();
        } else {
            cout << "Неизвестная команда. Попробуйте снова.\n";
        }
    }
    
    string getMenu() override {
        return "\nДоступные действия:\n"
               "1. Поговорить с шерифом\n"
               "2. Изучить объявления о розыске\n"
               "3. Проверить ящик с наградами\n"
               "4. Выйти на площадь\n"
               "5. Показать статус\n"
               "6. Показать инвентарь\n"
               "> ";
    }
    
private:
    void offerJob(Player* player) {
        if (!jobAccepted) {
            cout << "\nШериф: 'Вижу ты новенький в городе. Ищешь работу?\n";
            cout << "Банда 'Призраки прерий' опять орудует. Нужен человек со стальными нервами.\n";
            cout << "Поймаешь лидера - 500$ твои. Но тебе понадобится оружие.'\n";
            
            cout << "\n1. Согласиться на работу\n";
            cout << "2. Отказаться\n";
            
            string choice;
            getline(cin, choice);
            
            if (choice == "1") {
                if (player->getInventory().hasItem("Старый револьвер")) {
                    cout << "\nШериф: 'Отлично! Вижу у тебя есть оружие. Держи аванс - 50$.'\n";
                    player->addMoney(50);
                    player->setHasJob(true);
                    jobAccepted = true;
                    cout << "Шериф: 'Ищи их в старом амбаре к северу. И будь осторожен.'\n";
                } else {
                    cout << "\nШериф: 'У тебя даже оружия нет? Вернись, когда раздобудешь револьвер.'\n";
                }
            } else {
                cout << "\nШериф: 'Как знаешь. Но деньги в этом городе зарабатываются трудом.'\n";
            }
        } else if (!rewardTaken) {
            cout << "\nШериф: 'Ты уже на работе. Иди ищи банду в старом амбаре.'\n";
        } else {
            cout << "\nШериф: 'Спасибо за помощь, шериф. Город теперь в безопасности.'\n";
        }
    }
    
    void takeReward(Player* player) {
        if (player->hasFought() && player->hasJob()) {
            cout << "\nШериф: 'Ты не только поймал бандита в салуне, но и помог с бандой?\n";
            cout << "Парень, ты просто герой! Забирай награду - 500$ и должность заместителя!'\n";
            player->addMoney(500);
            rewardTaken = true;
        } else if (player->hasFought()) {
            cout << "\nШериф: 'За того бандита из салуна - вот твои 100$.'\n";
            player->addMoney(100);
            rewardTaken = true;
        } else {
            cout << "\nШериф: 'Награду нужно заработать. Помоги с бандой или приведи того бандита.'\n";
        }
    }
    
    void stealReward(Player* player) {
        cout << "\nПока шериф отвернулся, вы забираете все деньги из ящика (800$)!\n";
        player->addMoney(800);
        rewardTaken = true;
        
        cout << "\nШериф оборачивается: 'Эй! Что ты делаешь?!'\n";
        cout << "Вас объявляют в розыск! Придется бежать из города...\n";
        player->takeDamage(1);
    }
};

// ==================== КЛАСС ИГРЫ ====================
class Game {
private:
    Player player;
    Location* currentLocation;
    TownSquare townSquare;
    Saloon saloon;
    SheriffOffice sheriffOffice;
    bool isRunning;
    
    void initialize() {
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
    
    void processCommand(const string& command) {
        if (command.rfind("использовать ", 0) == 0) {
            try {
                int itemIndex = stoi(command.substr(12));
                player.useItem(itemIndex);
                return;
            } catch (...) {
                cout << "Неверный формат. Используйте: использовать [номер предмета]\n";
                return;
            }
        }
        
        // Обработка перемещения
        if (command == "салун") {
            currentLocation = &saloon;
            player.setCurrentLocation(currentLocation);
            currentLocation->enter(&player);
            return;
        } else if (command == "шериф" || command == "офис") {
            currentLocation = &sheriffOffice;
            player.setCurrentLocation(currentLocation);
            currentLocation->enter(&player);
            return;
        } else if (command == "площадь" || command == "город") {
            currentLocation = &townSquare;
            player.setCurrentLocation(currentLocation);
            currentLocation->enter(&player);
            return;
        }
        
        // Обработка команд текущей локации
        currentLocation->handleInput(&player, command);
    }
    
    void checkEndConditions() {
        // Концовка 1: Закон и порядок (стал заместителем шерифа)
        if (player.hasJob() && player.getMoney() >= 550 && player.getHealth() > 0) {
            showEnding(1);
            isRunning = false;
            return;
        }
        
        // Концовка 2: Вольная птица (выиграл в покер, стал хозяином салуна)
        if (player.getInventory().hasItem("Счастливая монета") && 
            player.getMoney() >= 300 && 
            !player.hasJob() && 
            player.getHealth() > 0) {
            showEnding(2);
            isRunning = false;
            return;
        }
        
        // Концовка 3: Песок на ветру (украл у шерифа и сбежал)
        if (player.getMoney() >= 800 && player.getHealth() <= 1) {
            showEnding(3);
            isRunning = false;
            return;
        }
    }
    
    void showEnding(int ending) {
        cout << "\n\n========================================\n";
        cout << "          КОНЕЦ ИСТОРИИ\n";
        
        switch (ending) {
            case 1:
                cout << "       КОНЦОВКА: ЗАКОН И ПОРЯДОК\n";
                cout << "========================================\n\n";
                cout << "Вы помогли шерифу обезвредить банду 'Призраки прерий'.\n";
                cout << "За ваши заслуги вас назначили заместителем шерифа.\n";
                cout << "Теперь вы защитник закона в Грэйсбери, и у вас есть дом,\n";
                cout << "уважение горожан и стабильное жалование.\n";
                cout << "Может быть, это не самая богатая жизнь, но честная.\n";
                break;
                
            case 2:
                cout << "       КОНЦОВКА: ВОЛЬНАЯ ПТИЦА\n";
                cout << "========================================\n\n";
                cout << "Ваша удача в покере принесла вам состояние.\n";
                cout << "Вы выкупили салун 'Последний шанс' и стали его хозяином.\n";
                cout << "Теперь вы слушаете истории странников, наблюдаете за игрой\n";
                cout << "и наслаждаетесь свободой. Деньги текут рекой,\n";
                cout << "а уважение зарабатывается не оружием, а умением вести дела.\n";
                break;
                
            case 3:
                cout << "       КОНЦОВКА: ПЕСОК НА ВЕТРУ\n";
                cout << "========================================\n\n";
                cout << "Вы украли награду у шерифа и бежали из города под выстрелы.\n";
                cout << "С деньгами в кармане и пулей в плече вы скрываетесь в прериях.\n";
                cout << "Теперь вы вне закона, вечно оглядывающийся через плечо.\n";
                cout << "Деньги скоро закончатся, а клеймо предателя останется навсегда.\n";
                break;
        }
        
        cout << "\n========================================\n";
        cout << "Финал вашей истории на Диком Западе...\n";
        cout << "========================================\n";
    }
    
public:
    Game() : currentLocation(nullptr), isRunning(true) {
        srand(static_cast<unsigned int>(time(0)));
        initialize();
    }
    
    void run() {
        string command;
        
        while (isRunning && player.isAlive()) {
            cout << currentLocation->getMenu();
            getline(cin, command);
            
            if (command == "выход" || command == "exit") {
                cout << "\nДо свидания, ковбой!\n";
                break;
            }
            
            processCommand(command);
            checkEndConditions();
        }
        
        if (!player.isAlive()) {
            cout << "\n========================================\n";
            cout << "          ВАС НЕ СТАЛО\n";
            cout << "Вы пали в жестоком мире Дикого Запада.\n";
            cout << "========================================\n";
        }
    }
};

// ==================== ГЛАВНАЯ ФУНКЦИЯ ====================
int main() {
    #ifdef _WIN32
        system("chcp 65001 > nul");
    #endif
    
    cout << "========================================\n";
    cout << "   ДОБРО ПОЖАЛОВАТЬ В КОВБОЙСКИЙ КВЕСТ!\n";
    cout << "========================================\n\n";
    
    cout << "Управление:\n";
    cout << "- Используйте цифры для выбора действий\n";
    cout << "- Команды: 'инвентарь', 'статус', 'выход'\n";
    cout << "- Использовать предмет: 'использовать [номер]'\n";
    cout << "- Для перемещения между локациями используйте:\n";
    cout << "  'салун', 'шериф', 'площадь'\n\n";
    
    cout << "Нажмите Enter чтобы начать...";
    cin.get();
    
    Game game;
    game.run();
    
    cout << "\nСпасибо за игру!\n";
    return 0;
}
