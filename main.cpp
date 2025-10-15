#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <variant>
#include <random>
#include <bits/stdc++.h>

#include "config.cpp"

using namespace std;


// ==================== DEFINITIONS ====================

int randint(int, int);

class Dungeon;
class Room;
class Creature;
class Item;

class Item {
protected:
    string name;
public:
    virtual void use(Creature *user, Creature *target = nullptr) {};
    virtual bool requires_target() { return false; };
    const string get_name() {
        return this->name;
    };
    virtual string to_string() { return this->name; };
    Item(string name): name(name) {};
    friend ostream& operator<<(ostream&, Item&);
};

class Armor: virtual public Item {
protected:
    int armor_class;
public:
    int get_armor_class() {
        return this->armor_class;
    }
    virtual void use(Creature* user, Creature* _target) override;
    virtual void unequip(Creature* user);
    virtual string to_string() override {
        string s;
        s = this->name;
        s += " (";
        s += std::to_string(this->get_armor_class());
        s += ")";
        return s;
    }
    Armor(int armor_class, string name): armor_class(armor_class), Item(name) {}
};

class Weapon: virtual public Item {
protected:
    int hit_dice_sides;
public:
    // Returns dealt damage. If attack failed, damage is 0
    int attack(Creature* attacker, Creature* target);
    virtual void use(Creature* user, Creature* _target) override;
    virtual void unequip(Creature* user);
    int get_hit_dice_sides() {
        return this->hit_dice_sides;
    }
    virtual string to_string() override {
        string s;
        s = this->name;
        s += " (1к";
        s += std::to_string(this->get_hit_dice_sides());
        s += ")";
        return s;
    }
    Weapon(int hit_dice_sides, string name): hit_dice_sides(hit_dice_sides), Item(name) {}
};

class MagicArmor: public Armor, public Weapon {
public:
    void use(Creature* user, Creature* _target) override;
    void unequip(Creature* user) override;
    string to_string() override {
        string s;
        s = this->name;
        s += " (1к";
        s += std::to_string(this->get_hit_dice_sides());
        s += ") ";
        s += "(";
        s += std::to_string(this->get_armor_class());
        s += ")";
        return s;
    }
    MagicArmor(int dmg, int protection, string name):
        Weapon(dmg, name), Armor(dmg, name), Item(name) {}
};

class HealingPotion: public Item {
    int hits;
public:
    HealingPotion(int hits): hits(hits), Item("Зелье лечения") {}
    static HealingPotion* generate(float danger_level) {
        return new HealingPotion(randint(4, 8*(danger_level+1)));
    }
    void use(Creature* user, Creature* _target) override;
};

class StrengthPotion: public Item {
    int increase;
public:
    StrengthPotion(int increase): increase(increase), Item("Зелье силы") {}
    static StrengthPotion* generate(float _danger_level) {
        return new StrengthPotion(randint(1, 2));
    }
    void use(Creature* user, Creature* _target) override;
};

class TeleportPotion: public Item {
public:
    TeleportPotion(): Item("Зелье телепортации") {}
    void use(Creature* user, Creature* _target) override;
};

class DexterityPotion: public Item {
    int increase;
public:
    DexterityPotion(int increase): increase(increase), Item("Зелье ловкости") {}
    static DexterityPotion* generate(float _danger_level) {
        return new DexterityPotion(randint(1, 2));
    }
    void use(Creature* user, Creature* _target) override;
};

