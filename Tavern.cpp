/*
CSCI235 Fall 2023
Project 3 - Tavern Class
Georgina Woo
June 20 2023
Modified by Allison Lee on October 22, 2023
Tavern.cpp declares the Tavern class along with its private and public members
*/
#include "Tavern.hpp"
#include "Mage.hpp"
#include "Scoundrel.hpp"
#include "Ranger.hpp"
#include "Barbarian.hpp"
#include <fstream>

/** Default Constructor **/
Tavern::Tavern() : ArrayBag<Character*>(), level_sum_{0}, num_enemies_{0}
{
}

/**
  @param: the name of an input file
  @pre: Formatting of the csv file is as follows (each numbered item appears separated by 
        comma, only one value for each numbered item):
      1. Name: An uppercase string
      2. Race: An uppercase string [HUMAN, ELF, DWARF, LIZARD, UNDEAD]
      3. Subclass: An uppercase string [BARBARIAN, MAGE, SCOUNDREL, RANGER]
      4. Level/Vitality/Armor: A positive integer
      5. Enemy: 0 (False) or 1 (True)
      6. Main: Uppercase string or strings representing the main weapon (Barbarian and Mage), 
        Dagger type (Scoundrel), or arrows (Ranger). A ranger's arrows are of the form [TYPE] 
        [QUANTITY];[TYPE] [QUANTITY], where each arrow type is separated by a semicolon, and 
        the type and its quantity are separated with a space.
      7. Offhand: An uppercase string that is only applicable to Barbarians, and may be NONE 
        if the Barbarian does not have an offhand weapon, or if the character is of a different 
        subclass.
      8. School/Faction: Uppercase strings that represent a Mage's school of magic: [ELEMENTAL, 
        NECROMANCY, ILLUSION] or a Scoundrel's faction: [CUTPURSE, SHADOWBLADE, SILVERTONGUE], 
        and NONE where not applicable
      9. Summoning: 0 (False) or 1 (True), only applicable to Mages (summoning an Incarnate) and 
        Rangers (Having an Animal Companion)
      10. Affinity: Only applicable to Rangers. Affinities are of the form [AFFINITY1];[AFFINITY2] 
        where multiple affinities are separated by a semicolon. Th value may be NONE for a Ranger 
        with no affinities, or characters of other subclasses.
      11. Disguise: 0 (False) or 1 (True), only applicable to Scoundrels, representing if they have 
        a disguise.
      12. Enraged: 0 (False) or 1 (True), only applicable to Barbarians, representing if they are 
        enraged.
  @post: Each line of the input file corresponds to a Character subclass and dynamically allocates 
        Character derived objects, adding them to the Tavern.
*/
Tavern::Tavern(const std::string& filename) : Tavern(){
  std::ifstream file(filename);
  if (file.fail()) {
      std::cerr << "File cannot be opened for reading." << std::endl;
      exit(1);
  }
  std::string line, junk, NAME, RACE, SUBCLASS, MAIN, OFFHAND, SCHOOL_FACTION, AFFINITIY;
  std::string LEVEL, VITALITY, ARMOR;
  std::string ENEMY, SUMMONING, DISGUISE, ENRAGED;
  while(getline(file, line)){
    getline(file, NAME, ',');
    getline(file, RACE, ',');
    getline(file, SUBCLASS, ',');
    getline(file, LEVEL, ',');
    getline(file, VITALITY, ',');
    getline(file, ARMOR, ',');
    getline(file, ENEMY, ',');
    getline(file, MAIN, ',');
    getline(file, OFFHAND, ',');
    getline(file, SCHOOL_FACTION, ',');
    getline(file, SUMMONING, ',');
    getline(file, AFFINITIY, ',');
    getline(file, DISGUISE, ',');
    file >> ENRAGED;
    int level = stoi(LEVEL);
    int vitality = stoi(VITALITY);
    int armor = stoi(ARMOR);
    bool enemy = false;
    if (ENEMY == "1"){enemy = true;}
    bool summoning = false;
    if (SUMMONING == "1"){summoning = true;}
    bool disguise = false;
    if (DISGUISE == "1"){disguise = true;}
    bool enraged = false;
    if (ENRAGED == "1"){enraged = true;}
    // std::cout << NAME << " " << RACE << " " << SUBCLASS << " " << level << " " << vitality << " " << armor << 
    // " " << enemy << " " << MAIN << " " << OFFHAND << " " << SCHOOL_FACTION << " " << summoning << " " << AFFINITIY 
    // << " " << disguise << " " << enraged << std::endl;
    if (SUBCLASS == "MAGE"){
      Character* mage = new Mage(NAME, RACE, vitality, armor, level, enemy, SCHOOL_FACTION, MAIN, summoning);
      enterTavern(mage);
    }
    else if (SUBCLASS == "SCOUNDREL"){
      Character* scoundrel = new Scoundrel(NAME, RACE, vitality, armor, level, enemy, 
      MAIN, SCHOOL_FACTION, disguise);
      enterTavern(scoundrel);
    }
    else if (SUBCLASS == "RANGER"){
      Character* ranger = new Ranger(NAME, RACE, vitality, armor, level, enemy, formatMain(MAIN), formatAffinities(AFFINITIY), summoning);
      enterTavern(ranger);
    }
    else if (SUBCLASS == "BARBARIAN"){
      Character* barbarian = new Barbarian(NAME, RACE, vitality, armor, level, enemy, 
        MAIN, OFFHAND, enraged);
      enterTavern(barbarian);
    }
  }
}

