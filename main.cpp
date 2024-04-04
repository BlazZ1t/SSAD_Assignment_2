#include <iostream>
#include <utility>
#include <concepts>
#include <map>
#include <memory>
#include <vector>


using namespace std;

class Character {
protected:
    const int healthPoints;
    const string name;

public:
    Character(string &n, int HP) : name(n), healthPoints(HP){}
    virtual ~Character() = default;

    friend ostream& operator<<(ostream& os, const Character& character){

    }

    string getName(){
        return name;
    }

    int getHP(){
        return healthPoints;
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
public:
    Weapon(string &n, Character &character) : PhysicalItem(n, character, false)
    {

    }
};

class Potion : public PhysicalItem {
public:
    Potion(string &name, Character &owner) : PhysicalItem(name, owner, true)
    {

    }
};

class Spell : PhysicalItem {
    Spell(string &name, Character &owner) : PhysicalItem(name, owner, true)
    {

    }

};

template<typename  T>
class Container{
protected:
    vector<T> elems;
public:
    Container(){
        cout << "Base container created"<<endl;
    }
    ~Container(){
        cout << "Base container destroyed" << endl;
    }
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
        cout << "Container max capa created" << endl;
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


};


class WeaponUser : virtual public Character{
public:
    WeaponUser(string &n, int HP) : Character(n, HP){

    }
};

class PotionUser : virtual public Character{
public:
    PotionUser(string &n, int HP) : Character(n, HP){}

};

class SpellUser : virtual public Character{
public:
    SpellUser(string &n, int HP) : Character(n, HP){}

};

class Fighter : public WeaponUser, public PotionUser{
public:
    const int maxAllowedWeapons = 3;
    const int maxAllowedPotions = 5;
    Fighter(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP){
        cout << "A new fighter came to town, " << name << endl;
    }


};

class Archer : public WeaponUser, public PotionUser, public SpellUser{
public:
    const int maxAllowedWeapons = 2;
    const int maxAllowedPotions = 3;
    const int maxAllowedSpells = 2;
    Archer(string &n, int HP) : Character(n, HP), PotionUser(n, HP), WeaponUser(n, HP), SpellUser(n, HP){
        cout << "A new archer came to town, " << name << endl;
    }

};

class Wizard : public PotionUser, public SpellUser{
public:
    const int maxAllowedPotions = 10;
    const int maxAllowedSpells = 10;
    Wizard(string &n, int HP) : Character(n, HP), PotionUser(n, HP), SpellUser(n, HP){
        cout << "A new wizard came to town, " << name << endl;
    }

};

class Narrator {
private:
    string name;
};

int main(){

    return 0;
}