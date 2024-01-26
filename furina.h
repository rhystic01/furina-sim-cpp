#ifndef __FURINA__

#define __FURINA__

#include "furina_skills.h"

class Furina {
    friend class Pets;
    double CurrentHP, MaxHP, CR, CD, Goblet_DMGBonus, Artifact_DMGBonus, A4_DMGBonus;
    int Constellation;
    public:
    double getCurrentHP();
    double getMaxHP();
    double getCR();
    double getCD();
    double getGoblet_DMGBonus();
    double getArtifact_DMGBonus();
    double getA4_DMGBonus();
    int getConstellation();
    void c2IncreaseMaxHP(double fanfare_overstack_gained);
    void setMaxHP(double HP);
    void Initialize();
    Furina();
    ~Furina();
};

#endif