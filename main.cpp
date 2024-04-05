#include <iostream>
#include <utility>
#include <concepts>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

class PhysicalItem;

/*
 * Abstract class for creating characters
 * @param healthPoint - Health
 * @param name - Character name
 */
class Character
{
    friend PhysicalItem;
protected:
    int healthPoints;
    const string name;

    virtual string toString() const
    {
    }

public:
    Character(string &n, int HP) : name(n), healthPoints(HP) {}

    virtual ~Character() = default;

    /*
     * Method for making characters speak
     */
    void speak(const string &speech)
    {
        cout << name << ": " << speech << endl;
    }

    //Getter
    virtual string getName()
    {
        return name;
    }

    //Getter
    virtual int getHP()
    {
        return healthPoints;
    }

    /*
     * Method for getting item into the inventory
     */
    virtual void obtainItem(const shared_ptr<PhysicalItem> &item)
    {

    }

    /*
     * << operator overrule
     */
    friend ostream &operator<<(ostream &os, const Character &character)
    {
        os << character.toString();
        return os;
    }

private:
    /*
     * Method for taking damage
     */
    virtual void takeDamage(int damage)
    {
        healthPoints -= damage;
    }

    /*
     * Method for healing
     */
    virtual void heal(int healValue)
    {
        healthPoints += healValue;
    }
};

/*
 * Base abstract class for physical items
 * @param owner - Owner of the item
 * @param name - Name of the item
 */
class PhysicalItem
{
protected:
    Character owner;
    string name;

public:
    PhysicalItem(string &n, Character &character) : owner(character), name(n) {}

    ~PhysicalItem() = default;

    friend ostream &operator<<(ostream &os, PhysicalItem &physicalItem)
    {
        os << physicalItem.toString() << " ";
        return os;
    }

    string getName()
    {
        return name;
    }

protected:
    /*
     * Method for giving damage to another player
     */
    void giveDamageTo(Character &to, int damage)
    {
        to.takeDamage(damage);
    }

    /*
     * Method to heal player
     */
    void giveHealTo(Character &to, int heal)
    {
        to.heal(heal);
    }

    /*
     * Method for using the logic of the item
     */
    virtual void useLogic(Character &target)
    {

    }

    virtual string toString()
    {
    }

};

/*
 * Child class of a physical item
 * @param damage - amount of damage that will be dealt with this weapon
 * @see PhysicalItem
 */
class Weapon : public PhysicalItem
{
private:
    int damage;
public:
    Weapon(string &n, Character &character, int damage) : PhysicalItem(n, character), damage(damage)
    {
        cout << owner.getName() << " just obtained a new weapon called " << name << "." << endl;
    }

    ~Weapon() = default;

    void useLogic(Character &target) override
    {
        giveDamageTo(target, damage);
    }

protected:
    string toString() override
    {
        return name + ":" + to_string(damage);
    }


};

/*
 * Child class of a physical item
 * @param healValue - amount of HP given to a character
 * @see PhysicalItem
 */
class Potion : public PhysicalItem
{
private:
    int healValue;
public:
    Potion(string &n, Character &owner, int healValue) : PhysicalItem(n, owner), healValue(healValue)
    {
        cout << owner.getName() << " just obtained a new potion called " << name << "." << endl;
    }

    ~Potion() = default;

    void useLogic(Character &target) override
    {
        giveHealTo(target, healValue);
    }

protected:
    string toString() override
    {
        return name + ":" + to_string(healValue);
    }
};

/*
 * Child class of a physical item
 * @param allowedTargets - map of characters that can be attacked with a spell
 * @see PhysicalItem
 */
class Spell : public PhysicalItem
{
private:
    map<string, shared_ptr<Character>> allowedTargets;
public:
    Spell(string &n, Character &owner, map<string, shared_ptr<Character>> &targets) : PhysicalItem(n, owner),
                                                                                      allowedTargets(targets)
    {
        cout << owner.getName() << " just obtained a new spell called " << name << "." << endl;
    }

    ~Spell() = default;

    void useLogic(Character &target) override
    {
        auto it = allowedTargets.find(target.getName());
        if (it != allowedTargets.end() && it->second) {
            giveDamageTo(target, target.getHP());
        } else {
            cout << "Error caught" << endl;
        }
    }

