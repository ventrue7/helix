#include "Curve.h"

Curve::Curve():length(0){

}

Curve::Curve(double * terms, double * prices, int len):length(len),T(new double[len]),PV(new double[len]){ //Deep Copy
    int i;
    for (i = 0; i < length; ++i){
        T[i] = terms[i];
        PV[i] = prices[i];
    }
}


Curve::~Curve(){
    delete [] T;
    delete [] PV;
}

void Curve::getClosestTenorIndices(double t, int& index, int& direction){
   if (t<T[0]){
       index = 0;
       direction = 1;
       return;
   }else if (t==T[0]){
       index = 0;
       direction = 0;
       return;
   }else if (t > T[length-1]){
       index = length-1;
       direction = -1;
       return;
   }else{
       int i;
       for (i = 1; i < length; ++i){
           if (t>T[i-1] && t<T[i]){
               index = i;
               direction = -1;
               return;
           }else if (t==T[i]){
               index = i;
               direction = 0;
               return;
           }
       }
   }
}

double Curve::P(double tau){
    int index, index1, direction;

    getClosestTenorIndices(tau, index, direction);

    if (direction == 0){
        return PV[index];
    }else{
        index1 = index + direction;
    }
    if (tau!=0){
        //log linear extrapolation
        return exp( ( (T[index1] - tau)*log(PV[index]) + (tau - T[index])*log(PV[index1]) )/(T[index1]-T[index]) );
    }else{
        return 1.0;
    }
}

double Curve::P(double tau, double Tau){
    return P(Tau)/P(tau);
}

double * Curve::getTenor(){
    return T;
}

double * Curve::getPV(){
    return PV;
}

int Curve::Length(){
    return length;
}
