#include "G2PP.h"

G2PP::G2PP()
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(nullptr),
    Samples(nullptr),
    DIRTY(0),
    params(new double[NUM_PARAMS]{0}),
    nthreads(1),
    Error(1,"Succeed")
{
    this->markDirtyAll();
}

G2PP::G2PP(Curve * curve)
    : RateModel(RateModelType::RMT_G2PP, "G2++: Gaussian 2-factor short rate model"),
    Sim(new Simulation()),
    YieldCurve(curve),
    Samples(nullptr),
    DIRTY(0),
    params(new double[NUM_PARAMS]{0}),
    nthreads(1),
    Error(1,"Succeed")
{
    this->markDirtyAll();
}

G2PP::~G2PP(){
    if (Samples){
        delete Samples;
    }
    delete [] params;
    delete Sim;
}

double G2PP::resize(){
    if (Samples){
        delete Samples;
    }
    Samples = new mat3d(5,100,100);
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
    for (i = PROC_SIMULATION; i <= PROC_EVOLUTION; i <<= 1u){
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
