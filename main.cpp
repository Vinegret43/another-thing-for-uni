#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <random>
#include <bits/stdc++.h>

#include "config.cpp"
#include "lyrics.cpp"

using namespace std;


// ==================== DEFINITIONS ====================

class Dungeon;
class Room;
class Creature;
class Item;

class Item {
protected:
    const char* description;
    const char* name;
public:
    virtual void use(Creature *user, Creature *target = nullptr) {};
    virtual bool requires_target() { return false; };
    const char* get_name() {
        return this->name;
    };
    const char* get_description() {
        return this->description;
    };
    virtual string to_string() { return this->name; };
    Item(const char* name, const char* description);
    friend ostream& operator<<(ostream&, Item&);
};

class Armor: public Item {
protected:
    int armor_class;
public:
    int get_armor_class();
    void use(Creature *user, Creature *target = nullptr) override;
    string to_string() override;
    Armor(int armor_class, const char* name, const char* description);
};

class Weapon: public Item {
protected:
    int hit_dice_sides;
public:
    void attack(Creature *attacker, Creature *target);
    void use(Creature *user, Creature *target = nullptr) override;
    int get_hit_dice_sides();
    string to_string() override;
    Weapon(int hit_dice_sides, const char* name, const char* description);
};

class MagicArmor: public Armor, public Weapon {};

class Creature {
protected:
    int health;
    int max_health;
    int dexterity;
    int strength;
    char* name;
    vector<Item*> inventory;
    Armor* armor;
    Weapon* weapon;
    Room* room;
public:
    int get_health() { return this->health; }
    void add_item(Item *i);
    void remove_item(Item *i);
    void wound();
    bool is_dead();
    virtual void make_turn() {};
    const char* faction_id();
    const char* get_name() {
        return this->name;
    }
    virtual string to_string() {
        return this->name;
    };
    Room* get_current_room() {
        return this->room;
    }
    const vector<Item*>* get_inventory() {
        return &(this->inventory);
    }
    void set_room(Room* room) {
        this->room = room;
    }
    void set_weapon(Weapon* weapon) {
        this->weapon = weapon;
    }
    void set_armor(Armor* armor) {
        this->armor = armor;
    }
    friend ostream& operator<<(ostream&, Item&);
    Creature(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor);
    // TODO: деструктор. Предметы из инвентаря надо подчищать
};

/// \private
struct MoveTo {
    Room* room;
};

/// \private
struct AttackCreature {
    Creature* target;
};

/// \private
struct UseItem {
    int item_number;
    int target_number;
};

/// \private
struct TakeItem {
    int item_number;
};

using Turn = variant<MoveTo, AttackCreature, UseItem, TakeItem>;

class PlayerCharacter: public Creature {
    Turn turn;
public:
    void inspect_room();
    void inspect_self();
    void set_turn(Turn turn);
    void make_turn() override;
    PlayerCharacter(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor);
};

class Ork: Creature {

};

class OrkWarrior: Ork {

};

class OrkArcher: Ork {

};


class Room {
    vector<Item*> items;
    vector<Creature*> creatures;
    vector<Room*> connected_rooms;
public:
    void spawn_creature(Creature* creature) {
        creature->set_room(this);
        this->creatures.push_back(creature);
    };
    void remove_creature(Creature creature);
    void move_creature(Creature creature, Room* from, Room* to);
    void add_item(Item item);
    Item* pop_item(int index) {
        assert(this->items.size() > index);
        return this->items.erase(this->items.begin() + index)[0];
    }
    const vector<Item*>* get_items() {
        return &(this->items);
    }
    const vector<Creature*>* get_creatures() {
        return &(this->creatures);
    }
    void play_round();
    char* description();
    Room(vector<Item*> items, vector<Room*> connected_rooms):
        items(items), connected_rooms(connected_rooms) {};
    // От деструктора нет смысла: каждая комната живёт
    // в памяти до самого завершения программы
};

using RoomGrid = vector<vector<variant<Room, long int>>>;

class Dungeon {
    RoomGrid rooms;
    PlayerCharacter *player;
public:
    void play_round();
    PlayerCharacter* get_player() {
        return this->player;
    }
    Dungeon(RoomGrid rooms, PlayerCharacter *player);
};


// ==================== IMPLEMENTATIONS ====================

Item::Item(const char* name, const char* description): name(name), description(description) {}

ostream& operator<<(ostream& s, Item& item) {
    s << item.to_string();
    return s;
}

ostream& operator<<(ostream& s, Creature& creature) {
    s << creature.to_string();
    return s;
}

Armor::Armor(int cls, const char* name, const char* description): armor_class(cls), Item(name, description) {}

string Armor::to_string() {
    string s;
    s = this->name;
    s += " (";
    s += std::to_string(this->get_armor_class());
    s += ")";
    return s;
}

int Armor::get_armor_class() {
    return this->armor_class;
}

