#include "furina_skills.h"

#include <cmath>
#include <random>
#include <utility>
#include <vector>

void Pets::Initialize(Furina furina) {
    Interval1 = 1.57895*100;
    Drain1 = 1.6/100;

    Interval2 = 3.33333*100;
    Drain2 = 2.4/100;

    Interval3 = 5*100;
    Drain3 = 3.6/100;

    if (furina.getConstellation() >= 5) {
        Dmg1 = 6.87/100;
        Dmg2 = 12.67/100;
        Dmg3 = 17.61/100;
    } else {
        Dmg1 = 5.82/100;
        Dmg2 = 10.73/100;
        Dmg3 = 14.92/100;
    }
}

Pets::Pets(Furina furina){
    Initialize(furina);
}

Pets::~Pets(){}

double Pets::getDmg(int whichPet){
    switch (whichPet)
    {
    case 1:
        return Dmg1;
        break;
    case 2:
        return Dmg2;
        break;
    case 3:
        return Dmg3;
    default:
        return 0;
    }
}

double Pets::getDrain(int whichPet){
    switch (whichPet)
    {
    case 1:
        return Drain1;
        break;
    case 2:
        return Drain2;
        break;
    case 3:
        return Drain3;
    default:
        return 0;
    }
}

int Pets::getInterval(int whichPet){
    switch (whichPet)
    {
    case 1:
        return Interval1;
        break;
    case 2:
        return Interval2;
        break;
    case 3:
        return Interval3;
    default:
        return 0;
    }
}

bool shouldReturnTrue(double percentage) {
    if (percentage < 0.0 || percentage > 100.0) {
        return false;
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distribution(0.0, 100.0);
    double randomValue = distribution(gen);

    return randomValue <= percentage;
}

int getRandomIntInRange(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(gen);
}

std::pair<double,double> Pets::attack(int whichPet, Dummy dummy1, Furina& furina, double fanfareStacks) {
    // adjust fanfare to dmg conversion ratio based on Furi con
    double fanfare_to_dmg = 0.0025;
    if(furina.getConstellation() >= 3) {fanfare_to_dmg = 0.0031;}
        
    // setting dmg and drain of a pet according to which pet was choosen
    double Dmg = 0;
    double Drain = 0;
    switch (whichPet) {
    case 1:
        Dmg = Dmg1;
        Drain = Drain1;
        break;
    case 2:
        Dmg = Dmg2;
        Drain = Drain2;
        break;
    case 3:
        Dmg = Dmg3;
        Drain = Drain3;
        break;
    default:
        break;
    }

    // base damage of the skill 
    const double BaseDMG = Dmg * furina.getMaxHP();

    // enemy dummy defense multiplier
    const double enemyDefMult = 190/(290+dummy1.getLVL());

    //enemy dummy resistance multiplier
    const double enemyResMult = 1 - dummy1.getResistance();

    // damage bonus sources: furina A4, artifact set, goblet, fanfare stacks (dynamic)
    double DMGBonus = furina.getA4_DMGBonus() + furina.getArtifact_DMGBonus() + furina.getGoblet_DMGBonus() + fanfare_to_dmg*fanfareStacks;

    // checking if Furina is above 50% of her max HP so skill gets correct BaseDMGMultiplier
    // and draining HP of Furina accordingly
    int drained = 0;
    if(furina.getCurrentHP()/furina.getMaxHP() > 0.5) {
        drained = 4; //assuming we have 4 party members who got drained including Furina
        furina.CurrentHP = furina.CurrentHP - Drain * furina.getMaxHP();
    }
    double BaseDMGMultiplier = 1 + 0.1 * drained;

    // Crit rate simulation
    double Crit;
    if(shouldReturnTrue(furina.getCR()*100)) {
        Crit = 1 + furina.getCD();
    } else {
        Crit = 1;
    }

    // Final resulting damage of the attack
    double Damage = BaseDMG * BaseDMGMultiplier * (1 + DMGBonus) * Crit * enemyDefMult * enemyResMult;
    // returning the damage dealt and amount of hp drained in a team
    std::pair<double,double> return_values (Damage,Drain*drained);
    return return_values;
}

double fanfareGained (double drain, int furina_con) {
    int mult = 1;
    if(furina_con >=2) mult = 3.5;
    return drain * 100 * mult;
}

std::vector<std::pair<int,int>> generatePetAttacks (Pets pet, int whichPet, int rotationTime) { 
    int iterator = 1;
    int n = 1;
    std::vector<std::pair<int,int>> intervals1;
    intervals1.push_back(std::make_pair(0, whichPet));
    while(true) {
        intervals1.push_back(std::make_pair(pet.getInterval(whichPet) + getRandomIntInRange(-5,5) + intervals1.at(iterator-1).first, whichPet));
        if(intervals1.at(iterator).first>rotationTime) {
            intervals1.pop_back();
            break;
        }
        if(intervals1.at(iterator).first>3000*n) {
            intervals1.pop_back();
            intervals1.push_back(std::make_pair(3000*n, whichPet));
            n++;
        }
        iterator++;
    }

    return intervals1;
}