class Creature {
protected:
    int health;
    int max_health;
    int dexterity;
    int strength;
    int faction_id;
    string name;
    vector<Item*> inventory;
    Armor* armor;
    Weapon* weapon;
    Room* room;
public:
    int get_health() {
        return this->health;
    }
    void set_health(int health) {
        this->health = min(health, this->max_health);
    }
    void remove_item(Item* i) {
        this->inventory.erase(
            std::remove(this->inventory.begin(), this->inventory.end(), i),
            this->inventory.end()
        );
    };
    vector<Item*> clear_inventory() {
        vector<Item*> inv;
        this->inventory.swap(inv);
        return inv;
    }
    virtual void wound(int damage, Creature* _source) {
        this->health -= damage;
    }
    bool is_alive() {
        return this->health > 0;
    };
    virtual void make_turn() {};
    int get_faction_id() { 
        return this->faction_id;
    };
    const string get_name() {
        return this->name;
    }
    virtual string to_string() {
        return this->name;
    };
    Room* get_current_room() {
        return this->room;
    }
    // Pass the result of your hit roll (With modificators included) in here.
    // Will return true if attack was successful, after that you can call wound()
    virtual bool try_attack(int hit_roll) {
        if (this->armor != nullptr) {
            return hit_roll >= this->armor->get_armor_class();
        } else {
            return hit_roll >= 8;
        }
    }
    const vector<Item*>* get_inventory() {
        return &(this->inventory);
    }
    void set_room(Room* room) {
        this->room = room;
    }
    Room* get_room() {
        return this->room;
    }
    void set_weapon(Weapon* weapon) {
        this->weapon = weapon;
    }
    void unset_weapon() {
        if (this->weapon != nullptr)
            this->weapon->unequip(this);
    }
    void set_armor(Armor* armor) {
        this->armor = armor;
    }
    void unset_armor() {
        if (this->armor != nullptr)
            this->armor->unequip(this);
    }
    int get_strength() {
        return this->strength;
    }
    int get_dexterity() {
        return this->dexterity;
    }
    void set_strength(int strength) {
        this->strength = min(strength, MAX_STRENGTH);
    }
    void set_dexterity(int dexterity) {
        this->dexterity = min(dexterity, MAX_DEXTERITY);
    }
    friend ostream& operator<<(ostream&, Item&);
    Creature(int health, int str, int dex, vector<Item*> inv,
             Weapon* weapon, Armor* armor, int faction_id = 0, string name = ""):
        health(health), max_health(health), dexterity(dex), strength(str),
        inventory(inv), weapon(weapon), armor(armor), faction_id(faction_id), name(name) {}
    ~Creature() {
        // armor и weapon в любом случае указывают на предметы, находящиеся в инвентаре.
        // Дополнительно подчищать их не надо
        for (int i = 0; i < inventory.size(); i++) {
            delete inventory[i];
        }
    }
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
    Weapon fists;
public:
    void inspect_room();
    void inspect_self();
    void set_turn(Turn turn) {
        this->turn = turn;
    }
    void make_turn() override;
    void wound(int damage, Creature* source) override;
    PlayerCharacter(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor):
        Creature(health, dex, str, inv, weapon, armor, 1), fists(Weapon(4, "Кулаки")) {}
    static PlayerCharacter* generate() {
        int health = randint(MIN_STARTING_HELATH, MAX_STARTING_HELATH);
        int str = randint(MIN_STARTING_STR, MAX_STARTING_STR);
        int dex = randint(MIN_STARTING_DEX, MAX_STARTING_DEX);
        vector<Item*> inventory;
        Weapon* weapon = new Weapon(4, "Кинжал");
        Armor* armor = new Armor(12, "Проклёпанная кожа");
        inventory.push_back(weapon);
        inventory.push_back(armor);
        return new PlayerCharacter(health, str, dex, inventory, weapon, armor);
    }
};

class Ork: public Creature {
protected:
    Ork(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor, string name):
        Creature(health, str, dex, inv, weapon, armor, 0, name) {};
public:
    void make_turn() override;
};

class OrkWarrior: public Ork {
public:
    OrkWarrior(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor, string name):
        Ork(health, str, dex, inv, weapon, armor, name) {};

    static OrkWarrior* generate(float danger_level) {
        int health = randint(6, 6*(danger_level+1));
        int str = randint(1, 2*(danger_level+1));
        int dex = randint(1, 2*(danger_level+1));

        Weapon* weapon = new Weapon(randint(3, 4*(danger_level+1)), "Воинский меч");
        Armor* armor = new Armor(randint(11, 13*(danger_level/3+1)), "Кольчуга");
        vector<Item*> inv {weapon, armor};

        return new OrkWarrior(health, str, dex, inv, weapon, armor, "Орк-воин");
    }
};

class OrkArcher: public Ork {
public:
    OrkArcher(int health, int str, int dex, vector<Item*> inv, Weapon* weapon, Armor* armor, string name):
        Ork(health, str, dex, inv, weapon, armor, name) {};

