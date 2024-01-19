/*
CSCI 235 Fall 2023
Project 4 - main
Created by Allison Lee on October 9, 2023
main.cpp tests functions from project 4
*/

#include "Character.hpp"
#include "Barbarian.hpp"
#include "Mage.hpp"
#include "Ranger.hpp"
#include "Scoundrel.hpp"
#include "Tavern.hpp"
#include <iostream>
#include <vector>
#include <fstream>

int main(){
    Tavern t("characters.csv");
    std::vector<Character*> tavern = t.toVector();
    t.displayCharacters();
    //tavern[12]->display();
}