    /*
     * Checks if target character is in allowedTargets list
     * @see allowedTargets
     */
    bool isTargetInList(Character &target)
    {
        auto it = allowedTargets.find(target.getName());
        return it != allowedTargets.end() && it->second;
    }

protected:
    string toString() override
    {
        return name + ":" + to_string(allowedTargets.size());
    }

};

/*
 * Definition of a Container class
 */
template<typename T>
class Container
{
protected:
    vector<T> elems;
public:
    Container() = default;

    ~Container() = default;
};

template<typename T>
concept DerivedFromPhysicalItem = is_base_of<PhysicalItem, T>::value;

/*
 * Declaration of a container class with template type T, being all items derived from PhysicalItem
 * @param elements - map of items in the inventory
 * @param maxCapacity - max size
 * @see PhysicalItem
 */
template<DerivedFromPhysicalItem  T>
class Container<T>
{
private:
    map<string, shared_ptr<T>> elements;
    int maxCapacity;
public:


    Container(int size)
    {
        elements = std::move(map<string, shared_ptr<T>>());
        maxCapacity = size;
    }

    ~Container()
    {
        elements.clear();
    }

    bool isFull()
    {
        return this->elements.size() == maxCapacity;
    }

    /*
     * Adds item into the container
     */
    void addItem(shared_ptr<T> newItem)
    {
        elements.insert({newItem->getName(), std::move(newItem)});
    }

    /*
     * Gets a pointer to an item from the container
     */
    shared_ptr<T> &getItem(string &item)
    {
        return elements.at(item);
    }

    /*
     * Checks if item is in the container
     */
    bool find(string &item)
    {
        return elements.contains(item);
    }

    /*
     * Gives a map of all items in the container
     */
    map<string, shared_ptr<T>> &toShow()
    {
        return elements;
    }

    /*
     * Removes item from the container
     */
    void removeItem(string &itemName)
    {
        elements.erase(itemName);
    }

    /*
     * Changes the size of the container
     */
    void resizeContainer(int size)
    {
        maxCapacity = size;
    }


};

/*
 * Arsenal type, for Container<Weapons>
 */
typedef Container<Weapon> Arsenal;
/*
 * MedicalBag type, for Container<Potion>
 */
typedef Container<Potion> MedicalBag;
/*
 * SpellBook type, for Container<Spell>
 */
typedef Container<Spell> SpellBook;

/*
 * Child class of a character
 * Allows to store and use weapons
 * @param arsenal - container for weapon type items
 * @see Character
 * @see Container
 * @see Arsenal
 * @see Weapon
 */
class WeaponUser : virtual public Character
{
protected:
    Arsenal arsenal;
public:
    WeaponUser(string &n, int HP) : Character(n, HP), arsenal(0)
    {

    }

    ~WeaponUser() = default;

    bool isFull()
    {
        return arsenal.isFull();
    }

    /*
     * Method for showing weapons
     */
    void showWeapons()
    {
        for (const auto &weapon: arsenal.toShow()) {
            cout << *weapon.second << " ";
        }
        cout << endl;
    }

    /*
     * Method for attacking another player
     */
    void attack(Character &target, string weaponName)
    {
        if (arsenal.find(weaponName)) {
            arsenal.getItem(weaponName)->useLogic(target);
            cout << name << " attacks " << target.getName() << " with their " << weaponName << "!" << endl;
        } else {
            cout << "Error caught" << endl;
        }

    }
};

/*
 * Child class of a character
 * Allows to store and use potions
 * @param medicalBag - container for potion type items
 * @see Character
 * @see Container
 * @see MedicalBag
 * @see Potion
 */
class PotionUser : virtual public Character
{
protected:
    MedicalBag medicalBag;
public:
    PotionUser(string &n, int HP) : Character(n, HP), medicalBag(0) {}

    ~PotionUser() = default;

    bool isFull()
    {
        return medicalBag.isFull();
    };

    /*
     * Method for showing potions
     */
    void showPotions()
    {
        for (const auto &potion: medicalBag.toShow()) {
            cout << *potion.second << " ";
        }
        cout << endl;
    }

