#include "furina.h"

#include <cmath>

void Furina::Initialize(){
    Constellation = 0;
    MaxHP = 37390;
    CurrentHP = MaxHP;
    CR = 73.3/100;
    CD = 168.18/100;
    Goblet_DMGBonus = 0;
    Artifact_DMGBonus = 0.7;
    A4_DMGBonus = floor(MaxHP/1000) * 0.007;
    if(A4_DMGBonus > 0.28) {A4_DMGBonus = 0.28;}
}

Furina::Furina(){
    Initialize();
}
Furina::~Furina(){}

void Furina::c2IncreaseMaxHP(double fanfare_overstack_gained){
    if(fanfare_overstack_gained > 0) {
        MaxHP += 15307 * 0.0035 * fanfare_overstack_gained;
    }
}

void Furina::setMaxHP(double HP){
    MaxHP = HP;
}

int Furina::getConstellation(){
    return Constellation;
}

double Furina::getCurrentHP(){
    return CurrentHP;
}

double Furina::getMaxHP(){
    return MaxHP;
}

double Furina::getCR(){
    return CR;
}

double Furina::getCD(){
    return CD;
}

double Furina::getA4_DMGBonus(){
    return A4_DMGBonus;
}

double Furina::getArtifact_DMGBonus(){
    return Artifact_DMGBonus;
}

double Furina::getGoblet_DMGBonus(){
    return Goblet_DMGBonus;
}