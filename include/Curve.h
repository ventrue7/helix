/*
 * ==========================================================================
 *
 *       Filename:  Curve.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2017-09-17
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  James Ding
 *                  james.ding.illinois@gmail.com
 *
 * ==========================================================================
 */
#ifndef CURVE_H
#define CURVE_H

#include <cmath>

class Curve{
    private:
        double * T;     //Tenors
        double * PV;    //PVzcb

        int length;
    public:
        Curve();
        Curve(double*, double*, int);
        ~Curve();

        //Utilities:
        //assume T, PV are sorted from least to most
        void getClosestTenorIndices(double, int&, int&); //output the nearest 2 indices for inter/extrapolation
        //Log-Linear Interpolate
        double P(double);         //lnP(0, Tau) = (T-Tau)/(T-t) * lnP(0,t) + (Tau-t)/(T-t)lnP(0,T)
        double P(double, double); //P(t,T) = P(0,T)/P(0,t)

        //Getters/Setters
        double * getTenor();
        double * getPV();
        int      Length();

};


#endif