    static OrkArcher* generate(float danger_level) {
        int health = randint(4, 4*(danger_level+1));
        int str = randint(1, 2);
        int dex = randint(2, 2*(danger_level+1));

        Weapon* weapon = new Weapon(randint(4, 5*(danger_level+1)), "Лук");
        Armor* armor = new Armor(randint(7, 8*(danger_level/2+1)), "Проклёпанная кожа");
        vector<Item*> inv {weapon, armor};

        return new OrkArcher(health, str, dex, inv, weapon, armor, "Орк-лучник");
    }
};

class Room {
    vector<Item*> items;
    vector<Creature*> creatures;
    optional<Room*> room_to_top;
    pair<int, int> pos;
    Dungeon* dungeon;
    int count_neighbours();
public:
    void spawn_creature(Creature* creature) {
        creature->set_room(this);
        this->creatures.push_back(creature);
    };
    void remove_creature(Creature* creature) {
        this->creatures.erase(
            std::remove(this->creatures.begin(), this->creatures.end(), creature),
            this->creatures.end()
        );
    };
    void add_item(Item* item) {
        this->items.push_back(item);
    }
    Item* pop_item(int index) {
        Item* i = this->items[index];
        this->items.erase(this->items.begin() + index);
        return i;
    }
    const vector<Item*>* get_items() {
        return &(this->items);
    }
    const vector<Creature*>* get_creatures() {
        return &(this->creatures);
    }
    Dungeon* get_dungeon() {
        return this->dungeon;
    }
    void play_round();
    void clear_dead_creatures();
    bool is_exit() {
        if (pos.first == 0 || pos.first == DUNGEON_SIZE-1 || pos.second == 0 || pos.second == DUNGEON_SIZE-1) {
            if (count_neighbours() == 1) {
                return true;
            }
        }
        return false;
    }
    void print_neighbour_directions();
    pair<int, int> get_pos() {
        return this->pos;
    }
    Room(Dungeon* dungeon, pair<int, int> pos): dungeon(dungeon), pos(pos) {}
    // От деструктора нет смысла: каждая комната живёт в памяти
    // программы до самого её завершения, поэтому утечки быть не может
};

using RoomGrid = vector<vector<optional<Room*>>>;

class Dungeon {
    RoomGrid rooms;
    PlayerCharacter *player;
public:
    void play_round();
    void clear_dead_creatures();
    PlayerCharacter* get_player() {
        return this->player;
    }
    const RoomGrid* get_grid() {
        return &(this->rooms);
    }
    void set_grid(RoomGrid grid) {
        this->rooms = grid;
    }
    optional<Room*> get_room(pair<int, int> pos) {
        if (min(pos.first, pos.second) < 0 || max(pos.first, pos.second) > DUNGEON_SIZE-1) {
            return nullopt;
        }
        return this->rooms[pos.first][pos.second];
    }
    Dungeon(PlayerCharacter *player): player(player) {}
};


// ==================== IMPLEMENTATIONS ====================

ostream& operator<<(ostream& s, Item& item) {
    s << item.to_string();
    return s;
}

ostream& operator<<(ostream& s, Creature& creature) {
    s << creature.to_string();
    return s;
}

void Armor::use(Creature* user, Creature* target) {
    user->unset_armor();
    user->set_armor(this);
}

void Armor::unequip(Creature* user) {
    user->set_armor(nullptr);
}

void MagicArmor::use(Creature* user, Creature* _target) {
    user->set_weapon(this);
    user->set_armor(this);
}

void MagicArmor::unequip(Creature* user) {
    user->set_weapon(nullptr);
    user->set_armor(nullptr);
}

void HealingPotion::use(Creature* user, Creature* _target) {
    user->set_health(user->get_health() + this->hits);
    user->remove_item(this);
    delete this;
}

void StrengthPotion::use(Creature* user, Creature* _target) {
    user->set_strength(user->get_strength() + this->increase);
    user->remove_item(this);
    delete this;
}

void DexterityPotion::use(Creature* user, Creature* _target) {
    user->set_dexterity(user->get_dexterity() + this->increase);
    user->remove_item(this);
    delete this;
}

