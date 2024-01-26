#ifndef __FURINA_SKILLS__

#define __FURINA_SKILLS__

#include "dummy.h"
#include "furina.h"

#include <utility>
#include <vector>

class Furina;

class Pets {
    double Drain1, Drain2, Drain3, Dmg1, Dmg2, Dmg3;
    int Interval1, Interval2, Interval3;
    public:
    Pets(Furina furina);
    ~Pets();
    void Initialize(Furina furina);
    int getInterval(int whichPet);
    double getDrain(int whichPet);
    double getDmg(int whichPet);
    std::pair<double,double> attack(int whichPet, Dummy dummy1, Furina& furina, double fanfareStacks);
};

bool shouldReturnTrue(double percentage);
double fanfareGained(double drain, int furina_con);
int getRandomIntInRange(int a, int b);
std::vector<std::pair<int,int>> generatePetAttacks (Pets pet, int whichPet, int rotationTime);

#endif