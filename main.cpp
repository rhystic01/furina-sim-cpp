#include "furina.h"
#include "furina_skills.h"
#include "dummy.h"

#include <iostream>
#include <fstream>
#include <algorithm>

int main () {
    // spawn Furina, enemy dummy, and Furina's pets
    Furina f1;
    Dummy dummy1;
    Pets pet(f1);
    // check if Furina is at least c1 and adjust initial fanfare stacks and limit
    double starting_fanfare;
    double fanfare_limit;
    if(f1.getConstellation() >= 1) {
        starting_fanfare = 150;
        fanfare_limit = 400;
    } else {
        starting_fanfare = 0;
        fanfare_limit = 300;
    }

    // set limit for maxHP of Furina with C2 buff
    const double furina_C2_maxHP = f1.getMaxHP() + 21429.8;

    // generating vectors with pairs such as <time of the attack in cs, which pet is attacking> over 20s for each pet separately
    std::vector<std::pair<int,int>> attacks_vec_pet = generatePetAttacks(pet, 1, 2000);
    std::vector<std::pair<int,int>> attacks_vec_pet2 = generatePetAttacks(pet, 2, 2000);
    std::vector<std::pair<int,int>> attacks_vec_pet3 = generatePetAttacks(pet, 3, 2000);
    // merging all 3 vectors into 1
    attacks_vec_pet.insert(attacks_vec_pet.end(), attacks_vec_pet2.begin(), attacks_vec_pet2.end());
    attacks_vec_pet.insert(attacks_vec_pet.end(), attacks_vec_pet3.begin(), attacks_vec_pet3.end());
    // custom comparison function to be used below in the sort
    auto customCompare = [](const std::pair<int,int>& a, const std::pair<int,int>& b) {
        return a.first < b.first;
    };
    // sorting the merged vector by time of the attack. we get a sorted vector such as: [(time of the attack, which pet), (...), ...]
    std::sort(attacks_vec_pet.begin(), attacks_vec_pet.end(), customCompare);

    // preparing variables to do an example rotation
    double fanfare = starting_fanfare;          // current fanfare stacks counter
    double fanfare_gained = 0;                  // new fanfare stacks gained just after a pet attack
    double fanfare_overstack = starting_fanfare;// same counter as fanfare, needed for C2 calcs
    double fanfare_overstack_gained = 0;        // new fanfare stacks gained just after a pet attack, but can go over the limit (needed for C2 calcs)
    double dmgDealt = 0;                        // damage dealt to enemy dummy counter
    std::pair<double,double> pet_attack(0,0);   // pair to store return values of pet.attack(...) command which is (dmg_dealt, hp_drained)

    // opening a file to write out example simulation data
    std::ofstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Failed to open the file for writing." << std::endl;
        return 1;
    }

    // Using the merged and sorted vector we can simulate an example rotation and output data to a file
    outputFile<<"Pet"<<" "<<"Time(cs)"<<" "<<"DMG_Dealt"<<" "<<"Fanfare"<<" "<<"Total_DMG_dealt"<<" "<<"HP"<<" "<<"Max_HP"<<std::endl;
    for(const auto& pair : attacks_vec_pet) {
        if(pair.first >= 1800) {fanfare = starting_fanfare; fanfare_overstack = starting_fanfare;}
        pet_attack = pet.attack(pair.second, dummy1, f1, fanfare);
        dmgDealt += pet_attack.first;
        fanfare_gained = fanfareGained(pet_attack.second, f1.getConstellation());
        fanfare += fanfare_gained;
        fanfare_overstack += fanfare_gained;
        if(fanfare > fanfare_limit) {
            fanfare = fanfare_limit;
            if(f1.getConstellation()>=2) {
                fanfare_overstack_gained = std::abs(fanfare_overstack_gained - fanfare_overstack + fanfare_limit);
                f1.c2IncreaseMaxHP(fanfare_overstack_gained);
                if(f1.getMaxHP()>furina_C2_maxHP) {f1.setMaxHP(furina_C2_maxHP);}
            }
        }
        outputFile<<pair.second<<" "<<pair.first<<" "<<pet_attack.first<<" "<<fanfare<<" "<<dmgDealt<<" "<<f1.getCurrentHP()<<" "<<f1.getMaxHP()<<std::endl;
    }
    // closing the file
    outputFile.close();

    // cleaning up the mess and restoring Furina to initial state
    fanfare = starting_fanfare;
    fanfare_gained = 0;
    fanfare_overstack = starting_fanfare;
    fanfare_overstack_gained = 0;
    dmgDealt = 0;
    pet_attack = std::make_pair(0,0);
    attacks_vec_pet.clear();
    f1.Initialize();

    // here we will do the same thing as above but thousands of times without writing to a file

    int sims = 5000; // amount of times to perform the rotation
    // doing many rotations and adding up the damage dealt
    for(int j=0; j<sims; j++){
        attacks_vec_pet = generatePetAttacks(pet, 1, 2000);
        attacks_vec_pet2 = generatePetAttacks(pet, 2, 2000);
        attacks_vec_pet3 = generatePetAttacks(pet, 3, 2000);

        attacks_vec_pet.insert(attacks_vec_pet.end(), attacks_vec_pet2.begin(), attacks_vec_pet2.end());
        attacks_vec_pet.insert(attacks_vec_pet.end(), attacks_vec_pet3.begin(), attacks_vec_pet3.end());

        std::sort(attacks_vec_pet.begin(), attacks_vec_pet.end(), customCompare);

        for(const auto& pair : attacks_vec_pet) {
        if(pair.first >= 1800) {fanfare = starting_fanfare; fanfare_overstack = starting_fanfare;}
        pet_attack = pet.attack(pair.second, dummy1, f1, fanfare);
        dmgDealt += pet_attack.first;
        fanfare_gained = fanfareGained(pet_attack.second, f1.getConstellation());
        fanfare += fanfare_gained;
        fanfare_overstack += fanfare_gained;
        if(fanfare > fanfare_limit) {
            fanfare = fanfare_limit;
            if(f1.getConstellation()>=2) {
                fanfare_overstack_gained = std::abs(fanfare_overstack_gained - fanfare_overstack + fanfare_limit);
                f1.c2IncreaseMaxHP(fanfare_overstack_gained);
                if(f1.getMaxHP()>furina_C2_maxHP) {f1.setMaxHP(furina_C2_maxHP);}
            }
        }
    }

        // cleaning up the mess and restoring Furina to initial state, but keeping dmgDealt counting 
        fanfare = starting_fanfare;
        fanfare_gained = 0;
        fanfare_overstack = starting_fanfare;
        fanfare_overstack_gained = 0;
        pet_attack = std::make_pair(0,0);
        attacks_vec_pet.clear();
        f1.Initialize();
    }

    // printing out the average damage dealt
    std::cout << "Average damage dealt: " << dmgDealt/sims << std::endl;
    std::cout << "DPS: " << (dmgDealt/sims)/20 << std::endl;
    
    return 0;
}