void TeleportPotion::use(Creature* user, Creature* _target) {
    Room* depart = user->get_room();
    Dungeon* dungeon = depart->get_dungeon();
    optional<Room*> dest;
    while (!dest || dest.value()->is_exit()) {
        pair<int, int> pos = {randint(0, DUNGEON_SIZE-1), randint(0, DUNGEON_SIZE-1)};
        dest = dungeon->get_room(pos);
    }
    depart->remove_creature(user);
    dest.value()->spawn_creature(user);
    user->remove_item(this);
    delete this;
}

void Weapon::use(Creature* user, Creature* _target) {
    user->unset_weapon();
    user->set_weapon(this);
}

void Weapon::unequip(Creature* user) {
    user->set_weapon(nullptr);
}

int Weapon::attack(Creature* attacker, Creature* target) {
    int hit_roll = randint(1, 20) + attacker->get_dexterity();
    if (target->try_attack(hit_roll)) {
        int dmg_roll = randint(1, this->hit_dice_sides) + attacker->get_strength();
        target->wound(dmg_roll, attacker);
        return dmg_roll;
    }
    return 0;
}

void PlayerCharacter::inspect_room() {
    cout << "Предметы:" << endl;
    const vector<Item*>* items = room->get_items();
    if (items->size() == 0) {
        cout << "  Тут пусто :(" << endl;
    } else {
        for (int i = 0; i < items->size(); i++) {
            cout << "  " << i+1 << ". " << *((*items)[i]) << endl;
        }
    }

    cout << "Существа:" << endl;
    const vector<Creature*>* creatures = room->get_creatures();
    if (creatures->size() == 1) {
        cout << "  Ты тут один... Может оно и к лучшему" << endl;
    } else {
        int actual_count = 0;
        for (int i = 0; i < creatures->size(); i++) {
            if ((*creatures)[i] == this) continue;
            actual_count++;
            Creature* creature = (*creatures)[i];
            cout << "  " << actual_count << ". " << *creature << " [" << creature->get_health() << "]" << endl;
        }
    }
    cout << "Отсюда можно пойти: ";
    room->print_neighbour_directions();
    cout << endl;
}

void PlayerCharacter::inspect_self() {
    cout << "Здоровье: " << this->health << "/" << this->max_health << endl;
    cout << "Ловкость: " << this->dexterity << endl;
    cout << "Сила: " << this->strength << endl;
    if (this->weapon != nullptr) {
        cout << "Оружие: " << *(this->weapon) << endl;
    } else {
        cout << "Оружие: нет" << endl;
    }
    if (this->armor != nullptr) {
        cout << "Доспехи: " << *(this->armor) << endl;
    } else {
        cout << "Доспехи: нет" << endl;
    }
    cout << "Инвентарь: " << endl;
    for (int i = 0; i < this->inventory.size(); i++) {
        cout << "  " << i+1 << ". " << *(this->inventory[i]);
        if (this->inventory[i] == this->weapon || this->inventory[i] == this->armor) {
            cout << " (экипировано)";
        }
        cout << endl;
    }
}

void PlayerCharacter::make_turn() {
    if (holds_alternative<TakeItem>(this->turn)) {
        Item* item = this->room->pop_item(get<TakeItem>(this->turn).item_number - 1);
        this->inventory.push_back(item);
    } else if (holds_alternative<UseItem>(this->turn)) {
        UseItem action = get<UseItem>(this->turn);
        Creature* target = nullptr;
        // TODO: target check. Convert target number to pointer if it's required
        // NOTE: This feature is not used right now, so I'll leave this as is
        cout << "Использован предмет: " << *(this->inventory[action.item_number-1]) << endl;
        this->inventory[action.item_number-1]->use(this, target);
    } else if (holds_alternative<AttackCreature>(this->turn)) {
        AttackCreature action = get<AttackCreature>(this->turn);
        int dmg;
        if (this->weapon == nullptr) {
            dmg = this->fists.attack(this, action.target);
        } else {
            dmg = this->weapon->attack(this, action.target);
        }
        cout << "Ты совершаешь атаку по существу " << *action.target << ", нанося ему ";
        cout << dmg << " урона. ";
        if (action.target->is_alive()) {
            cout << endl;
        } else {
            cout << "Он падает на землю без сознания" << endl;
        }
    }
}

