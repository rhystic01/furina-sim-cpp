#include "dummy.h"

Dummy::Dummy(){
    LVL = 100;
    Resistance = 0.1;
}
Dummy::~Dummy(){}

double Dummy::getLVL(){
    return LVL;
}

double Dummy::getResistance(){
    return Resistance;
}