void Armor::use(Creature* user, Creature* target) {
    user->set_armor(this);
}

Weapon::Weapon(int dmg, const char* name, const char* description): hit_dice_sides(dmg), Item(name, description) {}

string Weapon::to_string() {
    string s;
    s = this->name;
    s += " (1к";
    s += std::to_string(this->get_hit_dice_sides());
    s += ")";
    return s;
}

int Weapon::get_hit_dice_sides() {
    return this->hit_dice_sides;
}

void Weapon::use(Creature* user, Creature* target) {
    user->set_weapon(this);
}

Creature::Creature(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor):
    health(health), max_health(health), dexterity(dex), strength(str), inventory(inv), weapon(weapon), armor(armor) {}

bool Creature::is_dead() {
    return this->health <= 0;
}

void PlayerCharacter::inspect_room() {
    cout << "Предметы:" << endl;
    const vector<Item*>* items = room->get_items();
    if (items->size() == 0) {
        cout << "Тут пусто :(" << endl;
    } else {
        for (int i = 0; i < items->size(); i++) {
        cout << i+1 << ". " << *((*items)[i]) << endl;
        }
    }

    cout << "Существа:" << endl;
    const vector<Creature*>* creatures = room->get_creatures();
    if (creatures->size() == 1) {
        cout << "Ты тут один... Может оно и к лучшему" << endl;
    } else {
        for (int i = 0; i < creatures->size(); i++) {
            cout << i+1 << ". " << (*creatures)[i] << endl;
        }
    }
}

void PlayerCharacter::inspect_self() {
    cout << "Здоровье: " << this->health << "/" << this->max_health << endl;
    cout << "Ловкость: " << this->dexterity << endl;
    cout << "Сила: " << this->strength << endl;
    cout << "Оружие: " << *(this->weapon) << endl;
    cout << "Доспехи: " << *(this->armor) << endl;
    cout << "Инвентарь: " << endl;
    for (int i = 0; i < this->inventory.size(); i++) {
        cout << i+1 << ". " << *(this->inventory[i]);
        if (this->inventory[i] == this->weapon || this->inventory[i] == this->armor) {
            cout << " (экипировано)";
        }
        cout << endl;
    }
}

void PlayerCharacter::set_turn(Turn turn) {
    this->turn = turn;
}

void PlayerCharacter::make_turn() {
    if (holds_alternative<TakeItem>(this->turn)) {
        Item* item = this->room->pop_item(get<TakeItem>(this->turn).item_number - 1);
        this->inventory.push_back(item);
    } else if (holds_alternative<UseItem>(this->turn)) {
        UseItem action = get<UseItem>(this->turn);
        // TODO: target check. Convert target number to pointer if it's required
        this->inventory[action.item_number-1]->use(this);
    }
}

PlayerCharacter::PlayerCharacter(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor):
    Creature(health, dex, str, inv, weapon, armor) {}


void Room::play_round() {
    for (Creature* i: this->creatures) {
        i->make_turn();
    }
}

Dungeon::Dungeon(RoomGrid rooms, PlayerCharacter *player): player(player), rooms(rooms) {}

void Dungeon::play_round() {
    this->player->get_current_room()->play_round();
}


// ==================== UTILITIES ====================

int randint(int min, int max) {
    return min + rand() % (max+1-min);
}

vector<string> split_string(string s) {
    string buf;
    stringstream ss(s);
    vector<string> v;
    while (getline(ss, buf, ' ')) {
        if (buf == "") continue;
        v.push_back(buf);
    }
    return v;
}

bool is_numeric(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int parse_integer(const std::string& s, int min = 0) {
    if (!is_numeric(s)) {
        throw std::invalid_argument("Допустимы только численные символы от 0 до 9");
    }
    int i = stoi(s, nullptr, 10);
    if (i < min) {
        throw std::invalid_argument("Значение меньше указанного минимума");
    }
    return i;
}

// ==================== APPLICATION CODE ====================

Dungeon generate_dungeon() {
    int health = randint(MIN_STARTING_HELATH, MAX_STARTING_HELATH);
    int str = randint(MIN_STARTING_STR, MAX_STARTING_STR);
    int dex = randint(MIN_STARTING_DEX, MAX_STARTING_DEX);
    vector<Item*> inventory;
    inventory.push_back(new Armor(14, "Кольчуга", ""));
    Weapon* weapon = new Weapon(4, "Кинжал", "");
    Armor* armor = new Armor(12, "Проклёпанная кожа", "");
    inventory.push_back(weapon);
    inventory.push_back(armor);
    PlayerCharacter *player = new PlayerCharacter(health, str, dex, inventory, weapon, armor);

    RoomGrid room_grid;
    for (int i = 0; i < DUNGEON_SIZE; i++) {
        vector<variant<Room, long int>> row;
        for (int j = 0; j < DUNGEON_SIZE; j++) {
            variant<Room, long int> element = NULL;
            row.push_back(element);
        }
        room_grid.push_back(row);
    }

    vector<Item*> room_inventory;
    room_inventory.push_back(new Weapon(8, "Длинный меч", ""));
    vector<Room*> connected_rooms;
    Room* starting_room = new Room(room_inventory, connected_rooms);
    starting_room->spawn_creature(player);

    player->get_current_room() == starting_room;

    // TODO: комната не добавляется в Dungeon. Должна добавляться
    Dungeon dungeon(room_grid, player);
    return dungeon;
}

const char* CMD_HELP = R"(Список доступных команд:
me - осмотреть самого себя
room - осмотреть текущую комнату
use [предмет] ([цель]) - использовать предмет из инвентаря. Если цель=0, использование на себе
take [предмет] - взять предмет из инвентаря комнаты
attack [цель] - атаковать существо основным оружием
go [right/top/left/bottom] - переместиться в соседнюю комнату
help - получить помощь по командам
quit - выйти из приложения)";