void PlayerCharacter::wound(int damage, Creature* source) {
    cout << "Тебе атаковал " << *source << "! Ты получаешь " << damage << " урона" << endl;
    this->health -= damage;
}

void Ork::make_turn() {
    for (Creature* creature: *(this->room->get_creatures())) {
        if (creature->get_faction_id() != this->faction_id) {
            this->weapon->attack(this, creature);
            break;
        }
    }
}

void Room::play_round() {
    for (Creature* i: this->creatures) {
        if (i->is_alive()) {
            i->make_turn();
        }
    }
}

void Room::clear_dead_creatures() {
    for (int i = 0; i < this->creatures.size(); i++) {
        Creature* creature = this->creatures[i];
        if (!creature->is_alive()) {
            this->creatures.erase(this->creatures.begin() + i);
            vector<Item*> inv = creature->clear_inventory();
            this->items.insert(this->items.end(), inv.begin(), inv.end());
            delete creature;
        }
    }
}

int Room::count_neighbours() {
    const RoomGrid* grid = this->dungeon->get_grid();
    int count = 0;
    if (pos.first > 0 && (*grid)[pos.first-1][pos.second])
        count++;
    if (pos.first < DUNGEON_SIZE-1 && (*grid)[pos.first+1][pos.second])
        count++;
    if (pos.second > 0 && (*grid)[pos.first][pos.second-1])
        count++;
    if (pos.second < DUNGEON_SIZE-1 && (*grid)[pos.first][pos.second+1])
        count++;
    return count;
}

void Room::print_neighbour_directions() {
    const RoomGrid* grid = this->dungeon->get_grid();
    int count = 0;
    if (pos.first > 0 && (*grid)[pos.first-1][pos.second]) {
        cout << "налево";
        count++;
    }
    if (pos.first < DUNGEON_SIZE-1 && (*grid)[pos.first+1][pos.second]) {
        if (count > 0) {
            cout << ", ";
        }
        cout << "направо";
        count++;
    }
    if (pos.second > 0 && (*grid)[pos.first][pos.second-1]) {
        if (count > 0) {
            cout << ", ";
        }
        cout << "вверх";
        count++;
    }
    if (pos.second < DUNGEON_SIZE-1 && (*grid)[pos.first][pos.second+1]) {
        if (count > 0) {
            cout << ", ";
        }
        cout << "вниз";
    }
}

void Dungeon::play_round() {
    this->player->get_current_room()->play_round();
}

void Dungeon::clear_dead_creatures() {
    this->player->get_current_room()->clear_dead_creatures();
}


// ==================== UTILITIES ====================

template <typename T> class LootTable {
    vector<tuple<float, int, T (*)(float)>> items;
public:
    void add_item(float min_danger, int weight, T (*f)(float)) {
        items.push_back(make_tuple(min_danger, weight, f));
    }
    optional<T> gen_item(float danger) {
        vector<tuple<float, int, T(*)(float)>> list;
        int weight_sum = 0;
        for (int i = 0; i < items.size(); i++) {
            if (danger >= get<0>(items[i])) {
                list.push_back(items[i]);
                weight_sum += get<1>(items[i]);
            }
        }
        if (weight_sum == 0)
            return nullopt;
        int r = randint(0, weight_sum - 1);
        for (int i = 0; i < list.size(); i++) {
            if (r < (get<1>(list[i]))) {
                return get<2>(list[i])(danger);
            }
            r -= get<1>(list[i]);
        }
        return nullopt; // Technically, this statement is unreachable
    }
};

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

