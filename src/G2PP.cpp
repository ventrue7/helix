#include "G2PP.h"

G2PP::G2PP(int dim, int path, int term)
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(nullptr),
    Samples(term, path, dim),
    DIRTY(0),
    Error(1,"Succeed")
{
    this->markDirtyAll();
}

G2PP::G2PP(int dim, int path, int term, Curve * curve)
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(curve),
    Samples(term, path, dim),
    DIRTY(0),
    Error(1,"Succeed")
{
    this->markDirtyAll();
}

G2PP::~G2PP(){
    delete Sim;
}

double G2PP::M(double x, double y){
}

double G2PP::V(double t, double T){
}


//----Getters & Setters-----
Simulation* G2PP::getSimEngine(){
   return Sim;
}


//----Bitwise marker----
void G2PP::markDirty(int step){
    DIRTY |= step;
}

void G2PP::markDirtyAll(){
    int i;
    for (i = Procedure::PROC_SIMULATION; i <= Procedure::PROC_EVOLUTION; i <<= 1u){
        DIRTY |= i;
    }
}

void G2PP::clearDirty(int step){
    DIRTY &= ~step;
}

void G2PP::clearDirtyAll(){
    DIRTY = 0;
}

int G2PP::isDirty(int step){
    return DIRTY & step;
}