    /*
     * Method for drinking potions
     */
    void drink(Character &target, string potionName)
    {
        if (medicalBag.find(potionName)) {
            medicalBag.getItem(potionName)->useLogic(target);
            cout << target.getName() << " drinks " << potionName << " from " << name << "." << endl;
            medicalBag.removeItem(potionName);
        } else {
            cout << "Error caught" << endl;
        }

    }

};

/*
 * Child class of a character
 * Allows to store and use spells
 * @param spellBook - container for spell type items
 * @see Character
 * @see Container
 * @see SpellBook
 * @see Spell
 */
class SpellUser : virtual public Character
{
protected:
    SpellBook spellBook;
public:
    SpellUser(string &n, int HP) : Character(n, HP), spellBook(0) {}

    ~SpellUser()
    {
    }

    bool isFull()
    {
        return spellBook.isFull();
    }

    /*
     * Method for showing spells
     */
    void showSpells()
    {
        for (const auto &spell: spellBook.toShow()) {
            cout << *spell.second << " ";
        }
        cout << endl;
    }

    /*
     * Method for casting spells on characters
     */
    void cast(Character &target, string spellName)
    {
        if (spellBook.find(spellName)) {
            if (spellBook.getItem(spellName)->isTargetInList(target)) {
                spellBook.getItem(spellName)->useLogic(target);
                cout << name << " casts " << spellName << " on " << target.getName() << "!" << endl;
                spellBook.removeItem(spellName);
            } else {
                cout << "Error caught" << endl;
            }
        } else {
            cout << "Error caught" << endl;
        }
    }

};

/*
 * Child class of a WeaponUser and PotionUser
 * @param maxAllowedWeapons - size of arsenal
 * @param maxAllowedPotions - size of medicalBag
 */
class Fighter : public WeaponUser, public PotionUser
{
public:
    const int maxAllowedWeapons = 3;
    const int maxAllowedPotions = 5;

    Fighter(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP)
    {
        cout << "A new fighter came to town, " << name << "." << endl;
        arsenal.resizeContainer(maxAllowedWeapons);
        medicalBag.resizeContainer(maxAllowedPotions);
    }

    ~Fighter() = default;

    void obtainItem(const shared_ptr<PhysicalItem> &item) override
    {
        if (auto potion = dynamic_pointer_cast<Potion>(item)) {
            medicalBag.addItem(potion);
        } else if (auto weapon = dynamic_pointer_cast<Weapon>(item)) {
            arsenal.addItem(weapon);
        }
    }

private:
    string toString() const override
    {
        return name + ":fighter:" + to_string(healthPoints);
    }


};

/*
 * Child class of a WeaponUser, PotionUser and SpellUser
 * @param maxAllowedWeapons - size of arsenal
 * @param maxAllowedPotions - size of medicalBag
 * @param maxAllowedSpells - size of spellBook
 */
class Archer : public WeaponUser, public PotionUser, public SpellUser
{
public:
    const int maxAllowedWeapons = 2;
    const int maxAllowedPotions = 3;
    const int maxAllowedSpells = 2;

    Archer(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP), SpellUser(n, HP)
    {
        cout << "A new archer came to town, " << name << "." << endl;
        arsenal.resizeContainer(maxAllowedWeapons);
        medicalBag.resizeContainer(maxAllowedPotions);
        spellBook.resizeContainer(maxAllowedSpells);
    }

    ~Archer() = default;

    void obtainItem(const shared_ptr<PhysicalItem> &item) override
    {
        if (auto potion = dynamic_pointer_cast<Potion>(item)) {
            medicalBag.addItem(potion);
        } else if (auto weapon = dynamic_pointer_cast<Weapon>(item)) {
            arsenal.addItem(weapon);
        } else if (auto spell = dynamic_pointer_cast<Spell>(item)) {
            spellBook.addItem(spell);
        }
    }

private:
    string toString() const override
    {
        return name + ":archer:" + to_string(healthPoints);
    }

};

/*
 * Child class of a PotionUser and SpellUser
 * @param maxAllowedPotions - size of medicalBag
 * @param maxAllowedSpells - size of spellBook
 */
class Wizard : public PotionUser, public SpellUser
{
public:
    const int maxAllowedPotions = 10;
    const int maxAllowedSpells = 10;

    Wizard(string &n, int HP) : Character(n, HP), PotionUser(n, HP), SpellUser(n, HP)
    {
        cout << "A new wizard came to town, " << name << "." << endl;
        medicalBag.resizeContainer(maxAllowedPotions);
        spellBook.resizeContainer(maxAllowedSpells);
    }