bool is_numeric(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
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

LootTable<Item*> get_item_loot_table() {
    LootTable<Item*> loot_table;
    loot_table.add_item(0, 20, [](float d) { return (Item*)(HealingPotion::generate(d)); });
    loot_table.add_item(0.1, 5, [](float d) { return (Item*)(StrengthPotion::generate(d)); });
    loot_table.add_item(0.1, 5, [](float d) { return (Item*)(DexterityPotion::generate(d)); });
    loot_table.add_item(0.2, 5, [](float d) { return (Item*)(new TeleportPotion()); });

    loot_table.add_item(0.1, 2, [](float d) { return (Item*)(new Weapon(6, "Короткий меч")); });
    loot_table.add_item(0.2, 3, [](float d) { return (Item*)(new Weapon(8, "Длинный меч")); });
    loot_table.add_item(0.4, 4, [](float d) { return (Item*)(new Weapon(10, "Алебарда")); });
    loot_table.add_item(0.4, 5, [](float d) { return (Item*)(new Weapon(12, "Секира")); });

    loot_table.add_item(0.0, 2, [](float d) { return (Item*)(new Armor(13, "Кольчужная рубаха")); });
    loot_table.add_item(0.2, 3, [](float d) { return (Item*)(new Armor(15, "Полулаты")); });
    loot_table.add_item(0.4, 4, [](float d) { return (Item*)(new Armor(17, "Наборный доспех")); });
    loot_table.add_item(0.6, 5, [](float d) { return (Item*)(new Armor(19, "Латы")); });

    loot_table.add_item(0.6, 5, [](float d) { return (Item*)(new MagicArmor(12, 20, "Магическая броня")); });
    return loot_table;
}

LootTable<Creature*> get_creature_loot_table() {
    LootTable<Creature*> loot_table;
    loot_table.add_item(0.01, 10, [](float d) { return (Creature*)(OrkWarrior::generate(d)); });
    loot_table.add_item(0.01, 10, [](float d) { return (Creature*)(OrkArcher::generate(d)); });
    return loot_table;
}

Dungeon* generate_dungeon() {
    PlayerCharacter* player = PlayerCharacter::generate();
    int center = floor(DUNGEON_SIZE / 2.0);
    Dungeon* dungeon = new Dungeon(player);
    Room* starting_room = new Room(dungeon, make_pair(center, center));
    starting_room->spawn_creature(player);

    RoomGrid room_grid;
    for (int i = 0; i < DUNGEON_SIZE; i++) {
        vector<optional<Room*>> row;
        for (int j = 0; j < DUNGEON_SIZE; j++) {
            row.push_back(nullopt);
        }
        room_grid.push_back(row);
    }

    // Тут реализован алгоритм Прима, если чё
    room_grid[center][center] = starting_room;
    vector<pair<int, int>> saved_walls = {
        {center+1, center}, {center-1, center}, 
        {center, center+1}, {center, center-1}
    };
    while (!saved_walls.empty()) {
        int chosen_index = randint(0, saved_walls.size() - 1);
        pair<int, int> xy = saved_walls[chosen_index];
        saved_walls.erase(saved_walls.begin() + chosen_index);
        pair<int, int> dir = {0, 0};
        if (xy.first > 0 && room_grid[xy.first - 1][xy.second]) {
            if (dir != make_pair(0, 0)) continue;
            dir = {-1, 0};
        }
        if (xy.first < DUNGEON_SIZE-1 && room_grid[xy.first + 1][xy.second]) {
            if (dir != make_pair(0, 0)) continue;
            dir = {1, 0};
        }
        if (xy.second > 0 && room_grid[xy.first][xy.second - 1]) {
            if (dir != make_pair(0, 0)) continue;
            dir = {0, -1};
        }
        if (xy.second < DUNGEON_SIZE-1 && room_grid[xy.first][xy.second + 1]) {
            if (dir != make_pair(0, 0)) continue;
            dir = {0, 1};
        }
        room_grid[xy.first][xy.second] =
            new Room(dungeon, make_pair(xy.first, xy.second));
        pair<int, int> new_coord = {xy.first - dir.first, xy.second - dir.second};
        room_grid[new_coord.first][new_coord.second] =
            new Room(dungeon, make_pair(new_coord.first, new_coord.second));
        if (new_coord.first > 0 && !room_grid[new_coord.first-1][new_coord.second]) {
            saved_walls.push_back(make_pair(new_coord.first-1, new_coord.second));
        }
        if (new_coord.first < DUNGEON_SIZE-1 && !room_grid[new_coord.first+1][new_coord.second]) {
            saved_walls.push_back(make_pair(new_coord.first+1, new_coord.second));
        }
        if (new_coord.second > 0 && !room_grid[new_coord.first][new_coord.second - 1]) {
            saved_walls.push_back(make_pair(new_coord.first, new_coord.second - 1));
        }
        if (new_coord.second < DUNGEON_SIZE-1 && !room_grid[new_coord.first][new_coord.second + 1]) {
            saved_walls.push_back(make_pair(new_coord.first, new_coord.second + 1));
        }
    }
    dungeon->set_grid(room_grid);

    LootTable<Item*> item_loot_table = get_item_loot_table();
    LootTable<Creature*> creature_loot_table = get_creature_loot_table();

    for (int i = 0; i < DUNGEON_SIZE; i++) {
        for (int j = 0; j < DUNGEON_SIZE; j++) {
            if (room_grid[i][j] && !room_grid[i][j].value()->is_exit()) {
                Room* room = room_grid[i][j].value();
                pair<int, int> pos = room->get_pos();
                float danger = (abs(pos.first - center) + abs(pos.second - center)) / (float)(center+center);
                int n_of_items = round(MIN_ITEMS_IN_ROOM + (MAX_ITEMS_IN_ROOM-MIN_ITEMS_IN_ROOM)*danger);
                for (int k = 0; k < n_of_items; k++) {
                    optional<Item*> item = item_loot_table.gen_item(danger);
                    if (item) {
                        room->add_item(item.value());
                    }
                }
                int n_of_creatures = 
                    round(MIN_ENEMIES_IN_ROOM + (MAX_ENEMIES_IN_ROOM-MIN_ENEMIES_IN_ROOM)*danger);
                for (int k = 0; k < n_of_creatures; k++) {
                    optional<Creature*> creature = creature_loot_table.gen_item(danger);
                    if (creature)
                        room->spawn_creature(creature.value());
                }
            }
        }
    }

    for (int i = 0; i < DUNGEON_SIZE; i++) {
        for (int j = 0; j < DUNGEON_SIZE; j++) {
            if (room_grid[j][i]) {
                if (room_grid[j][i].value()->is_exit()) {
                    cout << "░";
                } else {
                    cout << "█";
                }
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }

    return dungeon;
}

Dungeon* generate_test_dungeon() {
    int health = randint(MIN_STARTING_HELATH, MAX_STARTING_HELATH);
    int str = randint(MIN_STARTING_STR, MAX_STARTING_STR);
    int dex = randint(MIN_STARTING_DEX, MAX_STARTING_DEX);
    vector<Item*> inventory;
    inventory.push_back(new Armor(14, "Кольчуга"));
    Weapon* weapon = new Weapon(3, "Кинжал");
    Armor* armor = new Armor(12, "Проклёпанная кожа");
    inventory.push_back(weapon);
    inventory.push_back(armor);
    inventory.push_back(HealingPotion::generate(1.0));
    inventory.push_back(DexterityPotion::generate(1.0));
    inventory.push_back(StrengthPotion::generate(1.0));
    PlayerCharacter *player = new PlayerCharacter(health, str, dex, inventory, weapon, armor);
    OrkWarrior* ork = OrkWarrior::generate(1.0);

    RoomGrid room_grid;

    Dungeon* dungeon = new Dungeon(player);
    Room* starting_room = new Room(dungeon, make_pair(1, 1));
    starting_room->add_item(new Weapon(8, "Длинный меч"));
    starting_room->spawn_creature(player);
    starting_room->spawn_creature(ork);

    return dungeon;
}

const char* CMD_HELP = R"(Список доступных команд:
  me - осмотреть самого себя
  room - осмотреть текущую комнату
  use [предмет] - использовать предмет из инвентаря
  take [предмет] - взять предмет из инвентаря комнаты
  attack [цель] - атаковать существо основным оружием
  go [r/t/l/b] - переместиться в соседнюю комнату
  help - получить помощь по командам
  quit - выйти из приложения)";

int main(int argc, char* argv[]) {
    if (argc > 1) {
        int seed = parse_integer(argv[1]);
        srand(seed);
    } else {
        srand(time(0));
    }

    Dungeon* dungeon = generate_dungeon();

    string cmd;
    while (true) {
        if (!dungeon->get_player()->is_alive()) {
            cout << "Ты умер. Кек." << endl;
            break;
        }
        if (dungeon->get_player()->get_room()->is_exit()) {
            cout << "Ты победил. Ура." << endl;
            break;
        }
        dungeon->clear_dead_creatures();
        cout << "> ";
        getline(cin, cmd);
        vector<string> split = split_string(cmd);
        if (split.size() == 0) {
            continue;
        }
        if (!split[0].compare("me") || !split[0].compare("m")) {
            dungeon->get_player()->inspect_self();
        } else if (!split[0].compare("use") || !split[0].compare("u")) {
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

            const vector<Item*>* inventory = dungeon->get_player()->get_inventory();
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
                const vector<Creature*>* creatures = dungeon->get_player()->get_current_room()->get_creatures();
                if (target_number > creatures->size() - 1) {
                    cout << "Нет цели с таким номером" << endl;
                    continue;
                }
            }

            UseItem action = { item_number, target_number };
            dungeon->get_player()->set_turn(action);
            dungeon->play_round();
        } else if (!split[0].compare("take") || !split[0].compare("t")) {
            vector<string> split = split_string(cmd);
            if (split.size() < 2) {
                cout << "Укажите номер предмета в комнате" << endl;
                continue;
            }
            int item_number;
            try {
                item_number = parse_integer(split[1], 1);
            } catch (std::invalid_argument _) {
                cout << "Неверный аргумент команды. Требуется натуральное число" << endl;
                continue;
            }

            const vector<Item*>* items = dungeon->get_player()->get_current_room()->get_items();
            if (item_number > items->size() || item_number < 1) {
                cout << "В комнате нет предмета с таким номером" << endl;
                continue;
            }

            TakeItem action = { item_number };
            dungeon->get_player()->set_turn(action);
            dungeon->play_round();
        } else if (!split[0].compare("attack") || !split[0].compare("a")) {
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
            const vector<Creature*>* creatures = dungeon->get_player()->get_current_room()->get_creatures();
            if (target_number > creatures->size() - 1) {
                cout << "Нет цели с таким номером" << endl;
                continue;
            }

            int i = 0;
            Creature* creature;
            int actual_count = 0;
            for (int i = 0; i < creatures->size(); i++) {
                if ((*creatures)[i] == dungeon->get_player()) continue;
                if (actual_count+1 == target_number) {
                    creature = (*creatures)[i];
                    break;
                }
                actual_count++;
            }

            AttackCreature action = { creature };
            dungeon->get_player()->set_turn(action);
            dungeon->play_round();
        } else if (!split[0].compare("go") || !split[0].compare("g")) {
            if (split.size() < 2) {
                cout << "Укажите направление, в котором хотите переместиться" << endl;
                continue;
            }
            pair<int, int> direction;
            if (!split[1].compare("l")) {
                direction = {-1, 0};
            } else if (!split[1].compare("t")) {
                direction = {0, -1};
            } else if (!split[1].compare("r")) {
                direction = {1, 0};
            } else if (!split[1].compare("b")) {
                direction = {0, 1};
            } else {
                cout << "Неверное направление" << endl;
                cout << "Направление можно указать как l, r, b или t:" << endl;
                cout << "это лево, право, низ и верх соответственно." << endl;
                continue;
            }
            Creature* player = dungeon->get_player();
            Room* departure_room = player->get_room();
            pair<int, int> departure = departure_room->get_pos();
            pair<int, int> dest = {departure.first + direction.first, departure.second + direction.second};
            optional<Room*> dest_room = dungeon->get_room(dest);
            if (!dest_room) {
                cout << "В эту сторону нет прохода" << endl;
                continue;
            }
            if (departure_room->get_creatures()->size() > 1 && dest_room.value()->get_creatures()->size() > 0) {
                cout << "Переход загорожен монстрами. Туда не пройти" << endl;
                continue;
            }
            departure_room->remove_creature(player);
            dest_room.value()->spawn_creature(player);
            cout << "Ты перешёл в соседнюю комнату." << endl;
        } else if (!split[0].compare("room") || !split[0].compare("r")) {
            dungeon->get_player()->inspect_room();
        } else if (!split[0].compare("quit") || !split[0].compare("q")) {
            break;
        } else if (!split[0].compare("help") || !split[0].compare("h")) {
            cout << CMD_HELP << endl;
        } else {
            cout << "Неизвестная команда: " << cmd << 
                ". Используйте команду \"help\", чтобы получить помощь" << endl;
        }
    }

    return 0;
}