/** 
    @param:   A reference to a Character entering the Tavern
    @return:  returns true if a Character was successfully added to items_, false otherwise
    @post:    adds Character to the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::enterTavern(Character* const& a_character)
{
  if(add(a_character))
  {
    level_sum_ += a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_++;
     
    return true;
  }
  else
  {
    return false;
  }
}

/** @param:   A reference to a Character leaving the Tavern  
    @return:  returns true if a character was successfully removed from items_, false otherwise
    @post:    removes the character from the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::exitTavern(Character* const& a_character)
{
  if(remove(a_character))
  {
    level_sum_ -= a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_--;
      
    return true;
  }
  return false;
}



/** 
  @return:  The integer level count of all the characters currently in the Tavern
**/
int Tavern::getLevelSum()
{
  return level_sum_;
}



/** 
    @return:  The average level of all the characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the average level of the Tavern rounded to the NEAREST integer, and returns the integer value.
**/
int Tavern::calculateAvgLevel()
{
  return (level_sum_>0) ? round(double(level_sum_) / item_count_) : 0.0;
}

/** 
    @return:  The integer enemy count of the Tavern
    **/
    int Tavern::getEnemyCount()
    {
      return num_enemies_;
    }

/** 
    @return:  The percentage (double) of all the enemy characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the enemy percentage of the Tavern rounded to 2 decimal places, and returns the double value.
**/
double Tavern::calculateEnemyPercentage()
{
  double enemy_percent = (num_enemies_>0) ?  (double(num_enemies_) / item_count_) * 100: 0.0;
  return std::ceil(enemy_percent*100.0) / 100.0; //round up to to decimal places
 
}


/** 
    @param:   A string reference to a race 
    @return:  An integer tally of the number of characters in the Tavern of the given race
**/
int Tavern::tallyRace(const std::string &race)
{
  int frequency = 0;
  int curr_index = 0;   
  while (curr_index < item_count_)
  {
    if (items_[curr_index]->getRace() == race)
    {
      frequency++;
    } 

    curr_index++; 
  }

  return frequency;
}


/**
  @post:    Outputs a report of the characters currently in the tavern in the form:
  "Humans: [x] \nElves: [x] \nDwarves: [x] \nLizards: [x] \nUndead: [x] \n\nThe average level is: [x] \n[x]% are enemies.\n\n"

  Example output: 
  Humans: 5
  Elves: 8
  Dwarves: 3
  Lizards: 7
  Undead: 2

  The average level is: 16
  24% are enemies.
*/
void Tavern::tavernReport()
{
  int humans = tallyRace("HUMAN");
  int elves = tallyRace("ELF");
  int dwarves = tallyRace("DWARF");
  int lizards = tallyRace("LIZARD");
  int undead = tallyRace("UNDEAD");
  
  std::cout << "Humans: " << humans << std::endl;
  std::cout << "Elves: " << elves << std::endl;
  std::cout << "Dwarves: " << dwarves << std::endl;
  std::cout << "Lizards: " << lizards << std::endl;
  std::cout << "Undead: " << undead << std::endl;
  std::cout << "\nThe average level is: " << calculateAvgLevel() << std::endl;
  std::cout << std::fixed << std::setprecision(2) << calculateEnemyPercentage() << "% are enemies.\n\n";
}

/**
    @post: For every character in the tavern, displays each character's information
*/
void Tavern::displayCharacters() const{
  std::vector<Character*> tvn = toVector();
  for (int i = 0; i < tvn.size(); i++){
    tvn[i]->display();
  }
}

/**
    @param: a string reference to a race
    @post: For every character in the tavern of the given race (only exact matches to the input string), displays each character's information

*/
void Tavern::displayRace(const std::string& race) const{
  std::vector<Character*> tvn = toVector();
  for (int i = 0; i < tvn.size(); i++){
    if (tvn[i]->getRace() == race){
      tvn[i]->display();
    }
  }
}

/**
    @post: Every character in the tavern eats a tainted stew.
*/
void Tavern::taintedStew(){
  std::vector<Character*> tvn = toVector();
  for (int i = 0; i < tvn.size(); i++){
    tvn[i]->eatTaintedStew();
  }
}

/**
    @param: a string reference to MAIN from characters.csv (arrow types and quantities separated by ';')
    @return: a vector of arrows derived from MAIN
*/
std::vector<Arrows> Tavern::formatMain(const std::string& s_main){
  std::vector<Arrows> arrows;
  std::vector<std::string> types;
  std::vector<int> quants;
  std::string main = s_main + ';';
  while (main != ""){
    for (int i = 0; i < main.length(); i++){
      std::string type = "";
      std::string quantity = "";
      if (main[i] == ' '){
        type = main.substr(0, i);
        main = main.substr(i + 1);
        types.push_back(type);
        break;
      }
      else if (main[i] == ';'){
        quantity = main.substr(0, i);
        main = main.substr(i + 1);
        quants.push_back(stoi(quantity));
        break;
      }
    } 
  }
  for (int i = 0; i < types.size(); i++){
    Arrows arrow = {types[i], quants[i]};
    arrows.push_back(arrow);
  }
  return arrows;
}

/**
    @param: a string reference to MAIN from characters.csv (arrow types and quantities separated by ';')
    @return: a vector of arrows derived from MAIN
*/
std::vector<std::string> Tavern::formatAffinities(const std::string& s_affinities){
  std::vector<std::string> affinities;
  std::string n_affinities = s_affinities + ';';
  while (n_affinities != ""){
    for (int i = 0; i < n_affinities.size(); i++){
      std::string affinity = "";
      if (n_affinities[i] == ';'){
        affinity = n_affinities.substr(0, i);
        n_affinities = n_affinities.substr(i + 1);
        affinities.push_back(affinity);
        break;
      }
    }
  }
  return affinities;
}