    ~Wizard() = default;

    void obtainItem(const shared_ptr<PhysicalItem> &item) override
    {
        if (auto potion = dynamic_pointer_cast<Potion>(item)) {
            medicalBag.addItem(potion);
        } else if (auto spell = dynamic_pointer_cast<Spell>(item)) {
            spellBook.addItem(spell);
        }
    }

private:
    string toString() const override
    {
        return name + ":wizard:" + to_string(healthPoints);
    }

};


int main()
{
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    /*
     * A map of Characters
     */
    map<string, shared_ptr<Character>> characters;
    //Creation of character narrator
    string narratorName = "Narrator";
    Character narrator(narratorName, 0);
    int n;
    cin >> n;
    string command;
    for (int i = 0; i <= n; i++) {
        getline(cin, command);
        if (command.empty()) {
            continue;
        }
        vector<string> words;
        istringstream iss(command);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        //Case creation of something
        if (words[0] == "Create") {
            //Creation of a character
            if (words[1] == "character") {
                string type = words[2];
                string name = words[3];
                int initHP = stoi(words[4]);
                if (type == "fighter") {
                    characters[name] = make_shared<Fighter>(name, initHP);
                } else if (type == "wizard") {
                    characters[name] = make_shared<Wizard>(name, initHP);
                } else if (type == "archer") {
                    characters[name] = make_shared<Archer>(name, initHP);
                }
                continue;
                //Creation of an item
            } else if (words[1] == "item") {
                //Creation of a potion
                if (words[2] == "potion") {
                    string ownerName = words[3];
                    string potionName = words[4];
                    int healValue = stoi(words[5]);
                    if (healValue <= 0) {
                        cout << "Error caught" << endl;
                        continue;
                    }
                    auto it = characters.find(ownerName);
                    if (it != characters.end() && it->second) {
                        auto character = it->second;
                        if (auto potionUser = dynamic_cast<PotionUser *>(character.get())) {
                            if (!potionUser->isFull()) {
                                shared_ptr<Potion> potion = make_shared<Potion>(potionName, *characters[ownerName],
                                                                                healValue);
                                characters[ownerName]->obtainItem(potion);
                            } else {
                                cout << "Error caught" << endl;
                            }
                        } else {
                            cout << "Error caught" << endl;
                        }

                    } else {
                        cout << "Error caught" << endl;
                    }
                    //Creation of a weapon
                } else if (words[2] == "weapon") {
                    string ownerName = words[3];
                    string weaponName = words[4];
                    int damage = stoi(words[5]);
                    if (damage <= 0) {
                        cout << "Error caught" << endl;
                        continue;
                    }
                    auto it = characters.find(ownerName);
                    if (it != characters.end() && it->second) {
                        auto character = it->second;
                        if (auto weaponUser = dynamic_cast<WeaponUser *>(character.get())) {
                            if (!weaponUser->isFull()) {
                                shared_ptr<Weapon> weapon = make_shared<Weapon>(weaponName, *characters[ownerName],
                                                                                damage);
                                characters[ownerName]->obtainItem(weapon);
                            } else {
                                cout << "Error caught" << endl;
                            }
                        } else {
                            cout << "Error caught" << endl;
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                    //Creation of a spell
                } else if (words[2] == "spell") {
                    string ownerName = words[3];
                    string spellName = words[4];
                    map<string, shared_ptr<Character>> targets;
                    auto it = characters.find(ownerName);
                    if (it != characters.end() && it->second) {
                        auto character = it->second;
                        if (auto spellUser = dynamic_cast<SpellUser *>(character.get())) {
                            if (!spellUser->isFull()) {
                                bool flag = true;
                                for (int j = 6; j < words.size(); ++j) {
                                    if (characters.find(words[j]) == characters.end()) {
                                        cout << "Error caught" << endl;
                                        flag = false;
                                        break;
                                    } else {
                                        targets[characters.at(words[j])->getName()] = characters[words[j]];
                                    }
                                }
                                if (flag) {
                                    shared_ptr<Spell> spell = make_shared<Spell>(spellName, *characters[ownerName],
                                                                                 targets);
                                    characters[ownerName]->obtainItem(spell);
                                }
                            } else {
                                cout << "Error caught" << endl;
                            }
                        } else {
                            cout << "Error caught" << endl;
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                }
            }
            //Case of showing something
        } else if (words[0] == "Show") {
            //Showing characters
            if (words[1] == "characters") {
                for (const auto &character: characters) {
                    cout << *character.second << " ";
                }
                cout << endl;
                continue;
            } else if (words[1] == "potions") {
                string characterName = words[2];
                auto it = characters.find(characterName);
                if (it != characters.end() && it->second) {
                    auto character = it->second;
                    if (auto potionUser = dynamic_cast<PotionUser *>(character.get())) {
                        potionUser->showPotions();
                    } else {
                        std::cout << "Error caught" << std::endl;
                    }
                } else {
                    std::cout << "Error caught" << std::endl;
                }
                //Showing weapons of a specific character
            } else if (words[1] == "weapons") {
                string characterName = words[2];
                auto it = characters.find(characterName);
                if (it != characters.end() && it->second) {
                    auto character = it->second;
                    if (auto weaponUser = dynamic_cast<WeaponUser *>(character.get())) {
                        weaponUser->showWeapons();
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
                //Showing spells of a specific character
            } else if (words[1] == "spells") {
                string characterName = words[2];
                auto it = characters.find(characterName);
                if (it != characters.end() && it->second) {
                    auto character = it->second;
                    if (auto spellUser = dynamic_cast<SpellUser *>(character.get())) {
                        spellUser->showSpells();
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            }
            //Case for initiating a dialogue
        } else if (words[0] == "Dialogue") {
            int numberOfWords = stoi(words[2]);
            //Case if narrator is speaking
            if (words[1] == "Narrator") {
                string speech;
                for (int j = 3; j < numberOfWords + 3; ++j) {
                    speech.append(words[j] + " ");
                }
                narrator.speak(speech);
            } else {
                auto it = characters.find(words[1]);
                if (it != characters.end() && it->second) {
                    string speech;
                    for (int j = 3; j < numberOfWords + 3; ++j) {
                        speech.append(words[j] + " ");
                    }
                    characters[words[1]]->speak(speech);
                } else {
                    cout << "Error caught" << endl;
                }
            }
            //Case for drinking a potion
        } else if (words[0] == "Drink") {
            string supplierName = words[1];
            string drinkerName = words[2];
            string potionName = words[3];
            auto it = characters.find(supplierName);
            if (it != characters.end() && it->second) {
                auto supplier = it->second;
                it = characters.find(drinkerName);
                if (it != characters.end() && it->second) {
                    auto drinker = it->second;
                    auto sup = dynamic_cast<PotionUser *>(supplier.get());
                    auto dri = dynamic_cast<PotionUser *>(drinker.get());
                    sup->drink(*dri, potionName);
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
            //Case for attacking
        } else if (words[0] == "Attack") {
            string attackerName = words[1];
            string targetName = words[2];
            string weaponName = words[3];
            auto it = characters.find(attackerName);
            if (it != characters.end() && it->second) {
                auto attacker = it->second;
                it = characters.find(targetName);
                if (it != characters.end() && it->second) {
                    auto target = it->second;
                    if (auto att = dynamic_cast<WeaponUser *>(attacker.get())) {
                        att->attack(*target, weaponName);
                        if (target->getHP() <= 0) {
                            cout << target->getName() << " has died..." << endl;
                            characters.erase(target->getName());
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
            //Case for casting a spell
        } else if (words[0] == "Cast") {
            string casterName = words[1];
            string targetName = words[2];
            string spellName = words[3];
            auto it = characters.find(casterName);
            if (it != characters.end() && it->second) {
                auto attacker = it->second;
                it = characters.find(targetName);
                if (it != characters.end() && it->second) {
                    auto target = it->second;
                    if (auto att = dynamic_cast<SpellUser *>(attacker.get())) {
                        att->cast(*target, spellName);
                        if (target->getHP() <= 0) {
                            cout << target->getName() << " has died..." << endl;
                            characters.erase(target->getName());
                        }
                    } else {
                        cout << "Error caught" << endl;
                    }
                } else {
                    cout << "Error caught" << endl;
                }
            } else {
                cout << "Error caught" << endl;
            }
        }
    }
    return 0;
}