int main(int argc, char* argv[]) {
    if (argc > 1) {
        int seed = parse_integer(argv[1]);
        srand(seed);
    } else {
        srand(time(0));
    }

    Dungeon dungeon = generate_dungeon();

    cout << MSG_INTRODUCTION << endl;

    string cmd;
    while (true) {
        if (dungeon.get_player()->is_dead()) {
            break;
        }
        cout << "> ";
        getline(cin, cmd);
        vector<string> split = split_string(cmd);
        if (split.size() == 0) {
            continue;
        }
        if (!split[0].compare("me")) {
            dungeon.get_player()->inspect_self();
        } else if (!split[0].compare("use")) {
            vector<string> split = split_string(cmd);
            if (split.size() < 2) {
                cout << "Укажите номер предмета в инвентаре" << endl;
                continue;
            }
            int item_number;
            try {
                item_number = parse_integer(split[1], 1);
            } catch (std::invalid_argument _) {
                cout << "Неверный аргумент команды. Требуется натуральное число" << endl;
                continue;
            }

            const vector<Item*>* inventory = dungeon.get_player()->get_inventory();
            if (item_number > inventory->size() || item_number < 1) {
                cout << "В инвентаре нет предмета с таким номером" << endl;
                continue;
            }

            int target_number = 0;

            if ((*inventory)[item_number-1]->requires_target()) {
                if (split.size() < 3) {
                    cout << "Укажите номер цели, к которой необходимо применить предмет" << endl;
                    continue;
                }
                try {
                    target_number = parse_integer(split[2], 0);
                } catch (std::invalid_argument _) {
                    cout << "Неверный аргумент команды. Требуется целое неотрицательное число" << endl;
                    continue;
                }
                const vector<Creature*>* creatures = dungeon.get_player()->get_current_room()->get_creatures();
                if (target_number > creatures->size() - 1) {
                    cout << "Нет цели с таким номером" << endl;
                    continue;
                }
            }

            UseItem action = { item_number, target_number };
            dungeon.get_player()->set_turn(action);
            dungeon.play_round();
        } else if (!split[0].compare("take")) {
            vector<string> split = split_string(cmd);
            if (split.size() < 2) {
                cout << "Укажите номер предмета в инвентаре" << endl;
                continue;
            }
            int item_number;
            try {
                item_number = parse_integer(split[1], 1);
            } catch (std::invalid_argument _) {
                cout << "Неверный аргумент команды. Требуется натуральное число" << endl;
                continue;
            }

            const vector<Item*>* items = dungeon.get_player()->get_current_room()->get_items();
            if (item_number > items->size() || item_number < 1) {
                cout << "В комнате нет предмета с таким номером" << endl;
                continue;
            }

            TakeItem action = { item_number };
            dungeon.get_player()->set_turn(action);
            dungeon.play_round();
        } else if (!split[0].compare("attack")) {
            int target_number = 0;

            if (split.size() < 2) {
                cout << "Укажите номер цели, которую хотите атаковать" << endl;
                continue;
            }
            try {
                target_number = parse_integer(split[1], 1);
            } catch (std::invalid_argument _) {
                cout << "Неверный аргумент команды. Требуется натуральное число" << endl;
                continue;
            }
            const vector<Creature*>* creatures = dungeon.get_player()->get_current_room()->get_creatures();
            if (target_number > creatures->size() - 1) {
                cout << "Нет цели с таким номером" << endl;
                continue;
            }
            // TODO: set action and play round
        } else if (!split[0].compare("go")) {
            cout << "Where he go" << endl;
        } else if (!split[0].compare("room")) {
            dungeon.get_player()->inspect_room();
        } else if (!split[0].compare("quit")) {
            break;
        } else if (!split[0].compare("help")) {
            cout << CMD_HELP << endl;
        } else {
            cout << "Неизвестная команда: " << cmd << 
                ". Используйте команду \"help\", чтобы получить помощь" << endl;
        }
    }

    return 0;
}
