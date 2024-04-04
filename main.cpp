#include <iostream>
#include <utility>
#include <concepts>
#include <map>
#include <memory>
#include <vector>
#include <string>
#include <sstream>


using namespace std;

class Character {
protected:
    int healthPoints;
    const string name;

public:
    Character(string &n, int HP) : name(n), healthPoints(HP){}
    virtual ~Character() = default;



    virtual string getName(){
        return name;
    }

    virtual int getHP(){
        return healthPoints;
    }

private:
    virtual void takeDamage(int damage){
        healthPoints -= damage;
    }
    virtual void heal(int healValue) {
        healthPoints += healValue;
    }




};

class PhysicalItem {
    friend Character;
protected:
    bool isUsableOnce;
    Character owner;
    string name;
public:
    PhysicalItem(string &n, Character &character, bool isUsableOnce) : owner(character), name(n), isUsableOnce(isUsableOnce){}

};

class Weapon : public PhysicalItem {
private:
    int damage;
public:
    Weapon(string &n, Character &character, int damage) : PhysicalItem(n, character, false), damage(damage)
    {

    }
};

class Potion : public PhysicalItem {
private:
    int healValue;
public:
    Potion(string &name, Character &owner, int healValue) : PhysicalItem(name, owner, true), healValue(healValue)
    {

    }
};

class Spell : PhysicalItem {
private:
    vector<Character> allowedTargets;
public:
    Spell(string &name, Character &owner) : PhysicalItem(name, owner, true)
    {

    }

};

template<typename  T>
class Container{
protected:
    vector<T> elems;
public:
    Container()= default;
    ~Container()= default;
};
template<typename T>
concept DerivedFromPhysicalItem = is_base_of<PhysicalItem, T>::value;


template<DerivedFromPhysicalItem  T>
class Container<T>{
private:
    map<string, unique_ptr<T>> elements;
    int maxCapacity;
public:


    Container(int size){
        elements = std::move(map<string, unique_ptr<T>>());
        maxCapacity = size;
    }

    bool isFull(){
        return this->elements.size() == maxCapacity;
    }

    void addItem(unique_ptr<T> newItem){

        elements.insert({newItem->getName(), std::move(newItem)});
    }

    unique_ptr<T>& getItem(string & item){
        cout << item<<"________________________-"<<endl;
        return elements.at(item);


    }

    bool find(string & item){
        cout << elements.contains(item);
        return elements.contains(item);
    }

    map<string, unique_ptr<T>>& toShow(){
        return elements;
    }

    void removeItem(string & itemName){
        elements.erase(itemName);
    }
    void resizeContainer(int size){
        maxCapacity = size;
    }


};

typedef Container<Weapon> Arsenal;
typedef Container<Potion> MedicalBag;
typedef Container<Spell> SpellBook;


class WeaponUser : virtual public Character{
protected:
    Arsenal arsenal;
public:
    WeaponUser(string &n, int HP) : Character(n, HP), arsenal(0){

    }
};

class PotionUser : virtual public Character{
protected:
    MedicalBag medicalBag;
public:
    PotionUser(string &n, int HP) : Character(n, HP), medicalBag(0){}

};

class SpellUser : virtual public Character{
protected:
    SpellBook spellBook;
public:
    SpellUser(string &n, int HP) : Character(n, HP), spellBook(0){}

};

class Fighter : public WeaponUser, public PotionUser{
public:
    const int maxAllowedWeapons = 3;
    const int maxAllowedPotions = 5;
    Fighter(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP){
        cout << "A new fighter came to town, " << name << endl;
        arsenal.resizeContainer(maxAllowedWeapons);
        medicalBag.resizeContainer(maxAllowedPotions);
    }




};

class Archer : public WeaponUser, public PotionUser, public SpellUser{
public:
    const int maxAllowedWeapons = 2;
    const int maxAllowedPotions = 3;
    const int maxAllowedSpells = 2;
    Archer(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP), SpellUser(n, HP){
        cout << "A new archer came to town, " << name << endl;
        arsenal.resizeContainer(maxAllowedWeapons);
        medicalBag.resizeContainer(maxAllowedPotions);
        spellBook.resizeContainer(maxAllowedSpells);
    }

};

class Wizard : public PotionUser, public SpellUser{
public:
    const int maxAllowedPotions = 10;
    const int maxAllowedSpells = 10;
    Wizard(string &n, int HP) : Character(n, HP), PotionUser(n, HP), SpellUser(n, HP){
        cout << "A new wizard came to town, " << name << endl;
        medicalBag.resizeContainer(maxAllowedPotions);
        spellBook.resizeContainer(maxAllowedSpells);
    }

};



class Narrator {
private:
    string name = "Narrator";
public:
    Narrator() = default;
};

int main(){
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    map<string, Character> characters;
    int n;
    cin >> n;
    string command;
    for (int i = 0; i <= n; i++){
        getline(cin, command);
        if (command.empty()){
            continue;
        }
        vector<string> words;
        istringstream iss(command);
        string word;
        while (iss >> word) {
            words.push_back(word);
        }
        if (words[0] == "Create") {
            if (words[1] == "character"){
                string type = words[2];
                string name = words[3];
                int initHP = stoi(words[4]);
                if (type == "fighter"){
                    characters.insert(pair <string, Character> (name, Fighter(name, initHP)));
                } else if (type == "wizard") {
                    characters.insert(pair <string, Character> (name, Wizard(name, initHP)));
                } else if (type == "archer") {
                    characters.insert(pair <string, Character> (name, Archer(name, initHP)));
                }

            }
        }
    }
    return 0;
}